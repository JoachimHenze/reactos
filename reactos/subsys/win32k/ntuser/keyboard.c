/*
 *  ReactOS W32 Subsystem
 *  Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/* $Id: keyboard.c,v 1.14.2.1 2003/11/13 10:29:21 arty Exp $
 *
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS kernel
 * PURPOSE:          Messages
 * FILE:             subsys/win32k/ntuser/keyboard.c
 * PROGRAMER:        Casper S. Hornstrup (chorns@users.sourceforge.net)
 * REVISION HISTORY:
 *       06-06-2001  CSH  Created
 */

/* INCLUDES ******************************************************************/

#include <ddk/ntddk.h>
#include <win32k/win32k.h>
#include <internal/safe.h>
#include <internal/kbd.h>
#include <include/guicheck.h>
#include <include/msgqueue.h>
#include <include/window.h>
#include <include/class.h>
#include <include/error.h>
#include <include/object.h>
#include <include/winsta.h>

#define NDEBUG
#include <debug.h>

/* Directory to load key layouts from */
#define SYSTEMROOT_DIR L"\\SystemRoot\\System32\\"
/* Lock modifiers */
#define CAPITAL_BIT   0x80000000
#define NUMLOCK_BIT   0x40000000
#define MOD_BITS_MASK 0x3fffffff
#define MOD_KCTRL     0x02
/* Key States */
#define KS_DOWN_MASK     0xc0
#define KS_DOWN_BIT      0x80
#define KS_EXT_BIT       0x40
#define KS_LOCK_BIT    0x01
/* lParam bits */
#define LP_EXT_BIT       (1<<24)
/* From kbdxx.c -- Key changes with numlock */
#define KNUMP         0x400 

/* Lock the keyboard state to prevent unusual concurrent access */ 
KSPIN_LOCK QueueStateLock;
DWORD LastKeyboardMessage;

BYTE QueueKeyStateTable[256];

/* FUNCTIONS *****************************************************************/

/* Initialization -- Right now, just zero the key state and init the lock */
NTSTATUS FASTCALL InitKeyboardImpl(VOID) {
  KeInitializeSpinLock(&QueueStateLock);
  RtlZeroMemory(&QueueKeyStateTable,0x100);
  LastKeyboardMessage = 0;
  return STATUS_SUCCESS;
}

/*** Statics used by TranslateMessage ***/

static UINT DontDistinguishShifts( UINT ret ) {
    if( ret == VK_LSHIFT || ret == VK_RSHIFT ) ret = VK_SHIFT;
    if( ret == VK_LCONTROL || ret == VK_RCONTROL ) ret = VK_CONTROL;
    if( ret == VK_LMENU || ret == VK_RMENU ) ret = VK_MENU;
    return ret;
}

static VOID STDCALL SetKeyState(DWORD key, DWORD vk, DWORD ext, BOOL down) {
  /* Special handling for toggles like numpad and caps lock */
  if (vk == VK_CAPITAL || vk == VK_NUMLOCK) 
    {
      if (down) QueueKeyStateTable[key] ^= 1;
    } 

  if (down)
    QueueKeyStateTable[key] |= KS_DOWN_BIT | (ext ? KS_EXT_BIT : 0);
  else
    QueueKeyStateTable[key] &= ~KS_DOWN_MASK;
}

VOID DumpKeyState( PBYTE KeyState ) {
  int i;

  DbgPrint( "KeyState { " );
  for( i = 0; i < 0x100; i++ ) {
    if( KeyState[i] ) DbgPrint( "%02x(%02x) ", i, KeyState[i] );
  }
  DbgPrint( "};\n" );
}

static BYTE KeysSet( PKBDTABLES pkKT, PBYTE KeyState, 
		     int Mod, int FakeModLeft, int FakeModRight ) {
  int i;
  UINT Vk = 0;

  if( !KeyState || !pkKT ) return 0;

  for( i = 0; i < pkKT->bMaxVSCtoVK; i++ ) {
    Vk = pkKT->pusVSCtoVK[i] & 0xff;
    if( KeyState[i] && 
	((Vk == Mod) ||
	 (FakeModLeft && Vk == FakeModLeft) ||
	 (FakeModRight && Vk == FakeModRight)) ) {
      return KeyState[i];
    }
  }

  return 0;
}

