#ifndef __WIN32K_INPUT_H
#define __WIN32K_INPUT_H

#include <internal/kbd.h>

NTSTATUS FASTCALL
InitInputImpl(VOID);
NTSTATUS FASTCALL
InitKeyboardImpl(VOID);
PUSER_MESSAGE_QUEUE W32kGetPrimitiveMessageQueue(VOID);
PKBDTABLES W32kGetDefaultKeyLayout(VOID);
VOID FASTCALL W32kKeyProcessMessage(LPMSG Msg, PKBDTABLES KeyLayout,
				    DWORD UniqueId);

#endif /* __WIN32K_INPUT_H */