static DWORD ModBits( PKBDTABLES pkKT, PBYTE KeyState ) {
  int i;
  DWORD ModBits = 0;
  BYTE Mask;

  if( !KeyState ) return 0;

  /* DumpKeyState( KeyState ); */

  for( i = 0; pkKT->pCharModifiers->pVkToBit[i].Vk; i++ ) {
    int Vk = pkKT->pCharModifiers->pVkToBit[i].Vk;
    switch(Vk)
      {
        case VK_SHIFT:
	  Mask = KeysSet( pkKT, KeyState, Vk, VK_LSHIFT, VK_RSHIFT );
          if (Mask & KS_DOWN_MASK)
	    ModBits |= pkKT->pCharModifiers->pVkToBit[i].ModBits;
	  break;
        case VK_CONTROL:
	  Mask = KeysSet( pkKT, KeyState, Vk, VK_LCONTROL, VK_RCONTROL );
          if (Mask & KS_DOWN_MASK)
	    ModBits |= pkKT->pCharModifiers->pVkToBit[i].ModBits;
	  break;
        case VK_MENU:
	  Mask = KeysSet( pkKT, KeyState, Vk, VK_LMENU, VK_RMENU );
          if (Mask & KS_DOWN_MASK)
	    ModBits |= pkKT->pCharModifiers->pVkToBit[i].ModBits;
          if (Mask & KS_EXT_BIT)
            ModBits |= MOD_KCTRL;
	  break;
	default:
	  Mask = KeysSet( pkKT, KeyState, Vk, 0, 0 );
          if (Mask & KS_DOWN_BIT)
	    ModBits |= pkKT->pCharModifiers->pVkToBit[i].ModBits;
	  break;
      }
  }

  /* Deal with VK_CAPITAL */
  if (KeysSet( pkKT, KeyState, VK_CAPITAL, 0, 0 ) & KS_LOCK_BIT) 
    {
      ModBits |= CAPITAL_BIT;
    }

  /* Deal with VK_NUMLOCK */
  if (KeysSet( pkKT, KeyState, VK_NUMLOCK, 0, 0 ) & KS_LOCK_BIT) 
    {
      ModBits |= NUMLOCK_BIT;
    }

  DPRINT( "Current Mod Bits: %x\n", ModBits );

  return ModBits;
}

static BOOL TryToTranslateChar(WORD wVirtKey,
			       DWORD ModBits,
			       PBOOL pbDead,
			       PBOOL pbLigature,
			       PWCHAR pwcTranslatedChar,
			       PKBDTABLES keyLayout ) 
{
  PVK_TO_WCHAR_TABLE vtwTbl;
  PVK_TO_WCHARS10 vkPtr;
  size_t size_this_entry;
  int nMod, shift;
  DWORD CapsMod = 0, CapsState = 0;

  CapsState = ModBits & ~MOD_BITS_MASK;
  ModBits = ModBits & MOD_BITS_MASK;

  DPRINT ( "TryToTranslate: %04x %x\n", wVirtKey, ModBits ); 

  if (ModBits > keyLayout->pCharModifiers->wMaxModBits)
    {
      return FALSE;
    }
  shift = keyLayout->pCharModifiers->ModNumber[ModBits];

  for (nMod = 0; keyLayout->pVkToWcharTable[nMod].nModifications; nMod++)
    {
      if (shift > keyLayout->pVkToWcharTable[nMod].nModifications)
        {
	  continue;
	}
      vtwTbl = &keyLayout->pVkToWcharTable[nMod];
      size_this_entry = vtwTbl->cbSize;
      vkPtr = (PVK_TO_WCHARS10)((BYTE *)vtwTbl->pVkToWchars);
      while(vkPtr->VirtualKey)
        {
          if( wVirtKey == (vkPtr->VirtualKey & 0xff) )
	    {
	      CapsMod = 
		shift | ((CapsState & CAPITAL_BIT) ? vkPtr->Attributes : 0);
	      
	      *pbDead = vkPtr->wch[CapsMod] == WCH_DEAD;
	      *pbLigature = vkPtr->wch[CapsMod] == WCH_LGTR;
	      *pwcTranslatedChar = vkPtr->wch[CapsMod];
	      
	      DPRINT("CapsMod %08x CapsState %08x shift %08x Char %04x\n",
		     CapsMod, CapsState, shift, *pwcTranslatedChar);

	      if( *pbDead ) 
	        {
                  vkPtr = (PVK_TO_WCHARS10)(((BYTE *)vkPtr) + size_this_entry);
	          if( vkPtr->VirtualKey != 0xff ) 
	            {
	              DPRINT( "Found dead key with no trailer in the table.\n" );
	              DPRINT( "VK: %04x, ADDR: %08x\n", wVirtKey, (int)vkPtr );
	              return FALSE;
		    }
	          *pwcTranslatedChar = vkPtr->wch[shift];
	        }
	        return TRUE;
	    }
          vkPtr = (PVK_TO_WCHARS10)(((BYTE *)vkPtr) + size_this_entry);
	}
    }
  return FALSE;
}

static
int STDCALL
ToUnicodeInner(UINT wVirtKey,
	       UINT wScanCode,
	       PBYTE lpKeyState,
	       LPWSTR pwszBuff,
	       int cchBuff,
	       UINT wFlags,
	       PKBDTABLES pkKT)
{
  WCHAR wcTranslatedChar;
  BOOL bDead;
  BOOL bLigature;

  if( !pkKT ) return 0;

  if( TryToTranslateChar( wVirtKey,
			  ModBits( pkKT, lpKeyState ),
			  &bDead,
			  &bLigature,
			  &wcTranslatedChar, 
			  pkKT ) ) 
    {
      if( bLigature ) 
        {
	  DPRINT("Not handling ligature (yet)\n" );
	  return 0;
        }

      if( cchBuff > 0 ) pwszBuff[0] = wcTranslatedChar;

      return bDead ? -1 : 1;
    }

  return 0;
}

DWORD
STDCALL
NtUserGetKeyState(
  DWORD key)
{
  KIRQL OldIrql;
  DWORD ret = 0;

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);
  if( key < 0x100 ) {
    ret = ((DWORD)(QueueKeyStateTable[key] & KS_DOWN_BIT) << 8 ) |
      (QueueKeyStateTable[key] & KS_EXT_BIT) |
      (QueueKeyStateTable[key] & KS_LOCK_BIT);
  }
  KeReleaseSpinLock(&QueueStateLock, OldIrql);
  return ret;
}

int STDCALL ToUnicodeEx( UINT wVirtKey,
			 UINT wScanCode,
			 PBYTE lpKeyState,
			 LPWSTR pwszBuff,
			 int cchBuff,
			 UINT wFlags,
			 HKL dwhkl ) {
  KIRQL OldIrql;
  int ToUnicodeResult = 0;

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);
  ToUnicodeResult = ToUnicodeInner( wVirtKey,
				    wScanCode,
				    lpKeyState,
				    pwszBuff,
				    cchBuff,
				    wFlags,
				    PsGetWin32Thread() ? 
				    PsGetWin32Thread()->KeyboardLayout : 0 );
  KeReleaseSpinLock(&QueueStateLock, OldIrql);

  return ToUnicodeResult;
}

int STDCALL ToUnicode( UINT wVirtKey,
		       UINT wScanCode,
		       PBYTE lpKeyState,
		       LPWSTR pwszBuff,
		       int cchBuff,
		       UINT wFlags ) {
  return ToUnicodeEx( wVirtKey,
		      wScanCode,
		      QueueKeyStateTable,
		      pwszBuff,
		      cchBuff,
		      wFlags,
		      0 );
}

/* 
 * Utility to copy and append two unicode strings.
 *
 * IN OUT PUNICODE_STRING ResultFirst -> First string and result
 * IN     PUNICODE_STRING Second      -> Second string to append
 * IN     BOOL            Deallocate  -> TRUE: Deallocate First string before
 *                                       overwriting.
 *
 * Returns NTSTATUS.
 */

static NTSTATUS ReallyAppendUnicodeString(PUNICODE_STRING ResultFirst,
					  PUNICODE_STRING Second,
					  BOOL Deallocate) {
  NTSTATUS Status;
  PWSTR new_string = 
    ExAllocatePool(PagedPool,
		   (ResultFirst->Length + Second->Length + sizeof(WCHAR)));
  if( !new_string ) {
    return STATUS_NO_MEMORY;
  }
  memcpy( new_string, ResultFirst->Buffer, 
	  ResultFirst->Length );
  memcpy( new_string + ResultFirst->Length / sizeof(WCHAR),
	  Second->Buffer,
	  Second->Length );
  if( Deallocate ) RtlFreeUnicodeString(ResultFirst);
  ResultFirst->Length += Second->Length;
  ResultFirst->MaximumLength = ResultFirst->Length;
  new_string[ResultFirst->Length / sizeof(WCHAR)] = 0;
  Status = RtlCreateUnicodeString(ResultFirst,new_string) ? 
    STATUS_SUCCESS : STATUS_NO_MEMORY;
  ExFreePool(new_string);
  return Status;
}

/*
 * Utility function to read a value from the registry more easily.
 *
 * IN  PUNICODE_STRING KeyName       -> Name of key to open
 * IN  PUNICODE_STRING ValueName     -> Name of value to open
 * OUT PUNICODE_STRING ReturnedValue -> String contained in registry
 *
 * Returns NTSTATUS
 */

static NTSTATUS ReadRegistryValue( PUNICODE_STRING KeyName,
				   PUNICODE_STRING ValueName,
				   PUNICODE_STRING ReturnedValue ) {
  NTSTATUS Status;
  HANDLE KeyHandle;
  OBJECT_ATTRIBUTES KeyAttributes;
  PKEY_VALUE_PARTIAL_INFORMATION KeyValuePartialInfo;
  ULONG Length = 0;
  ULONG ResLength = 0;
  UNICODE_STRING Temp;

  InitializeObjectAttributes(&KeyAttributes, KeyName, OBJ_CASE_INSENSITIVE,
			     NULL, NULL);
  Status = ZwOpenKey(&KeyHandle, KEY_ALL_ACCESS, &KeyAttributes);
  if( !NT_SUCCESS(Status) ) {
    return Status;
  }

  Status = ZwQueryValueKey(KeyHandle, ValueName, KeyValuePartialInformation,
			   0,
			   0,
			   &ResLength);

  if( Status != STATUS_BUFFER_TOO_SMALL ) {
    NtClose(KeyHandle);
    return Status;
  }

  ResLength += sizeof( *KeyValuePartialInfo );
  KeyValuePartialInfo = 
    ExAllocatePool(PagedPool, ResLength);
  Length = ResLength;

  if( !KeyValuePartialInfo ) {
    NtClose(KeyHandle);
    return STATUS_NO_MEMORY;
  }

  Status = ZwQueryValueKey(KeyHandle, ValueName, KeyValuePartialInformation,
			   (PVOID)KeyValuePartialInfo,
			   Length,
			   &ResLength);

  if( !NT_SUCCESS(Status) ) {
    NtClose(KeyHandle);
    ExFreePool(KeyValuePartialInfo);
    return Status;
  }

  Temp.Length = Temp.MaximumLength = KeyValuePartialInfo->DataLength;
  Temp.Buffer = (PWCHAR)KeyValuePartialInfo->Data;
  
  /* At this point, KeyValuePartialInfo->Data contains the key data */
  RtlInitUnicodeString(ReturnedValue,L"");
  ReallyAppendUnicodeString(ReturnedValue,&Temp,FALSE);

  ExFreePool(KeyValuePartialInfo);
  NtClose(KeyHandle);

  return Status;
}

typedef PVOID (*KbdLayerDescriptor)(VOID);
NTSTATUS STDCALL LdrGetProcedureAddress(PVOID module,
					PANSI_STRING import_name,
					DWORD flags,
					PVOID *func_addr);

void InitKbdLayout( PVOID *pkKeyboardLayout ) {
  UNICODE_STRING KeyName;
  UNICODE_STRING ValueName;
  UNICODE_STRING LayoutKeyName;
  UNICODE_STRING LayoutValueName;
  UNICODE_STRING DefaultLocale;
  UNICODE_STRING LayoutFile;
  UNICODE_STRING FullLayoutPath;
  PWCHAR KeyboardLayoutWSTR;
  HMODULE kbModule = 0;
  NTSTATUS Status;
  ANSI_STRING kbdProcedureName;
  KbdLayerDescriptor layerDescGetFn;

  #define XX_STATUS(x) if (!NT_SUCCESS(Status = (x))) continue;

  do {
    RtlInitUnicodeString(&KeyName,
			 L"\\REGISTRY\\Machine\\SYSTEM\\CurrentControlSet"
			 L"\\Control\\Nls\\Locale");
    RtlInitUnicodeString(&ValueName,
			 L"(Default)");
       
    DPRINT("KeyName = %wZ, ValueName = %wZ\n", &KeyName, &ValueName);

    Status = ReadRegistryValue(&KeyName,&ValueName,&DefaultLocale);
    
    if( !NT_SUCCESS(Status) ) {
      DbgPrint( "Could not get default locale (%08x).\n", Status );
    } else {
      DPRINT( "DefaultLocale = %wZ\n", &DefaultLocale );

      RtlInitUnicodeString(&LayoutKeyName,
			   L"\\REGISTRY\\Machine\\SYSTEM\\CurrentControlSet"
			   L"\\Control\\KeyboardLayouts\\");

      ReallyAppendUnicodeString(&LayoutKeyName,&DefaultLocale,FALSE);

      RtlFreeUnicodeString(&DefaultLocale);
      RtlInitUnicodeString(&LayoutValueName,L"Layout File");

      Status = ReadRegistryValue(&LayoutKeyName,&LayoutValueName,&LayoutFile);
      RtlInitUnicodeString(&FullLayoutPath,SYSTEMROOT_DIR);

      if( !NT_SUCCESS(Status) ) {
	DbgPrint("Got default locale but not layout file. (%08x)\n",
		 Status);
	RtlFreeUnicodeString(&LayoutFile);
      } else {
	DPRINT("Read registry and got %wZ\n", &LayoutFile);
    
	RtlFreeUnicodeString(&LayoutKeyName);

	ReallyAppendUnicodeString(&FullLayoutPath,&LayoutFile,FALSE);

	DPRINT("Loading Keyboard DLL %wZ\n", &FullLayoutPath);

	RtlFreeUnicodeString(&LayoutFile);

	KeyboardLayoutWSTR = ExAllocatePool(PagedPool,
					    (FullLayoutPath.Length + 1) * 
					    sizeof(WCHAR));

	if( !KeyboardLayoutWSTR ) {
	  DbgPrint("Couldn't allocate a string for the keyboard layout name.\n");
	  RtlFreeUnicodeString(&FullLayoutPath);
	  return;
	}
	memcpy(KeyboardLayoutWSTR,FullLayoutPath.Buffer,
	       (FullLayoutPath.Length + 1) * sizeof(WCHAR));
	KeyboardLayoutWSTR[FullLayoutPath.Length] = 0;

	kbModule = EngLoadImage(KeyboardLayoutWSTR);
	DPRINT( "Load Keyboard Layout: %S\n", KeyboardLayoutWSTR );

        if( !kbModule )
	  DbgPrint( "Load Keyboard Layout: No %wZ\n", &FullLayoutPath );
      }

      RtlFreeUnicodeString(&FullLayoutPath);
    }

    if( !kbModule )
    {
      DbgPrint("Trying to load US Keyboard Layout\n");
      kbModule = EngLoadImage(L"\\SystemRoot\\system32\\kbdus.dll");
      
      if (!kbModule)
      {
        DbgPrint("Failed to load any Keyboard Layout\n");
        return;
	    }
    }

    RtlInitAnsiString( &kbdProcedureName, "KbdLayerDescriptor" );

    LdrGetProcedureAddress((PVOID)kbModule,
			   &kbdProcedureName,
			   0,
			   (PVOID*)&layerDescGetFn);
    
    if( layerDescGetFn ) {
      *pkKeyboardLayout = layerDescGetFn();
    }
  } while (FALSE);

  if( !*pkKeyboardLayout ) {
    DbgPrint("Failed to load the keyboard layout.\n");
  }

#undef XX_STATUS
}

PKBDTABLES W32kGetDefaultKeyLayout() {
  PKBDTABLES pkKeyboardLayout = 0;
  InitKbdLayout( (PVOID) &pkKeyboardLayout );
  return pkKeyboardLayout;
}

BOOL STDCALL
NtUserTranslateMessage(LPMSG lpMsg,
		       HKL dwhkl) /* Used to pass the kbd layout */
{
  KIRQL OldIrql;
  static INT dead_char = 0;
  LONG UState = 0;
  WCHAR wp[2] = { 0 };
  MSG NewMsg = { 0 };
  MSG InMsg = { 0 };
  PUSER_MESSAGE UMsg;
  PKBDTABLES keyLayout;
  BOOL Result = FALSE;
  DWORD ScanCode = 0;

  if( !NT_SUCCESS(MmCopyFromCaller(&InMsg, lpMsg, sizeof(InMsg))) ) {
    return FALSE;
  }

  keyLayout = PsGetWin32Thread()->KeyboardLayout;
  if( !keyLayout )
    return FALSE;

  if (InMsg.message != WM_KEYDOWN && InMsg.message != WM_SYSKEYDOWN)
    return FALSE;

  ScanCode = (InMsg.lParam >> 16) & 0xff;

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);

  UState = ToUnicodeInner(InMsg.wParam, HIWORD(InMsg.lParam) & 0xff,
			  QueueKeyStateTable, wp, 2, 0, 
			  keyLayout );

  if (UState == 1)
    {
      NewMsg.message = (InMsg.message == WM_KEYDOWN) ? WM_CHAR : WM_SYSCHAR;
      if (dead_char)
        {
	  ULONG i;
	  WCHAR first, second;
	  DPRINT("PREVIOUS DEAD CHAR: %c\n", dead_char);

	  for( i = 0; keyLayout->pDeadKey[i].dwBoth; i++ )
	    {
	      first = keyLayout->pDeadKey[i].dwBoth >> 16;
	      second = keyLayout->pDeadKey[i].dwBoth;
	      if (first == dead_char && second == wp[0]) 
		{
		  wp[0] = keyLayout->pDeadKey[i].wchComposed;
		  dead_char = 0;
 		  break;
		}
	    }

	  DPRINT("FINAL CHAR: %c\n", wp[0]);
	}
      if (dead_char) 
	{
	  NewMsg.hwnd = InMsg.hwnd;
	  NewMsg.wParam = dead_char;
	  NewMsg.lParam = InMsg.lParam;
	  UMsg = MsqCreateMessage(&NewMsg);
	  dead_char = 0;
	  if (UMsg)
	    MsqPostMessage(PsGetWin32Thread()->MessageQueue, UMsg);
	}
      
      NewMsg.hwnd = InMsg.hwnd;
      NewMsg.wParam = wp[0];
      NewMsg.lParam = InMsg.lParam;
      UMsg = MsqCreateMessage(&NewMsg);
      DPRINT( "CHAR='%c' %04x %08x\n", wp[0], wp[0], InMsg.lParam );
      if (UMsg) 
	MsqPostMessage(PsGetWin32Thread()->MessageQueue, UMsg);
      Result = TRUE;
    }
  else if (UState == -1)
    {
      NewMsg.message = 
	(InMsg.message == WM_KEYDOWN) ? WM_DEADCHAR : WM_SYSDEADCHAR;
      NewMsg.hwnd = InMsg.hwnd;
      NewMsg.wParam = wp[0];
      NewMsg.lParam = InMsg.lParam;
      dead_char = wp[0];
      UMsg = MsqCreateMessage(&NewMsg);
      if (UMsg)
	MsqPostMessage(PsGetWin32Thread()->MessageQueue, UMsg);
      Result = TRUE;
    }

  KeReleaseSpinLock(&QueueStateLock, OldIrql);
  return Result;
}

HWND STDCALL
NtUserSetFocus(HWND hWnd)
{
  return IntSetFocusWindow(hWnd);
}

DWORD
STDCALL
NtUserGetKeyboardState(
  LPBYTE lpKeyState)
{
  KIRQL OldIrql;
  BOOL Result = TRUE;

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);
  if (lpKeyState) {
	if(!NT_SUCCESS(MmCopyToCaller(lpKeyState, QueueKeyStateTable, 256)))
	  Result = FALSE;
  }
  KeReleaseSpinLock(&QueueStateLock, OldIrql);
  return Result;
}

DWORD
STDCALL
NtUserSetKeyboardState(
  LPBYTE lpKeyState)
{
  KIRQL OldIrql;
  BOOL Result = TRUE;

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);
  if (lpKeyState) {
    if(! NT_SUCCESS(MmCopyFromCaller(QueueKeyStateTable, lpKeyState, 256)))
      Result = FALSE;
  }
  KeReleaseSpinLock(&QueueStateLock, OldIrql);
  
  return Result;
}

static UINT VkToScan( UINT Code, BOOL ExtCode, PKBDTABLES pkKT ) {
  int i;

  for( i = 0; i < pkKT->bMaxVSCtoVK; i++ ) {
    if( pkKT->pusVSCtoVK[i] == Code ) { return i; }
  }

  return 0;
}

UINT ScanToVk( UINT Code, BOOL ExtKey, PKBDTABLES pkKT ) {
  if( ExtKey ) {
    int i;

    for( i = 0; pkKT->pVSCtoVK_E0[i].Vsc; i++ ) {
      if( pkKT->pVSCtoVK_E0[i].Vsc == Code ) 
	return pkKT->pVSCtoVK_E0[i].Vk & 0xff;
    }
    for( i = 0; pkKT->pVSCtoVK_E1[i].Vsc; i++ ) {
      if( pkKT->pVSCtoVK_E1[i].Vsc == Code ) 
	return pkKT->pVSCtoVK_E1[i].Vk & 0xff;
    }

    return 0;
  } else {
    if( Code >= pkKT->bMaxVSCtoVK ) { return 0; }
    return pkKT->pusVSCtoVK[Code] & 0xff;
  }
}

/*
 * Map a virtual key code, or virtual scan code, to a scan code, key code,
 * or unshifted unicode character.
 *
 * Code: See Below
 * Type:
 * 0 -- Code is a virtual key code that is converted into a virtual scan code
 *      that does not distinguish between left and right shift keys.
 * 1 -- Code is a virtual scan code that is converted into a virtual key code
 *      that does not distinguish between left and right shift keys.
 * 2 -- Code is a virtual key code that is converted into an unshifted unicode
 *      character.
 * 3 -- Code is a virtual scan code that is converted into a virtual key code
 *      that distinguishes left and right shift keys.
 * KeyLayout: Keyboard layout handle (currently, unused)
 *
 * @implemented
 */

UINT
STDCALL
NtUserMapVirtualKeyEx( UINT Code, UINT Type, DWORD keyboardId, HKL dwhkl ) {
  UINT ret = 0;
  PKBDTABLES keyLayout = PsGetWin32Thread()->KeyboardLayout;

  if( !keyLayout ) return 0;

  switch( Type ) {
  case 0:
    if( Code == VK_RSHIFT ) Code = VK_LSHIFT;
    if( Code == VK_RMENU ) Code = VK_LMENU;
    if( Code == VK_RCONTROL ) Code = VK_LCONTROL;
    ret = VkToScan( Code, FALSE, keyLayout );
    break;

  case 1:
    ret = 
      DontDistinguishShifts
      (NtUserMapVirtualKeyEx( Code, 3, keyboardId, dwhkl ) );
    break;

  case 2: {
    WCHAR wp[2];

    ret = VkToScan( Code, FALSE, keyLayout );
    ToUnicodeInner( Code, ret, 0, wp, 2, 0, keyLayout );
    ret = wp[0];
  } break;

  case 3:
    ret = ScanToVk( Code, FALSE, keyLayout );
    break;
  }

  return ret;
}


int
STDCALL
NtUserToUnicodeEx(
		  UINT wVirtKey,
		  UINT wScanCode,
		  PBYTE lpKeyState,
		  LPWSTR pwszBuff,
		  int cchBuff,
		  UINT wFlags,
		  HKL dwhkl ) {
  BYTE KeyStateBuf[0x100];
  PWCHAR OutPwszBuff = 0;
  int ret = 0;
  

  if( !NT_SUCCESS(MmCopyFromCaller(KeyStateBuf,
				   lpKeyState,
				   sizeof(KeyStateBuf))) ) {
    DbgPrint( "Couldn't copy key state from caller.\n" );
    return 0;
  }
  OutPwszBuff = ExAllocatePool(NonPagedPool,sizeof(WCHAR) * cchBuff);
  if( !OutPwszBuff ) {
    DbgPrint( "ExAllocatePool(%d) failed\n", sizeof(WCHAR) * cchBuff);
    return 0;
  }
  RtlZeroMemory( OutPwszBuff, sizeof( WCHAR ) * cchBuff );

  ret = ToUnicodeEx( wVirtKey,
		     wScanCode,
		     KeyStateBuf,
		     OutPwszBuff,
		     cchBuff,
		     wFlags,
		     dwhkl );  

  MmCopyToCaller(pwszBuff,OutPwszBuff,sizeof(WCHAR)*cchBuff);
  ExFreePool(OutPwszBuff);

  return ret;
}

static int W32kSimpleToupper( int ch ) {
  if( ch >= 'a' && ch <= 'z' ) ch = ch - 'a' + 'A';
  return ch;
}

DWORD
STDCALL
NtUserGetKeyNameText( LONG lParam, LPWSTR lpString, int nSize ) {
  int i;
  DWORD ret = 0;
  UINT CareVk = 0;
  UINT VkCode = 0;
  UINT ScanCode = (lParam >> 16) & 0xff;
  BOOL ExtKey = lParam & (1<<24) ? TRUE : FALSE;
  PKBDTABLES keyLayout = 
    PsGetWin32Thread() ? 
    PsGetWin32Thread()->KeyboardLayout : 0;

  if( !keyLayout || nSize < 1 ) return 0;

  if( lParam & (1<<25) ) {
    CareVk = VkCode = ScanToVk( ScanCode, ExtKey, keyLayout );
    if( VkCode == VK_LSHIFT || VkCode == VK_RSHIFT )
      VkCode = VK_LSHIFT;
    if( VkCode == VK_LCONTROL || VkCode == VK_RCONTROL )
      VkCode = VK_LCONTROL;
    if( VkCode == VK_LMENU || VkCode == VK_RMENU )
      VkCode = VK_LMENU;
  } else {
    VkCode = ScanToVk( ScanCode, ExtKey, keyLayout );
  }

  VSC_LPWSTR *KeyNames = 0;

  if( CareVk != VkCode ) 
    ScanCode = VkToScan( VkCode, ExtKey, keyLayout );
  
  if( ExtKey ) 
    KeyNames = keyLayout->pKeyNamesExt;
  else 
    KeyNames = keyLayout->pKeyNames;

  for( i = 0; KeyNames[i].pwsz; i++ ) {
    if( KeyNames[i].vsc == ScanCode ) {
      UINT StrLen = wcslen(KeyNames[i].pwsz);
      UINT StrMax = StrLen > (nSize - 1) ? (nSize - 1) : StrLen;
      WCHAR null_wc = 0;
      if( NT_SUCCESS( MmCopyToCaller( lpString, 
				      KeyNames[i].pwsz, 
				      StrMax * sizeof(WCHAR) ) ) &&
	  NT_SUCCESS( MmCopyToCaller( lpString + StrMax,
				      &null_wc,
				      sizeof( WCHAR ) ) ) ) {
	ret = StrMax;
	break;
      }
    }
  }

  if( ret == 0 ) {
    WCHAR UCName[2];

    UCName[0] = W32kSimpleToupper(NtUserMapVirtualKeyEx( VkCode, 2, 0, 0 ));
    UCName[1] = 0;
    ret = 1;

    if( !NT_SUCCESS(MmCopyToCaller( lpString, UCName, 2 * sizeof(WCHAR) )) )
      return 0;
  }

  return ret;
}

/*
 * Filter this message according to the current key layout, setting wParam
 * appropriately.
 */

VOID FASTCALL W32kKeyProcessMessage(LPMSG Msg, PKBDTABLES KeyboardLayout,
				    DWORD UniqueId) {
  KIRQL OldIrql;
  DWORD ScanCode = 0, ModifierBits = 0;
  DWORD i = 0;
  DWORD RawVk = 0;
  static WORD NumpadConversion[][2] = 
    { { VK_DELETE, VK_DECIMAL },
      { VK_INSERT, VK_NUMPAD0 },
      { VK_END,    VK_NUMPAD1 },
      { VK_DOWN,   VK_NUMPAD2 },
      { VK_NEXT,   VK_NUMPAD3 },
      { VK_LEFT,   VK_NUMPAD4 },
      { VK_CLEAR,  VK_NUMPAD5 },
      { VK_RIGHT,  VK_NUMPAD6 },
      { VK_HOME,   VK_NUMPAD7 },
      { VK_UP,     VK_NUMPAD8 },
      { VK_PRIOR,  VK_NUMPAD9 },
      { 0,0 } };

  if( !KeyboardLayout || !Msg || 
      (Msg->message != WM_KEYDOWN && Msg->message != WM_SYSKEYDOWN &&
       Msg->message != WM_KEYUP   && Msg->message != WM_SYSKEYUP) ) 
    {
      return;
    }

  KeAcquireSpinLock(&QueueStateLock, &OldIrql);
  if (UniqueId - LastKeyboardMessage == 1) 
    {
      /* This is the first time we've processed this message.  Set the key
	 state */
      LastKeyboardMessage = UniqueId;
    }
  else
    {
      KeReleaseSpinLock(&QueueStateLock, OldIrql);
      return;
    }

  /* arty -- handle numpad -- On real windows, the actual key produced 
   * by the messaging layer is different based on the state of numlock. */
  ModifierBits = ModBits(KeyboardLayout,QueueKeyStateTable);

  /* Get the raw scan code, so we can look up whether the key is a numpad
   * key */
  ScanCode = (Msg->lParam >> 16) & 0xff;
  Msg->wParam = NtUserMapVirtualKeyEx( ScanCode, 1, 0, 0 );
  RawVk = KeyboardLayout->pusVSCtoVK[ScanCode];

  if ((ModifierBits & NUMLOCK_BIT) && (RawVk & KNUMP)) 
    {
      /* The key in question is a numpad key.  Search for a translation. */
      for (i = 0; NumpadConversion[i][0]; i++) 
	{
	  if ((RawVk & 0xff) == NumpadConversion[i][0]) 
	    {
	      DbgPrint("NUMLOCK: Converting %04x to %04x\n",
		       NumpadConversion[i][0],
		       NumpadConversion[i][1]);

	      Msg->wParam = NumpadConversion[i][1];
	      break;
	    }
	}
    }

  /* Now that we have the VK, we can set the keymap appropriately
   * This is a better place for this code, as it's guaranteed to be
   * run, unlike translate message. */
  if (Msg->message == WM_KEYDOWN || Msg->message == WM_SYSKEYDOWN)
    {
      if (Msg->lParam & LP_EXT_BIT)
	DbgPrint("Ext key hit %04x %08x\n", Msg->wParam, Msg->lParam);
      SetKeyState( ScanCode, Msg->wParam, Msg->lParam & LP_EXT_BIT,
		   TRUE ); /* Strike key */
    }
  else if (Msg->message == WM_KEYUP || Msg->message == WM_SYSKEYUP)
    {
      SetKeyState( ScanCode, Msg->wParam, Msg->lParam & LP_EXT_BIT,
		   FALSE ); /* Release key */
    }

  KeReleaseSpinLock(&QueueStateLock, OldIrql);
}
/* EOF */
