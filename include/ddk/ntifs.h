/*
 * ntifs.h
 *
 * Windows NT Filesystem Driver Developer Kit
 *
 * This file is part of the w32api package.
 *
 * Contributors:
 *   Created by Bo Brant�n <bosse@acc.umu.se>
 *
 * THIS SOFTWARE IS NOT COPYRIGHTED
 *
 * This source code is offered for use in the public domain. You may
 * use, modify or distribute it freely.
 *
 * This code is distributed in the hope that it will be useful but
 * WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 * DISCLAIMED. This includes but is not limited to warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#pragma once

#define _NTIFS_INCLUDED_
#define _GNU_NTIFS_

/* Helper macro to enable gcc's extension.  */
#ifndef __GNU_EXTENSION
#ifdef __GNUC__
#define __GNU_EXTENSION __extension__
#else
#define __GNU_EXTENSION
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_NTHALDLL_) && !defined(_BLDR_)
#define NTHALAPI DECLSPEC_IMPORT
#else
#define NTHALAPI
#endif

#if !defined(_NTOSKRNL_) /* For ReactOS */
#define NTKERNELAPI DECLSPEC_IMPORT
#else
#define NTKERNELAPI
#endif

/* Dependencies */
#include <ntddk.h>
#include <excpt.h>
#include <ntdef.h>
#include <ntnls.h>
#include <ntstatus.h>
#include <bugcodes.h>
/* FIXME : #include <ntiologc.h> */

#ifndef FlagOn
#define FlagOn(_F,_SF)        ((_F) & (_SF))
#endif

#ifndef BooleanFlagOn
#define BooleanFlagOn(F,SF)   ((BOOLEAN)(((F) & (SF)) != 0))
#endif

#ifndef SetFlag
#define SetFlag(_F,_SF)       ((_F) |= (_SF))
#endif

#ifndef ClearFlag
#define ClearFlag(_F,_SF)     ((_F) &= ~(_SF))
#endif

#define PsGetCurrentProcess IoGetCurrentProcess

#if (NTDDI_VERSION >= NTDDI_VISTA)
extern NTSYSAPI volatile CCHAR KeNumberProcessors;
#elif (NTDDI_VERSION >= NTDDI_WINXP)
extern NTSYSAPI CCHAR KeNumberProcessors;
#else
extern PCCHAR KeNumberProcessors;
#endif

typedef UNICODE_STRING LSA_UNICODE_STRING, *PLSA_UNICODE_STRING;
typedef STRING LSA_STRING, *PLSA_STRING;
typedef OBJECT_ATTRIBUTES LSA_OBJECT_ATTRIBUTES, *PLSA_OBJECT_ATTRIBUTES;

#ifndef SID_IDENTIFIER_AUTHORITY_DEFINED
#define SID_IDENTIFIER_AUTHORITY_DEFINED
typedef struct _SID_IDENTIFIER_AUTHORITY {
  UCHAR Value[6];
} SID_IDENTIFIER_AUTHORITY,*PSID_IDENTIFIER_AUTHORITY,*LPSID_IDENTIFIER_AUTHORITY;
#endif

#ifndef SID_DEFINED
#define SID_DEFINED
typedef struct _SID {
   UCHAR  Revision;
   UCHAR  SubAuthorityCount;
   SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
   ULONG SubAuthority[ANYSIZE_ARRAY];
} SID, *PISID;
#endif

#define SID_REVISION                    1
#define SID_MAX_SUB_AUTHORITIES         15
#define SID_RECOMMENDED_SUB_AUTHORITIES 1

typedef enum _SID_NAME_USE {
    SidTypeUser = 1,
    SidTypeGroup,
    SidTypeDomain,
    SidTypeAlias,
    SidTypeWellKnownGroup,
    SidTypeDeletedAccount,
    SidTypeInvalid,
    SidTypeUnknown,
    SidTypeComputer,
    SidTypeLabel
} SID_NAME_USE, *PSID_NAME_USE;

typedef struct _SID_AND_ATTRIBUTES {
  PSID Sid;
  ULONG Attributes;
} SID_AND_ATTRIBUTES, *PSID_AND_ATTRIBUTES;
typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[ANYSIZE_ARRAY];
typedef SID_AND_ATTRIBUTES_ARRAY *PSID_AND_ATTRIBUTES_ARRAY;

#define SID_HASH_SIZE 32
typedef ULONG_PTR SID_HASH_ENTRY, *PSID_HASH_ENTRY;

typedef struct _SID_AND_ATTRIBUTES_HASH {
  ULONG SidCount;
  PSID_AND_ATTRIBUTES SidAttr;
  SID_HASH_ENTRY Hash[SID_HASH_SIZE];
} SID_AND_ATTRIBUTES_HASH, *PSID_AND_ATTRIBUTES_HASH;

/* Universal well-known SIDs */

#define SECURITY_NULL_SID_AUTHORITY         {0,0,0,0,0,0}
#define SECURITY_WORLD_SID_AUTHORITY        {0,0,0,0,0,1}
#define SECURITY_LOCAL_SID_AUTHORITY        {0,0,0,0,0,2}
#define SECURITY_CREATOR_SID_AUTHORITY      {0,0,0,0,0,3}
#define SECURITY_NON_UNIQUE_AUTHORITY       {0,0,0,0,0,4}
#define SECURITY_RESOURCE_MANAGER_AUTHORITY {0,0,0,0,0,9}

#define SECURITY_NULL_RID                 (0x00000000L)
#define SECURITY_WORLD_RID                (0x00000000L)
#define SECURITY_LOCAL_RID                (0x00000000L)
#define SECURITY_LOCAL_LOGON_RID          (0x00000001L)

#define SECURITY_CREATOR_OWNER_RID        (0x00000000L)
#define SECURITY_CREATOR_GROUP_RID        (0x00000001L)
#define SECURITY_CREATOR_OWNER_SERVER_RID (0x00000002L)
#define SECURITY_CREATOR_GROUP_SERVER_RID (0x00000003L)
#define SECURITY_CREATOR_OWNER_RIGHTS_RID (0x00000004L)

/* NT well-known SIDs */

#define SECURITY_NT_AUTHORITY           {0,0,0,0,0,5}

#define SECURITY_DIALUP_RID             (0x00000001L)
#define SECURITY_NETWORK_RID            (0x00000002L)
#define SECURITY_BATCH_RID              (0x00000003L)
#define SECURITY_INTERACTIVE_RID        (0x00000004L)
#define SECURITY_LOGON_IDS_RID          (0x00000005L)
#define SECURITY_LOGON_IDS_RID_COUNT    (3L)
#define SECURITY_SERVICE_RID            (0x00000006L)
#define SECURITY_ANONYMOUS_LOGON_RID    (0x00000007L)
#define SECURITY_PROXY_RID              (0x00000008L)
#define SECURITY_ENTERPRISE_CONTROLLERS_RID (0x00000009L)
#define SECURITY_SERVER_LOGON_RID       SECURITY_ENTERPRISE_CONTROLLERS_RID
#define SECURITY_PRINCIPAL_SELF_RID     (0x0000000AL)
#define SECURITY_AUTHENTICATED_USER_RID (0x0000000BL)
#define SECURITY_RESTRICTED_CODE_RID    (0x0000000CL)
#define SECURITY_TERMINAL_SERVER_RID    (0x0000000DL)
#define SECURITY_REMOTE_LOGON_RID       (0x0000000EL)
#define SECURITY_THIS_ORGANIZATION_RID  (0x0000000FL)
#define SECURITY_IUSER_RID              (0x00000011L)
#define SECURITY_LOCAL_SYSTEM_RID       (0x00000012L)
#define SECURITY_LOCAL_SERVICE_RID      (0x00000013L)
#define SECURITY_NETWORK_SERVICE_RID    (0x00000014L)
#define SECURITY_NT_NON_UNIQUE          (0x00000015L)
#define SECURITY_NT_NON_UNIQUE_SUB_AUTH_COUNT  (3L)
#define SECURITY_ENTERPRISE_READONLY_CONTROLLERS_RID (0x00000016L)

#define SECURITY_BUILTIN_DOMAIN_RID     (0x00000020L)
#define SECURITY_WRITE_RESTRICTED_CODE_RID (0x00000021L)


#define SECURITY_PACKAGE_BASE_RID       (0x00000040L)
#define SECURITY_PACKAGE_RID_COUNT      (2L)
#define SECURITY_PACKAGE_NTLM_RID       (0x0000000AL)
#define SECURITY_PACKAGE_SCHANNEL_RID   (0x0000000EL)
#define SECURITY_PACKAGE_DIGEST_RID     (0x00000015L)

#define SECURITY_CRED_TYPE_BASE_RID             (0x00000041L)
#define SECURITY_CRED_TYPE_RID_COUNT            (2L)
#define SECURITY_CRED_TYPE_THIS_ORG_CERT_RID    (0x00000001L)

#define SECURITY_MIN_BASE_RID		(0x00000050L)
#define SECURITY_SERVICE_ID_BASE_RID    (0x00000050L)
#define SECURITY_SERVICE_ID_RID_COUNT   (6L)
#define SECURITY_RESERVED_ID_BASE_RID   (0x00000051L)
#define SECURITY_APPPOOL_ID_BASE_RID    (0x00000052L)
#define SECURITY_APPPOOL_ID_RID_COUNT   (6L)
#define SECURITY_VIRTUALSERVER_ID_BASE_RID    (0x00000053L)
#define SECURITY_VIRTUALSERVER_ID_RID_COUNT   (6L)
#define SECURITY_USERMODEDRIVERHOST_ID_BASE_RID  (0x00000054L)
#define SECURITY_USERMODEDRIVERHOST_ID_RID_COUNT (6L)
#define SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_BASE_RID  (0x00000055L)
#define SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_RID_COUNT (6L)
#define SECURITY_WMIHOST_ID_BASE_RID  (0x00000056L)
#define SECURITY_WMIHOST_ID_RID_COUNT (6L)
#define SECURITY_TASK_ID_BASE_RID                 (0x00000057L)
#define SECURITY_NFS_ID_BASE_RID        (0x00000058L)
#define SECURITY_COM_ID_BASE_RID        (0x00000059L)
#define SECURITY_VIRTUALACCOUNT_ID_RID_COUNT   (6L)

#define SECURITY_MAX_BASE_RID		(0x0000006FL)

#define SECURITY_MAX_ALWAYS_FILTERED    (0x000003E7L)
#define SECURITY_MIN_NEVER_FILTERED     (0x000003E8L)

#define SECURITY_OTHER_ORGANIZATION_RID (0x000003E8L)

#define SECURITY_WINDOWSMOBILE_ID_BASE_RID (0x00000070L)

/* Well-known domain relative sub-authority values (RIDs) */

#define DOMAIN_GROUP_RID_ENTERPRISE_READONLY_DOMAIN_CONTROLLERS (0x000001F2L)

#define FOREST_USER_RID_MAX            (0x000001F3L)

/* Well-known users */

#define DOMAIN_USER_RID_ADMIN          (0x000001F4L)
#define DOMAIN_USER_RID_GUEST          (0x000001F5L)
#define DOMAIN_USER_RID_KRBTGT         (0x000001F6L)

#define DOMAIN_USER_RID_MAX            (0x000003E7L)

/* Well-known groups */

#define DOMAIN_GROUP_RID_ADMINS        (0x00000200L)
#define DOMAIN_GROUP_RID_USERS         (0x00000201L)
#define DOMAIN_GROUP_RID_GUESTS        (0x00000202L)
#define DOMAIN_GROUP_RID_COMPUTERS     (0x00000203L)
#define DOMAIN_GROUP_RID_CONTROLLERS   (0x00000204L)
#define DOMAIN_GROUP_RID_CERT_ADMINS   (0x00000205L)
#define DOMAIN_GROUP_RID_SCHEMA_ADMINS (0x00000206L)
#define DOMAIN_GROUP_RID_ENTERPRISE_ADMINS (0x00000207L)
#define DOMAIN_GROUP_RID_POLICY_ADMINS (0x00000208L)
#define DOMAIN_GROUP_RID_READONLY_CONTROLLERS (0x00000209L)

/* Well-known aliases */

#define DOMAIN_ALIAS_RID_ADMINS                         (0x00000220L)
#define DOMAIN_ALIAS_RID_USERS                          (0x00000221L)
#define DOMAIN_ALIAS_RID_GUESTS                         (0x00000222L)
#define DOMAIN_ALIAS_RID_POWER_USERS                    (0x00000223L)

#define DOMAIN_ALIAS_RID_ACCOUNT_OPS                    (0x00000224L)
#define DOMAIN_ALIAS_RID_SYSTEM_OPS                     (0x00000225L)
#define DOMAIN_ALIAS_RID_PRINT_OPS                      (0x00000226L)
#define DOMAIN_ALIAS_RID_BACKUP_OPS                     (0x00000227L)

#define DOMAIN_ALIAS_RID_REPLICATOR                     (0x00000228L)
#define DOMAIN_ALIAS_RID_RAS_SERVERS                    (0x00000229L)
#define DOMAIN_ALIAS_RID_PREW2KCOMPACCESS               (0x0000022AL)
#define DOMAIN_ALIAS_RID_REMOTE_DESKTOP_USERS           (0x0000022BL)
#define DOMAIN_ALIAS_RID_NETWORK_CONFIGURATION_OPS      (0x0000022CL)
#define DOMAIN_ALIAS_RID_INCOMING_FOREST_TRUST_BUILDERS (0x0000022DL)

#define DOMAIN_ALIAS_RID_MONITORING_USERS               (0x0000022EL)
#define DOMAIN_ALIAS_RID_LOGGING_USERS                  (0x0000022FL)
#define DOMAIN_ALIAS_RID_AUTHORIZATIONACCESS            (0x00000230L)
#define DOMAIN_ALIAS_RID_TS_LICENSE_SERVERS             (0x00000231L)
#define DOMAIN_ALIAS_RID_DCOM_USERS                     (0x00000232L)
#define DOMAIN_ALIAS_RID_IUSERS                         (0x00000238L)
#define DOMAIN_ALIAS_RID_CRYPTO_OPERATORS               (0x00000239L)
#define DOMAIN_ALIAS_RID_CACHEABLE_PRINCIPALS_GROUP     (0x0000023BL)
#define DOMAIN_ALIAS_RID_NON_CACHEABLE_PRINCIPALS_GROUP (0x0000023CL)
#define DOMAIN_ALIAS_RID_EVENT_LOG_READERS_GROUP        (0x0000023DL)
#define DOMAIN_ALIAS_RID_CERTSVC_DCOM_ACCESS_GROUP      (0x0000023EL)

#define SECURITY_MANDATORY_LABEL_AUTHORITY          {0,0,0,0,0,16}
#define SECURITY_MANDATORY_UNTRUSTED_RID            (0x00000000L)
#define SECURITY_MANDATORY_LOW_RID                  (0x00001000L)
#define SECURITY_MANDATORY_MEDIUM_RID               (0x00002000L)
#define SECURITY_MANDATORY_HIGH_RID                 (0x00003000L)
#define SECURITY_MANDATORY_SYSTEM_RID               (0x00004000L)
#define SECURITY_MANDATORY_PROTECTED_PROCESS_RID    (0x00005000L)

/* SECURITY_MANDATORY_MAXIMUM_USER_RID is the highest RID that
   can be set by a usermode caller.*/

#define SECURITY_MANDATORY_MAXIMUM_USER_RID   SECURITY_MANDATORY_SYSTEM_RID

#define MANDATORY_LEVEL_TO_MANDATORY_RID(IL) (IL * 0x1000)

/* Allocate the System Luid.  The first 1000 LUIDs are reserved.
   Use #999 here (0x3e7 = 999) */

#define SYSTEM_LUID                     { 0x3e7, 0x0 }
#define ANONYMOUS_LOGON_LUID            { 0x3e6, 0x0 }
#define LOCALSERVICE_LUID               { 0x3e5, 0x0 }
#define NETWORKSERVICE_LUID             { 0x3e4, 0x0 }
#define IUSER_LUID                      { 0x3e3, 0x0 }

typedef struct _ACE_HEADER {
  UCHAR AceType;
  UCHAR AceFlags;
  USHORT AceSize;
} ACE_HEADER, *PACE_HEADER;

/* also in winnt.h */
#define ACCESS_MIN_MS_ACE_TYPE                  (0x0)
#define ACCESS_ALLOWED_ACE_TYPE                 (0x0)
#define ACCESS_DENIED_ACE_TYPE                  (0x1)
#define SYSTEM_AUDIT_ACE_TYPE                   (0x2)
#define SYSTEM_ALARM_ACE_TYPE                   (0x3)
#define ACCESS_MAX_MS_V2_ACE_TYPE               (0x3)
#define ACCESS_ALLOWED_COMPOUND_ACE_TYPE        (0x4)
#define ACCESS_MAX_MS_V3_ACE_TYPE               (0x4)
#define ACCESS_MIN_MS_OBJECT_ACE_TYPE           (0x5)
#define ACCESS_ALLOWED_OBJECT_ACE_TYPE          (0x5)
#define ACCESS_DENIED_OBJECT_ACE_TYPE           (0x6)
#define SYSTEM_AUDIT_OBJECT_ACE_TYPE            (0x7)
#define SYSTEM_ALARM_OBJECT_ACE_TYPE            (0x8)
#define ACCESS_MAX_MS_OBJECT_ACE_TYPE           (0x8)
#define ACCESS_MAX_MS_V4_ACE_TYPE               (0x8)
#define ACCESS_MAX_MS_ACE_TYPE                  (0x8)
#define ACCESS_ALLOWED_CALLBACK_ACE_TYPE        (0x9)
#define ACCESS_DENIED_CALLBACK_ACE_TYPE         (0xA)
#define ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE (0xB)
#define ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE  (0xC)
#define SYSTEM_AUDIT_CALLBACK_ACE_TYPE          (0xD)
#define SYSTEM_ALARM_CALLBACK_ACE_TYPE          (0xE)
#define SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE   (0xF)
#define SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE   (0x10)
#define ACCESS_MAX_MS_V5_ACE_TYPE               (0x11)
#define SYSTEM_MANDATORY_LABEL_ACE_TYPE         (0x11)

/* The following are the inherit flags that go into the AceFlags field
   of an Ace header. */

#define OBJECT_INHERIT_ACE                (0x1)
#define CONTAINER_INHERIT_ACE             (0x2)
#define NO_PROPAGATE_INHERIT_ACE          (0x4)
#define INHERIT_ONLY_ACE                  (0x8)
#define INHERITED_ACE                     (0x10)
#define VALID_INHERIT_FLAGS               (0x1F)

#define SUCCESSFUL_ACCESS_ACE_FLAG       (0x40)
#define FAILED_ACCESS_ACE_FLAG           (0x80)

typedef struct _ACCESS_ALLOWED_ACE {
  ACE_HEADER Header;
  ACCESS_MASK Mask;
  ULONG SidStart;
} ACCESS_ALLOWED_ACE, *PACCESS_ALLOWED_ACE;

typedef struct _ACCESS_DENIED_ACE {
  ACE_HEADER Header;
  ACCESS_MASK Mask;
  ULONG SidStart;
} ACCESS_DENIED_ACE, *PACCESS_DENIED_ACE;

typedef struct _SYSTEM_AUDIT_ACE {
  ACE_HEADER Header;
  ACCESS_MASK Mask;
  ULONG SidStart;
} SYSTEM_AUDIT_ACE, *PSYSTEM_AUDIT_ACE;

typedef struct _SYSTEM_ALARM_ACE {
  ACE_HEADER Header;
  ACCESS_MASK Mask;
  ULONG SidStart;
} SYSTEM_ALARM_ACE, *PSYSTEM_ALARM_ACE;

typedef struct _SYSTEM_MANDATORY_LABEL_ACE {
  ACE_HEADER Header;
  ACCESS_MASK Mask;
  ULONG SidStart;
} SYSTEM_MANDATORY_LABEL_ACE, *PSYSTEM_MANDATORY_LABEL_ACE;

#define SYSTEM_MANDATORY_LABEL_NO_WRITE_UP         0x1
#define SYSTEM_MANDATORY_LABEL_NO_READ_UP          0x2
#define SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP       0x4
#define SYSTEM_MANDATORY_LABEL_VALID_MASK (SYSTEM_MANDATORY_LABEL_NO_WRITE_UP   | \
                                           SYSTEM_MANDATORY_LABEL_NO_READ_UP    | \
                                           SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP)

#define SECURITY_DESCRIPTOR_MIN_LENGTH   (sizeof(SECURITY_DESCRIPTOR))

typedef USHORT SECURITY_DESCRIPTOR_CONTROL,*PSECURITY_DESCRIPTOR_CONTROL;

#define SE_OWNER_DEFAULTED              0x0001
#define SE_GROUP_DEFAULTED              0x0002
#define SE_DACL_PRESENT                 0x0004
#define SE_DACL_DEFAULTED               0x0008
#define SE_SACL_PRESENT                 0x0010
#define SE_SACL_DEFAULTED               0x0020
#define SE_DACL_UNTRUSTED               0x0040
#define SE_SERVER_SECURITY              0x0080
#define SE_DACL_AUTO_INHERIT_REQ        0x0100
#define SE_SACL_AUTO_INHERIT_REQ        0x0200
#define SE_DACL_AUTO_INHERITED          0x0400
#define SE_SACL_AUTO_INHERITED          0x0800
#define SE_DACL_PROTECTED               0x1000
#define SE_SACL_PROTECTED               0x2000
#define SE_RM_CONTROL_VALID             0x4000
#define SE_SELF_RELATIVE                0x8000

typedef struct _SECURITY_DESCRIPTOR_RELATIVE {
  UCHAR Revision;
  UCHAR Sbz1;
  SECURITY_DESCRIPTOR_CONTROL Control;
  ULONG Owner;
  ULONG Group;
  ULONG Sacl;
  ULONG Dacl;
} SECURITY_DESCRIPTOR_RELATIVE, *PISECURITY_DESCRIPTOR_RELATIVE;

typedef struct _SECURITY_DESCRIPTOR {
  UCHAR Revision;
  UCHAR Sbz1;
  SECURITY_DESCRIPTOR_CONTROL Control;
  PSID Owner;
  PSID Group;
  PACL Sacl;
  PACL Dacl;
} SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;

typedef struct _OBJECT_TYPE_LIST {
  USHORT Level;
  USHORT Sbz;
  GUID *ObjectType;
} OBJECT_TYPE_LIST, *POBJECT_TYPE_LIST;

#define ACCESS_OBJECT_GUID       0
#define ACCESS_PROPERTY_SET_GUID 1
#define ACCESS_PROPERTY_GUID     2
#define ACCESS_MAX_LEVEL         4

typedef enum _AUDIT_EVENT_TYPE {
  AuditEventObjectAccess,
  AuditEventDirectoryServiceAccess
} AUDIT_EVENT_TYPE, *PAUDIT_EVENT_TYPE;

#define AUDIT_ALLOW_NO_PRIVILEGE 0x1

#define ACCESS_DS_SOURCE_A "DS"
#define ACCESS_DS_SOURCE_W L"DS"
#define ACCESS_DS_OBJECT_TYPE_NAME_A "Directory Service Object"
#define ACCESS_DS_OBJECT_TYPE_NAME_W L"Directory Service Object"

#define ACCESS_REASON_TYPE_MASK 0xffff0000
#define ACCESS_REASON_DATA_MASK 0x0000ffff

typedef enum _ACCESS_REASON_TYPE {
  AccessReasonNone = 0x00000000,
  AccessReasonAllowedAce = 0x00010000,
  AccessReasonDeniedAce = 0x00020000,
  AccessReasonAllowedParentAce = 0x00030000,
  AccessReasonDeniedParentAce = 0x00040000,
  AccessReasonMissingPrivilege = 0x00100000,
  AccessReasonFromPrivilege = 0x00200000,
  AccessReasonIntegrityLevel = 0x00300000,
  AccessReasonOwnership = 0x00400000,
  AccessReasonNullDacl = 0x00500000,
  AccessReasonEmptyDacl = 0x00600000,
  AccessReasonNoSD = 0x00700000,
  AccessReasonNoGrant = 0x00800000
} ACCESS_REASON_TYPE;

typedef ULONG ACCESS_REASON;

typedef struct _ACCESS_REASONS {
  ACCESS_REASON Data[32];
} ACCESS_REASONS, *PACCESS_REASONS;

#define SE_SECURITY_DESCRIPTOR_FLAG_NO_OWNER_ACE    0x00000001
#define SE_SECURITY_DESCRIPTOR_FLAG_NO_LABEL_ACE    0x00000002
#define SE_SECURITY_DESCRIPTOR_VALID_FLAGS          0x00000003

typedef struct _SE_SECURITY_DESCRIPTOR {
  ULONG Size;
  ULONG Flags;
  PSECURITY_DESCRIPTOR SecurityDescriptor;
} SE_SECURITY_DESCRIPTOR, *PSE_SECURITY_DESCRIPTOR;

typedef struct _SE_ACCESS_REQUEST {
  ULONG Size;
  PSE_SECURITY_DESCRIPTOR SeSecurityDescriptor;
  ACCESS_MASK DesiredAccess;
  ACCESS_MASK PreviouslyGrantedAccess;
  PSID PrincipalSelfSid;
  PGENERIC_MAPPING GenericMapping;
  ULONG ObjectTypeListCount;
  POBJECT_TYPE_LIST ObjectTypeList;
} SE_ACCESS_REQUEST, *PSE_ACCESS_REQUEST;

typedef struct _SE_ACCESS_REPLY {
  ULONG Size;
  ULONG ResultListCount;
  PACCESS_MASK GrantedAccess;
  PNTSTATUS AccessStatus;
  PACCESS_REASONS AccessReason;
  PPRIVILEGE_SET* Privileges;
} SE_ACCESS_REPLY, *PSE_ACCESS_REPLY;

typedef enum _SE_AUDIT_OPERATION {
  AuditPrivilegeObject,
  AuditPrivilegeService,
  AuditAccessCheck,
  AuditOpenObject,
  AuditOpenObjectWithTransaction,
  AuditCloseObject,
  AuditDeleteObject,
  AuditOpenObjectForDelete,
  AuditOpenObjectForDeleteWithTransaction,
  AuditCloseNonObject,
  AuditOpenNonObject,
  AuditObjectReference,
  AuditHandleCreation,
} SE_AUDIT_OPERATION, *PSE_AUDIT_OPERATION;

typedef struct _SE_AUDIT_INFO {
  ULONG Size;
  AUDIT_EVENT_TYPE AuditType;
  SE_AUDIT_OPERATION AuditOperation;
  ULONG AuditFlags;
  UNICODE_STRING SubsystemName;
  UNICODE_STRING ObjectTypeName;
  UNICODE_STRING ObjectName;
  PVOID HandleId;
  GUID* TransactionId;
  LUID* OperationId;
  BOOLEAN ObjectCreation;
  BOOLEAN GenerateOnClose;
} SE_AUDIT_INFO, *PSE_AUDIT_INFO;

#define TOKEN_ASSIGN_PRIMARY            (0x0001)
#define TOKEN_DUPLICATE                 (0x0002)
#define TOKEN_IMPERSONATE               (0x0004)
#define TOKEN_QUERY                     (0x0008)
#define TOKEN_QUERY_SOURCE              (0x0010)
#define TOKEN_ADJUST_PRIVILEGES         (0x0020)
#define TOKEN_ADJUST_GROUPS             (0x0040)
#define TOKEN_ADJUST_DEFAULT            (0x0080)
#define TOKEN_ADJUST_SESSIONID          (0x0100)

#define TOKEN_ALL_ACCESS_P (STANDARD_RIGHTS_REQUIRED  |\
                            TOKEN_ASSIGN_PRIMARY      |\
                            TOKEN_DUPLICATE           |\
                            TOKEN_IMPERSONATE         |\
                            TOKEN_QUERY               |\
                            TOKEN_QUERY_SOURCE        |\
                            TOKEN_ADJUST_PRIVILEGES   |\
                            TOKEN_ADJUST_GROUPS       |\
                            TOKEN_ADJUST_DEFAULT )

#if ((defined(_WIN32_WINNT) && (_WIN32_WINNT > 0x0400)) || (!defined(_WIN32_WINNT)))
#define TOKEN_ALL_ACCESS  (TOKEN_ALL_ACCESS_P |\
                           TOKEN_ADJUST_SESSIONID )
#else
#define TOKEN_ALL_ACCESS  (TOKEN_ALL_ACCESS_P)
#endif

#define TOKEN_READ       (STANDARD_RIGHTS_READ     |\
                          TOKEN_QUERY)

#define TOKEN_WRITE      (STANDARD_RIGHTS_WRITE    |\
                          TOKEN_ADJUST_PRIVILEGES  |\
                          TOKEN_ADJUST_GROUPS      |\
                          TOKEN_ADJUST_DEFAULT)

#define TOKEN_EXECUTE    (STANDARD_RIGHTS_EXECUTE)

typedef enum _TOKEN_TYPE {
  TokenPrimary = 1,
  TokenImpersonation
} TOKEN_TYPE,*PTOKEN_TYPE;

typedef enum _TOKEN_INFORMATION_CLASS {
  TokenUser = 1,
  TokenGroups,
  TokenPrivileges,
  TokenOwner,
  TokenPrimaryGroup,
  TokenDefaultDacl,
  TokenSource,
  TokenType,
  TokenImpersonationLevel,
  TokenStatistics,
  TokenRestrictedSids,
  TokenSessionId,
  TokenGroupsAndPrivileges,
  TokenSessionReference,
  TokenSandBoxInert,
  TokenAuditPolicy,
  TokenOrigin,
  TokenElevationType,
  TokenLinkedToken,
  TokenElevation,
  TokenHasRestrictions,
  TokenAccessInformation,
  TokenVirtualizationAllowed,
  TokenVirtualizationEnabled,
  TokenIntegrityLevel,
  TokenUIAccess,
  TokenMandatoryPolicy,
  TokenLogonSid,
  MaxTokenInfoClass
} TOKEN_INFORMATION_CLASS, *PTOKEN_INFORMATION_CLASS;

typedef struct _TOKEN_USER {
  SID_AND_ATTRIBUTES User;
} TOKEN_USER, *PTOKEN_USER;

typedef struct _TOKEN_GROUPS {
  ULONG GroupCount;
  SID_AND_ATTRIBUTES Groups[ANYSIZE_ARRAY];
} TOKEN_GROUPS,*PTOKEN_GROUPS,*LPTOKEN_GROUPS;

typedef struct _TOKEN_PRIVILEGES {
  ULONG PrivilegeCount;
  LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES,*PTOKEN_PRIVILEGES,*LPTOKEN_PRIVILEGES;

typedef struct _TOKEN_OWNER {
  PSID Owner;
} TOKEN_OWNER,*PTOKEN_OWNER;

typedef struct _TOKEN_PRIMARY_GROUP {
  PSID PrimaryGroup;
} TOKEN_PRIMARY_GROUP,*PTOKEN_PRIMARY_GROUP;

typedef struct _TOKEN_DEFAULT_DACL {
  PACL DefaultDacl;
} TOKEN_DEFAULT_DACL,*PTOKEN_DEFAULT_DACL;

typedef struct _TOKEN_GROUPS_AND_PRIVILEGES {
  ULONG SidCount;
  ULONG SidLength;
  PSID_AND_ATTRIBUTES Sids;
  ULONG RestrictedSidCount;
  ULONG RestrictedSidLength;
  PSID_AND_ATTRIBUTES RestrictedSids;
  ULONG PrivilegeCount;
  ULONG PrivilegeLength;
  PLUID_AND_ATTRIBUTES Privileges;
  LUID AuthenticationId;
} TOKEN_GROUPS_AND_PRIVILEGES, *PTOKEN_GROUPS_AND_PRIVILEGES;

typedef struct _TOKEN_LINKED_TOKEN {
  HANDLE LinkedToken;
} TOKEN_LINKED_TOKEN, *PTOKEN_LINKED_TOKEN;

typedef struct _TOKEN_ELEVATION {
  ULONG TokenIsElevated;
} TOKEN_ELEVATION, *PTOKEN_ELEVATION;

typedef struct _TOKEN_MANDATORY_LABEL {
  SID_AND_ATTRIBUTES Label;
} TOKEN_MANDATORY_LABEL, *PTOKEN_MANDATORY_LABEL;

#define TOKEN_MANDATORY_POLICY_OFF             0x0
#define TOKEN_MANDATORY_POLICY_NO_WRITE_UP     0x1
#define TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN 0x2

#define TOKEN_MANDATORY_POLICY_VALID_MASK    (TOKEN_MANDATORY_POLICY_NO_WRITE_UP | \
                                              TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN)

typedef struct _TOKEN_MANDATORY_POLICY {
  ULONG Policy;
} TOKEN_MANDATORY_POLICY, *PTOKEN_MANDATORY_POLICY;

typedef struct _TOKEN_ACCESS_INFORMATION {
  PSID_AND_ATTRIBUTES_HASH SidHash;
  PSID_AND_ATTRIBUTES_HASH RestrictedSidHash;
  PTOKEN_PRIVILEGES Privileges;
  LUID AuthenticationId;
  TOKEN_TYPE TokenType;
  SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
  TOKEN_MANDATORY_POLICY MandatoryPolicy;
  ULONG Flags;
} TOKEN_ACCESS_INFORMATION, *PTOKEN_ACCESS_INFORMATION;

#define POLICY_AUDIT_SUBCATEGORY_COUNT (53)

typedef struct _TOKEN_AUDIT_POLICY {
  UCHAR PerUserPolicy[((POLICY_AUDIT_SUBCATEGORY_COUNT) >> 1) + 1];
} TOKEN_AUDIT_POLICY, *PTOKEN_AUDIT_POLICY;

#define TOKEN_SOURCE_LENGTH 8

typedef struct _TOKEN_SOURCE {
  CHAR SourceName[TOKEN_SOURCE_LENGTH];
  LUID SourceIdentifier;
} TOKEN_SOURCE,*PTOKEN_SOURCE;

typedef struct _TOKEN_STATISTICS {
  LUID TokenId;
  LUID AuthenticationId;
  LARGE_INTEGER ExpirationTime;
  TOKEN_TYPE TokenType;
  SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
  ULONG DynamicCharged;
  ULONG DynamicAvailable;
  ULONG GroupCount;
  ULONG PrivilegeCount;
  LUID ModifiedId;
} TOKEN_STATISTICS, *PTOKEN_STATISTICS;

typedef struct _TOKEN_CONTROL {
  LUID TokenId;
  LUID AuthenticationId;
  LUID ModifiedId;
  TOKEN_SOURCE TokenSource;
} TOKEN_CONTROL,*PTOKEN_CONTROL;

typedef struct _TOKEN_ORIGIN {
  LUID OriginatingLogonSession;
} TOKEN_ORIGIN, *PTOKEN_ORIGIN;

typedef enum _MANDATORY_LEVEL {
  MandatoryLevelUntrusted = 0,
  MandatoryLevelLow,
  MandatoryLevelMedium,
  MandatoryLevelHigh,
  MandatoryLevelSystem,
  MandatoryLevelSecureProcess,
  MandatoryLevelCount
} MANDATORY_LEVEL, *PMANDATORY_LEVEL;

#if (NTDDI_VERSION >= NTDDI_WIN2K)

NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenThreadToken(
  IN HANDLE ThreadHandle,
  IN ACCESS_MASK DesiredAccess,
  IN BOOLEAN OpenAsSelf,
  OUT PHANDLE TokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenProcessToken(
  IN HANDLE ProcessHandle,
  IN ACCESS_MASK DesiredAccess,
  OUT PHANDLE TokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationToken(
  IN HANDLE TokenHandle,
  IN TOKEN_INFORMATION_CLASS TokenInformationClass,
  OUT PVOID TokenInformation OPTIONAL,
  IN ULONG TokenInformationLength,
  OUT PULONG ReturnLength);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAdjustPrivilegesToken(
  IN HANDLE TokenHandle,
  IN BOOLEAN DisableAllPrivileges,
  IN PTOKEN_PRIVILEGES NewState OPTIONAL,
  IN ULONG BufferLength,
  OUT PTOKEN_PRIVILEGES PreviousState,
  OUT PULONG ReturnLength OPTIONAL);

#endif

#if (NTDDI_VERSION >= NTDDI_WINXP)

NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenThreadTokenEx(
  IN HANDLE ThreadHandle,
  IN ACCESS_MASK DesiredAccess,
  IN BOOLEAN OpenAsSelf,
  IN ULONG HandleAttributes,
  OUT PHANDLE TokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenProcessTokenEx(
  IN HANDLE ProcessHandle,
  IN ACCESS_MASK DesiredAccess,
  IN ULONG HandleAttributes,
  OUT PHANDLE TokenHandle);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenJobObjectToken(
  IN HANDLE JobHandle,
  IN ACCESS_MASK DesiredAccess,
  OUT PHANDLE TokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtDuplicateToken(
  IN HANDLE ExistingTokenHandle,
  IN ACCESS_MASK DesiredAccess,
  IN POBJECT_ATTRIBUTES ObjectAttributes,
  IN BOOLEAN EffectiveOnly,
  IN TOKEN_TYPE TokenType,
  OUT PHANDLE NewTokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtFilterToken(
  IN HANDLE ExistingTokenHandle,
  IN ULONG Flags,
  IN PTOKEN_GROUPS SidsToDisable OPTIONAL,
  IN PTOKEN_PRIVILEGES PrivilegesToDelete OPTIONAL,
  IN PTOKEN_GROUPS RestrictedSids OPTIONAL,
  OUT PHANDLE NewTokenHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtImpersonateAnonymousToken(
  IN HANDLE ThreadHandle);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetInformationToken(
  IN HANDLE TokenHandle,
  IN TOKEN_INFORMATION_CLASS TokenInformationClass,
  IN PVOID TokenInformation,
  IN ULONG TokenInformationLength);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAdjustGroupsToken(
  IN HANDLE TokenHandle,
  IN BOOLEAN ResetToDefault,
  IN PTOKEN_GROUPS NewState OPTIONAL,
  IN ULONG BufferLength OPTIONAL,
  OUT PTOKEN_GROUPS PreviousState,
  OUT PULONG ReturnLength);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtPrivilegeCheck(
  IN HANDLE ClientToken,
  IN OUT PPRIVILEGE_SET RequiredPrivileges,
  OUT PBOOLEAN Result);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAccessCheckAndAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN PUNICODE_STRING ObjectTypeName,
  IN PUNICODE_STRING ObjectName,
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN ACCESS_MASK DesiredAccess,
  IN PGENERIC_MAPPING GenericMapping,
  IN BOOLEAN ObjectCreation,
  OUT PACCESS_MASK GrantedAccess,
  OUT PNTSTATUS AccessStatus,
  OUT PBOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAccessCheckByTypeAndAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId,
  IN PUNICODE_STRING ObjectTypeName,
  IN PUNICODE_STRING ObjectName,
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID PrincipalSelfSid OPTIONAL,
  IN ACCESS_MASK DesiredAccess,
  IN AUDIT_EVENT_TYPE AuditType,
  IN ULONG Flags,
  IN POBJECT_TYPE_LIST ObjectTypeList OPTIONAL,
  IN ULONG ObjectTypeLength,
  IN PGENERIC_MAPPING GenericMapping,
  IN BOOLEAN ObjectCreation,
  OUT PACCESS_MASK GrantedAccess,
  OUT PNTSTATUS AccessStatus,
  OUT PBOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAccessCheckByTypeResultListAndAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN PUNICODE_STRING ObjectTypeName,
  IN PUNICODE_STRING ObjectName,
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID PrincipalSelfSid OPTIONAL,
  IN ACCESS_MASK DesiredAccess,
  IN AUDIT_EVENT_TYPE AuditType,
  IN ULONG Flags,
  IN POBJECT_TYPE_LIST ObjectTypeList OPTIONAL,
  IN ULONG ObjectTypeLength,
  IN PGENERIC_MAPPING GenericMapping,
  IN BOOLEAN ObjectCreation,
  OUT PACCESS_MASK GrantedAccess,
  OUT PNTSTATUS AccessStatus,
  OUT PBOOLEAN GenerateOnClose);

NTSTATUS
NTAPI
NtAccessCheckByTypeResultListAndAuditAlarmByHandle(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN HANDLE ClientToken,
  IN PUNICODE_STRING ObjectTypeName,
  IN PUNICODE_STRING ObjectName,
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID PrincipalSelfSid OPTIONAL,
  IN ACCESS_MASK DesiredAccess,
  IN AUDIT_EVENT_TYPE AuditType,
  IN ULONG Flags,
  IN POBJECT_TYPE_LIST ObjectTypeList OPTIONAL,
  IN ULONG ObjectTypeLength,
  IN PGENERIC_MAPPING GenericMapping,
  IN BOOLEAN ObjectCreation,
  OUT PACCESS_MASK GrantedAccess,
  OUT PNTSTATUS AccessStatus,
  OUT PBOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtOpenObjectAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN PUNICODE_STRING ObjectTypeName,
  IN PUNICODE_STRING ObjectName,
  IN PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL,
  IN HANDLE ClientToken,
  IN ACCESS_MASK DesiredAccess,
  IN ACCESS_MASK GrantedAccess,
  IN PPRIVILEGE_SET Privileges OPTIONAL,
  IN BOOLEAN ObjectCreation,
  IN BOOLEAN AccessGranted,
  OUT PBOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtPrivilegeObjectAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN HANDLE ClientToken,
  IN ACCESS_MASK DesiredAccess,
  IN PPRIVILEGE_SET Privileges,
  IN BOOLEAN AccessGranted);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtCloseObjectAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN BOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtDeleteObjectAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PVOID HandleId OPTIONAL,
  IN BOOLEAN GenerateOnClose);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtPrivilegedServiceAuditAlarm(
  IN PUNICODE_STRING SubsystemName,
  IN PUNICODE_STRING ServiceName,
  IN HANDLE ClientToken,
  IN PPRIVILEGE_SET Privileges,
  IN BOOLEAN AccessGranted);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetInformationThread(
  IN HANDLE ThreadHandle,
  IN THREADINFOCLASS ThreadInformationClass,
  IN PVOID ThreadInformation,
  IN ULONG ThreadInformationLength);

#endif

typedef NTSTATUS
(NTAPI * PRTL_HEAP_COMMIT_ROUTINE) (
  IN PVOID Base,
  IN OUT PVOID *CommitAddress,
  IN OUT PSIZE_T CommitSize);

typedef struct _RTL_HEAP_PARAMETERS {
  ULONG Length;
  SIZE_T SegmentReserve;
  SIZE_T SegmentCommit;
  SIZE_T DeCommitFreeBlockThreshold;
  SIZE_T DeCommitTotalFreeThreshold;
  SIZE_T MaximumAllocationSize;
  SIZE_T VirtualMemoryThreshold;
  SIZE_T InitialCommit;
  SIZE_T InitialReserve;
  PRTL_HEAP_COMMIT_ROUTINE CommitRoutine;
  SIZE_T Reserved[2];
} RTL_HEAP_PARAMETERS, *PRTL_HEAP_PARAMETERS;

#if (NTDDI_VERSION >= NTDDI_WIN2K)

NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap(
  IN HANDLE HeapHandle,
  IN ULONG Flags OPTIONAL,
  IN SIZE_T Size);

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHeap(
  IN PVOID HeapHandle,
  IN ULONG Flags OPTIONAL,
  IN PVOID BaseAddress);

NTSYSAPI
VOID
NTAPI
RtlCaptureContext(
  OUT PCONTEXT ContextRecord);

NTSYSAPI
ULONG
NTAPI
RtlRandom(
  IN OUT PULONG Seed);

NTSYSAPI
BOOLEAN
NTAPI
RtlCreateUnicodeString(
  OUT PUNICODE_STRING DestinationString,
  IN PCWSTR SourceString);

NTSYSAPI
NTSTATUS
NTAPI
RtlAppendStringToString(
  IN OUT PSTRING Destination,
  IN const STRING *Source);

NTSYSAPI
NTSTATUS
NTAPI
RtlOemStringToUnicodeString(
  IN OUT PUNICODE_STRING DestinationString,
  IN PCOEM_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToOemString(
  IN OUT POEM_STRING DestinationString,
  IN PCUNICODE_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeStringToOemString(
  IN OUT POEM_STRING DestinationString,
  IN PCUNICODE_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlOemStringToCountedUnicodeString(
  IN OUT PUNICODE_STRING DestinationString,
  IN PCOEM_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToCountedOemString(
  IN OUT POEM_STRING DestinationString,
  IN PCUNICODE_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeStringToCountedOemString(
  IN OUT POEM_STRING DestinationString,
  IN PCUNICODE_STRING SourceString,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlDowncaseUnicodeString(
  IN OUT PUNICODE_STRING UniDest,
  IN PCUNICODE_STRING UniSource,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
VOID
NTAPI
RtlFreeOemString (
  IN OUT POEM_STRING OemString);

NTSYSAPI
ULONG
NTAPI
RtlxUnicodeStringToOemSize(
  IN PCUNICODE_STRING UnicodeString);

NTSYSAPI
ULONG
NTAPI
RtlxOemStringToUnicodeSize(
  IN PCOEM_STRING OemString);

NTSYSAPI
NTSTATUS
NTAPI
RtlMultiByteToUnicodeN(
  OUT PWCH UnicodeString,
  IN ULONG MaxBytesInUnicodeString,
  OUT PULONG BytesInUnicodeString OPTIONAL,
  IN const CHAR *MultiByteString,
  IN ULONG BytesInMultiByteString);

NTSYSAPI
NTSTATUS
NTAPI
RtlMultiByteToUnicodeSize(
  OUT PULONG BytesInUnicodeString,
  IN const CHAR *MultiByteString,
  IN ULONG BytesInMultiByteString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToMultiByteSize(
  OUT PULONG BytesInMultiByteString,
  IN PCWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToMultiByteN(
  OUT PCHAR MultiByteString,
  IN ULONG MaxBytesInMultiByteString,
  OUT PULONG BytesInMultiByteString OPTIONAL,
  IN PWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeToMultiByteN(
  OUT PCHAR MultiByteString,
  IN ULONG MaxBytesInMultiByteString,
  OUT PULONG BytesInMultiByteString OPTIONAL,
  IN PCWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
NTSTATUS
NTAPI
RtlOemToUnicodeN(
  OUT PWSTR UnicodeString,
  IN ULONG MaxBytesInUnicodeString,
  OUT PULONG BytesInUnicodeString OPTIONAL,
  IN PCCH OemString,
  IN ULONG BytesInOemString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToOemN(
  OUT PCHAR OemString,
  IN ULONG MaxBytesInOemString,
  OUT PULONG BytesInOemString OPTIONAL,
  IN PCWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeToOemN(
  OUT PCHAR OemString,
  IN ULONG MaxBytesInOemString,
  OUT PULONG BytesInOemString OPTIONAL,
  IN PCWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

#if (NTDDI_VERSION >= NTDDI_VISTASP1)
NTSYSAPI
NTSTATUS
NTAPI
RtlGenerate8dot3Name(
  IN PCUNICODE_STRING Name,
  IN BOOLEAN AllowExtendedCharacters,
  IN OUT PGENERATE_NAME_CONTEXT Context,
  IN OUT PUNICODE_STRING Name8dot3);
#else
NTSYSAPI
VOID
NTAPI
RtlGenerate8dot3Name(
  IN PCUNICODE_STRING Name,
  IN BOOLEAN AllowExtendedCharacters,
  IN OUT PGENERATE_NAME_CONTEXT Context,
  IN OUT PUNICODE_STRING Name8dot3);
#endif

NTSYSAPI
BOOLEAN
NTAPI
RtlIsNameLegalDOS8Dot3(
  IN PCUNICODE_STRING Name,
  IN OUT POEM_STRING OemName OPTIONAL,
  IN OUT PBOOLEAN NameContainsSpaces OPTIONAL);

NTSYSAPI
BOOLEAN
NTAPI
RtlIsValidOemCharacter(
  IN OUT PWCHAR Char);

NTSYSAPI
VOID
NTAPI
PfxInitialize(
  OUT PPREFIX_TABLE PrefixTable);

NTSYSAPI
BOOLEAN
NTAPI
PfxInsertPrefix(
  IN PPREFIX_TABLE PrefixTable,
  IN PSTRING Prefix,
  OUT PPREFIX_TABLE_ENTRY PrefixTableEntry);

NTSYSAPI
VOID
NTAPI
PfxRemovePrefix(
  IN PPREFIX_TABLE PrefixTable,
  IN PPREFIX_TABLE_ENTRY PrefixTableEntry);

NTSYSAPI
PPREFIX_TABLE_ENTRY
NTAPI
PfxFindPrefix(
  IN PPREFIX_TABLE PrefixTable,
  IN PSTRING FullName);

NTSYSAPI
VOID
NTAPI
RtlInitializeUnicodePrefix(
  OUT PUNICODE_PREFIX_TABLE PrefixTable);

NTSYSAPI
BOOLEAN
NTAPI
RtlInsertUnicodePrefix(
  IN PUNICODE_PREFIX_TABLE PrefixTable,
  IN PUNICODE_STRING Prefix,
  OUT PUNICODE_PREFIX_TABLE_ENTRY PrefixTableEntry);

NTSYSAPI
VOID
NTAPI
RtlRemoveUnicodePrefix(
  IN PUNICODE_PREFIX_TABLE PrefixTable,
  IN PUNICODE_PREFIX_TABLE_ENTRY PrefixTableEntry);

NTSYSAPI
PUNICODE_PREFIX_TABLE_ENTRY
NTAPI
RtlFindUnicodePrefix(
  IN PUNICODE_PREFIX_TABLE PrefixTable,
  IN PUNICODE_STRING FullName,
  IN ULONG CaseInsensitiveIndex);

NTSYSAPI
PUNICODE_PREFIX_TABLE_ENTRY
NTAPI
RtlNextUnicodePrefix(
  IN PUNICODE_PREFIX_TABLE PrefixTable,
  IN BOOLEAN Restart);

NTSYSAPI
SIZE_T
NTAPI
RtlCompareMemoryUlong(
  IN PVOID Source,
  IN SIZE_T Length,
  IN ULONG Pattern);

NTSYSAPI
BOOLEAN
NTAPI
RtlTimeToSecondsSince1980(
  IN PLARGE_INTEGER Time,
  OUT PULONG ElapsedSeconds);

NTSYSAPI
VOID
NTAPI
RtlSecondsSince1980ToTime(
  IN ULONG ElapsedSeconds,
  OUT PLARGE_INTEGER Time);

NTSYSAPI
BOOLEAN
NTAPI
RtlTimeToSecondsSince1970(
  IN PLARGE_INTEGER Time,
  OUT PULONG ElapsedSeconds);

NTSYSAPI
VOID
NTAPI
RtlSecondsSince1970ToTime(
  IN ULONG ElapsedSeconds,
  OUT PLARGE_INTEGER Time);

NTSYSAPI
BOOLEAN
NTAPI
RtlValidSid(
  IN PSID Sid);

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualSid(
  IN PSID Sid1,
  IN PSID Sid2);

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualPrefixSid(
  IN PSID Sid1,
  IN PSID Sid2);

NTSYSAPI
ULONG
NTAPI
RtlLengthRequiredSid(
  IN ULONG SubAuthorityCount);

NTSYSAPI
PVOID
NTAPI
RtlFreeSid(
  IN PSID Sid);

NTSYSAPI
NTSTATUS
NTAPI
RtlAllocateAndInitializeSid(
  IN PSID_IDENTIFIER_AUTHORITY IdentifierAuthority,
  IN UCHAR SubAuthorityCount,
  IN ULONG SubAuthority0,
  IN ULONG SubAuthority1,
  IN ULONG SubAuthority2,
  IN ULONG SubAuthority3,
  IN ULONG SubAuthority4,
  IN ULONG SubAuthority5,
  IN ULONG SubAuthority6,
  IN ULONG SubAuthority7,
  OUT PSID *Sid);

NTSYSAPI
NTSTATUS
NTAPI
RtlInitializeSid(
  OUT PSID Sid,
  IN PSID_IDENTIFIER_AUTHORITY IdentifierAuthority,
  IN UCHAR SubAuthorityCount);

NTSYSAPI
PULONG
NTAPI
RtlSubAuthoritySid(
  IN PSID Sid,
  IN ULONG SubAuthority);

NTSYSAPI
ULONG
NTAPI
RtlLengthSid(
  IN PSID Sid);

NTSYSAPI
NTSTATUS
NTAPI
RtlCopySid(
  IN ULONG Length,
  IN PSID Destination,
  IN PSID Source);

NTSYSAPI
NTSTATUS
NTAPI
RtlConvertSidToUnicodeString(
  IN OUT PUNICODE_STRING UnicodeString,
  IN PSID Sid,
  IN BOOLEAN AllocateDestinationString);

NTSYSAPI
VOID
NTAPI
RtlCopyLuid(
  OUT PLUID DestinationLuid,
  IN PLUID SourceLuid);

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateAcl(
  OUT PACL Acl,
  IN ULONG AclLength,
  IN ULONG AclRevision);

NTSYSAPI
NTSTATUS
NTAPI
RtlAddAce(
  IN OUT PACL Acl,
  IN ULONG AceRevision,
  IN ULONG StartingAceIndex,
  IN PVOID AceList,
  IN ULONG AceListLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlDeleteAce(
  IN OUT PACL Acl,
  IN ULONG AceIndex);

NTSYSAPI
NTSTATUS
NTAPI
RtlGetAce(
  IN PACL Acl,
  IN ULONG AceIndex,
  OUT PVOID *Ace);

NTSYSAPI
NTSTATUS
NTAPI
RtlAddAccessAllowedAce(
  IN OUT PACL Acl,
  IN ULONG AceRevision,
  IN ACCESS_MASK AccessMask,
  IN PSID Sid);

NTSYSAPI
NTSTATUS
NTAPI
RtlAddAccessAllowedAceEx(
  IN OUT PACL Acl,
  IN ULONG AceRevision,
  IN ULONG AceFlags,
  IN ACCESS_MASK AccessMask,
  IN PSID Sid);

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateSecurityDescriptorRelative(
  OUT PISECURITY_DESCRIPTOR_RELATIVE SecurityDescriptor,
  IN ULONG Revision);

NTSYSAPI
NTSTATUS
NTAPI
RtlGetDaclSecurityDescriptor(
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  OUT PBOOLEAN DaclPresent,
  OUT PACL *Dacl,
  OUT PBOOLEAN DaclDefaulted);

NTSYSAPI
NTSTATUS
NTAPI
RtlSetOwnerSecurityDescriptor(
  IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID Owner OPTIONAL,
  IN BOOLEAN OwnerDefaulted);

NTSYSAPI
NTSTATUS
NTAPI
RtlGetOwnerSecurityDescriptor(
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  OUT PSID *Owner,
  OUT PBOOLEAN OwnerDefaulted);

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosError(
  IN NTSTATUS Status);

NTSYSAPI
NTSTATUS
NTAPI
RtlCustomCPToUnicodeN(
  IN PCPTABLEINFO CustomCP,
  OUT PWCH UnicodeString,
  IN ULONG MaxBytesInUnicodeString,
  OUT PULONG BytesInUnicodeString OPTIONAL,
  IN PCH CustomCPString,
  IN ULONG BytesInCustomCPString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToCustomCPN(
  IN PCPTABLEINFO CustomCP,
  OUT PCH CustomCPString,
  IN ULONG MaxBytesInCustomCPString,
  OUT PULONG BytesInCustomCPString OPTIONAL,
  IN PWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeToCustomCPN(
  IN PCPTABLEINFO CustomCP,
  OUT PCH CustomCPString,
  IN ULONG MaxBytesInCustomCPString,
  OUT PULONG BytesInCustomCPString OPTIONAL,
  IN PWCH UnicodeString,
  IN ULONG BytesInUnicodeString);

NTSYSAPI
VOID
NTAPI
RtlInitCodePageTable(
  IN PUSHORT TableBase,
  IN OUT PCPTABLEINFO CodePageTable);

#endif

#if (NTDDI_VERSION >= NTDDI_WINXP)

NTSYSAPI
PVOID
NTAPI
RtlCreateHeap(
  IN ULONG Flags,
  IN PVOID HeapBase OPTIONAL,
  IN SIZE_T ReserveSize OPTIONAL,
  IN SIZE_T CommitSize OPTIONAL,
  IN PVOID Lock OPTIONAL,
  IN PRTL_HEAP_PARAMETERS Parameters OPTIONAL);

NTSYSAPI
PVOID
NTAPI
RtlDestroyHeap(
  IN PVOID HeapHandle);

NTSYSAPI
USHORT
NTAPI
RtlCaptureStackBackTrace(
  IN ULONG FramesToSkip,
  IN ULONG FramesToCapture,
  OUT PVOID *BackTrace,
  OUT PULONG BackTraceHash OPTIONAL);

NTSYSAPI
ULONG
NTAPI
RtlRandomEx(
  IN OUT PULONG Seed);

NTSYSAPI
NTSTATUS
NTAPI
RtlInitUnicodeStringEx(
  OUT PUNICODE_STRING DestinationString,
  IN PCWSTR SourceString OPTIONAL);

NTSYSAPI
NTSTATUS
NTAPI
RtlValidateUnicodeString(
  IN ULONG Flags,
  IN PCUNICODE_STRING String);

NTSYSAPI
NTSTATUS
NTAPI
RtlDuplicateUnicodeString(
  IN ULONG Flags,
  IN PCUNICODE_STRING SourceString,
  OUT PUNICODE_STRING DestinationString);

NTSYSAPI
NTSTATUS
NTAPI
RtlGetCompressionWorkSpaceSize(
  IN USHORT CompressionFormatAndEngine,
  OUT PULONG CompressBufferWorkSpaceSize,
  OUT PULONG CompressFragmentWorkSpaceSize);

NTSYSAPI
NTSTATUS
NTAPI
RtlCompressBuffer(
  IN USHORT CompressionFormatAndEngine,
  IN PUCHAR UncompressedBuffer,
  IN ULONG UncompressedBufferSize,
  OUT PUCHAR CompressedBuffer,
  IN ULONG CompressedBufferSize,
  IN ULONG UncompressedChunkSize,
  OUT PULONG FinalCompressedSize,
  IN PVOID WorkSpace);

NTSYSAPI
NTSTATUS
NTAPI
RtlDecompressBuffer(
  IN USHORT CompressionFormat,
  OUT PUCHAR UncompressedBuffer,
  IN ULONG UncompressedBufferSize,
  IN PUCHAR CompressedBuffer,
  IN ULONG CompressedBufferSize,
  OUT PULONG FinalUncompressedSize);

NTSYSAPI
NTSTATUS
NTAPI
RtlDecompressFragment(
  IN USHORT CompressionFormat,
  OUT PUCHAR UncompressedFragment,
  IN ULONG UncompressedFragmentSize,
  IN PUCHAR CompressedBuffer,
  IN ULONG CompressedBufferSize,
  IN ULONG FragmentOffset,
  OUT PULONG FinalUncompressedSize,
  IN PVOID WorkSpace);

NTSYSAPI
NTSTATUS
NTAPI
RtlDescribeChunk(
  IN USHORT CompressionFormat,
  IN OUT PUCHAR *CompressedBuffer,
  IN PUCHAR EndOfCompressedBufferPlus1,
  OUT PUCHAR *ChunkBuffer,
  OUT PULONG ChunkSize);

NTSYSAPI
NTSTATUS
NTAPI
RtlReserveChunk(
  IN USHORT CompressionFormat,
  IN OUT PUCHAR *CompressedBuffer,
  IN PUCHAR EndOfCompressedBufferPlus1,
  OUT PUCHAR *ChunkBuffer,
  IN ULONG ChunkSize);

NTSYSAPI
NTSTATUS
NTAPI
RtlDecompressChunks(
  OUT PUCHAR UncompressedBuffer,
  IN ULONG UncompressedBufferSize,
  IN PUCHAR CompressedBuffer,
  IN ULONG CompressedBufferSize,
  IN PUCHAR CompressedTail,
  IN ULONG CompressedTailSize,
  IN PCOMPRESSED_DATA_INFO CompressedDataInfo);

NTSYSAPI
NTSTATUS
NTAPI
RtlCompressChunks(
  IN PUCHAR UncompressedBuffer,
  IN ULONG UncompressedBufferSize,
  OUT PUCHAR CompressedBuffer,
  IN ULONG CompressedBufferSize,
  IN OUT PCOMPRESSED_DATA_INFO CompressedDataInfo,
  IN ULONG CompressedDataInfoLength,
  IN PVOID WorkSpace);

NTSYSAPI
PSID_IDENTIFIER_AUTHORITY
NTAPI
RtlIdentifierAuthoritySid(
  IN PSID Sid);

NTSYSAPI
PUCHAR
NTAPI
RtlSubAuthorityCountSid(
  IN PSID Sid);

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosErrorNoTeb(
  IN NTSTATUS Status);

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateSystemVolumeInformationFolder(
  IN PCUNICODE_STRING VolumeRootPath);

#endif

#if defined(_M_AMD64)

FORCEINLINE
VOID
RtlFillMemoryUlong (
  OUT PVOID Destination,
  IN SIZE_T Length,
  IN ULONG Pattern)
{
  PULONG Address = (PULONG)Destination;
  if ((Length /= 4) != 0) {
    if (((ULONG64)Address & 4) != 0) {
      *Address = Pattern;
      if ((Length -= 1) == 0) {
        return;
      }
    Address += 1;
    }
    __stosq((PULONG64)(Address), Pattern | ((ULONG64)Pattern << 32), Length / 2);
    if ((Length & 1) != 0) Address[Length - 1] = Pattern;
  }
  return;
}

#define RtlFillMemoryUlonglong(Destination, Length, Pattern)                \
    __stosq((PULONG64)(Destination), Pattern, (Length) / 8)

#else

#if (NTDDI_VERSION >= NTDDI_WINXP)

NTSYSAPI
VOID
NTAPI
RtlFillMemoryUlong(
  OUT PVOID Destination,
  IN SIZE_T Length,
  IN ULONG Pattern);

NTSYSAPI
VOID
NTAPI
RtlFillMemoryUlonglong(
  OUT PVOID Destination,
  IN SIZE_T Length,
  IN ULONGLONG Pattern);

#endif

#endif // defined(_M_AMD64)

#if (NTDDI_VERSION >= NTDDI_WS03)

NTSYSAPI
NTSTATUS
NTAPI
RtlInitAnsiStringEx(
  OUT PANSI_STRING DestinationString,
  IN PCSZ SourceString OPTIONAL);

#endif

#if (NTDDI_VERSION >= NTDDI_WS03SP1)

NTSYSAPI
NTSTATUS
NTAPI
RtlGetSaclSecurityDescriptor(
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  OUT PBOOLEAN SaclPresent,
  OUT PACL *Sacl,
  OUT PBOOLEAN SaclDefaulted);

NTSYSAPI
NTSTATUS
NTAPI
RtlSetGroupSecurityDescriptor(
  IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID Group OPTIONAL,
  IN BOOLEAN GroupDefaulted OPTIONAL);

NTSYSAPI
NTSTATUS
NTAPI
RtlGetGroupSecurityDescriptor(
  IN PSECURITY_DESCRIPTOR SecurityDescriptor,
  OUT PSID *Group,
  OUT PBOOLEAN GroupDefaulted);

NTSYSAPI
NTSTATUS
NTAPI
RtlAbsoluteToSelfRelativeSD(
  IN PSECURITY_DESCRIPTOR AbsoluteSecurityDescriptor,
  OUT PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor OPTIONAL,
  IN OUT PULONG BufferLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlSelfRelativeToAbsoluteSD(
  IN PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor,
  OUT PSECURITY_DESCRIPTOR AbsoluteSecurityDescriptor OPTIONAL,
  IN OUT PULONG AbsoluteSecurityDescriptorSize,
  OUT PACL Dacl OPTIONAL,
  IN OUT PULONG DaclSize,
  OUT PACL Sacl OPTIONAL,
  IN OUT PULONG SaclSize,
  OUT PSID Owner OPTIONAL,
  IN OUT PULONG OwnerSize,
  OUT PSID PrimaryGroup OPTIONAL,
  IN OUT PULONG PrimaryGroupSize);

#endif

#if (NTDDI_VERSION >= NTDDI_VISTA)

NTSYSAPI
NTSTATUS
NTAPI
RtlNormalizeString(
  IN ULONG NormForm,
  IN PCWSTR SourceString,
  IN LONG SourceStringLength,
  OUT PWSTR DestinationString,
  IN OUT PLONG DestinationStringLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlIsNormalizedString(
  IN ULONG NormForm,
  IN PCWSTR SourceString,
  IN LONG SourceStringLength,
  OUT PBOOLEAN Normalized);

NTSYSAPI
NTSTATUS
NTAPI
RtlIdnToAscii(
  IN ULONG Flags,
  IN PCWSTR SourceString,
  IN LONG SourceStringLength,
  OUT PWSTR DestinationString,
  IN OUT PLONG DestinationStringLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlIdnToUnicode(
  IN ULONG Flags,
  IN PCWSTR SourceString,
  IN LONG SourceStringLength,
  OUT PWSTR DestinationString,
  IN OUT PLONG DestinationStringLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlIdnToNameprepUnicode(
  IN ULONG Flags,
  IN PCWSTR SourceString,
  IN LONG SourceStringLength,
  OUT PWSTR DestinationString,
  IN OUT PLONG DestinationStringLength);

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateServiceSid(
  IN PUNICODE_STRING ServiceName,
  OUT PSID ServiceSid,
  IN OUT PULONG ServiceSidLength);

NTSYSAPI
LONG
NTAPI
RtlCompareAltitudes(
  IN PCUNICODE_STRING Altitude1,
  IN PCUNICODE_STRING Altitude2);

#endif

#if (NTDDI_VERSION >= NTDDI_WIN7)

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToUTF8N(
  OUT PCHAR UTF8StringDestination,
  IN ULONG UTF8StringMaxByteCount,
  OUT PULONG UTF8StringActualByteCount,
  IN PCWCH UnicodeStringSource,
  IN ULONG UnicodeStringByteCount);

NTSYSAPI
NTSTATUS
NTAPI
RtlUTF8ToUnicodeN(
  OUT PWSTR UnicodeStringDestination,
  IN ULONG UnicodeStringMaxByteCount,
  OUT PULONG UnicodeStringActualByteCount,
  IN PCCH UTF8StringSource,
  IN ULONG UTF8StringByteCount);

NTSYSAPI
NTSTATUS
NTAPI
RtlReplaceSidInSd(
  IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
  IN PSID OldSid,
  IN PSID NewSid,
  OUT ULONG *NumChanges);

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateVirtualAccountSid(
  IN PCUNICODE_STRING Name,
  IN ULONG BaseSubAuthority,
  OUT PSID Sid,
  IN OUT PULONG SidLength);

#endif

#define HEAP_NO_SERIALIZE               0x00000001
#define HEAP_GROWABLE                   0x00000002
#define HEAP_GENERATE_EXCEPTIONS        0x00000004
#define HEAP_ZERO_MEMORY                0x00000008
#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010
#define HEAP_TAIL_CHECKING_ENABLED      0x00000020
#define HEAP_FREE_CHECKING_ENABLED      0x00000040
#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080

#define HEAP_CREATE_ALIGN_16            0x00010000
#define HEAP_CREATE_ENABLE_TRACING      0x00020000
#define HEAP_CREATE_ENABLE_EXECUTE      0x00040000

#define HEAP_SETTABLE_USER_VALUE        0x00000100
#define HEAP_SETTABLE_USER_FLAG1        0x00000200
#define HEAP_SETTABLE_USER_FLAG2        0x00000400
#define HEAP_SETTABLE_USER_FLAG3        0x00000800
#define HEAP_SETTABLE_USER_FLAGS        0x00000E00

#define HEAP_CLASS_0                    0x00000000
#define HEAP_CLASS_1                    0x00001000
#define HEAP_CLASS_2                    0x00002000
#define HEAP_CLASS_3                    0x00003000
#define HEAP_CLASS_4                    0x00004000
#define HEAP_CLASS_5                    0x00005000
#define HEAP_CLASS_6                    0x00006000
#define HEAP_CLASS_7                    0x00007000
#define HEAP_CLASS_8                    0x00008000
#define HEAP_CLASS_MASK                 0x0000F000

#define HEAP_MAXIMUM_TAG                0x0FFF
#define HEAP_GLOBAL_TAG                 0x0800
#define HEAP_PSEUDO_TAG_FLAG            0x8000
#define HEAP_TAG_SHIFT                  18
#define HEAP_TAG_MASK                  (HEAP_MAXIMUM_TAG << HEAP_TAG_SHIFT)

#define HEAP_CREATE_VALID_MASK         (HEAP_NO_SERIALIZE |             \
                                        HEAP_GROWABLE |                 \
                                        HEAP_GENERATE_EXCEPTIONS |      \
                                        HEAP_ZERO_MEMORY |              \
                                        HEAP_REALLOC_IN_PLACE_ONLY |    \
                                        HEAP_TAIL_CHECKING_ENABLED |    \
                                        HEAP_FREE_CHECKING_ENABLED |    \
                                        HEAP_DISABLE_COALESCE_ON_FREE | \
                                        HEAP_CLASS_MASK |               \
                                        HEAP_CREATE_ALIGN_16 |          \
                                        HEAP_CREATE_ENABLE_TRACING |    \
                                        HEAP_CREATE_ENABLE_EXECUTE)

FORCEINLINE
ULONG
HEAP_MAKE_TAG_FLAGS(
  IN ULONG TagBase,
  IN ULONG Tag)
{
  __assume_bound(TagBase);
  return ((ULONG)((TagBase) + ((Tag) << HEAP_TAG_SHIFT)));
}

#define RTL_DUPLICATE_UNICODE_STRING_NULL_TERMINATE 1
#define RTL_DUPLICATE_UNICODE_STRING_ALLOCATE_NULL_STRING 2

#define RtlUnicodeStringToOemSize(STRING) (NLS_MB_OEM_CODE_PAGE_TAG ?                                \
                                           RtlxUnicodeStringToOemSize(STRING) :                      \
                                           ((STRING)->Length + sizeof(UNICODE_NULL)) / sizeof(WCHAR) \
)

#define RtlOemStringToUnicodeSize(STRING) (                 \
    NLS_MB_OEM_CODE_PAGE_TAG ?                              \
    RtlxOemStringToUnicodeSize(STRING) :                    \
    ((STRING)->Length + sizeof(ANSI_NULL)) * sizeof(WCHAR)  \
)

#define RtlOemStringToCountedUnicodeSize(STRING) (                    \
    (ULONG)(RtlOemStringToUnicodeSize(STRING) - sizeof(UNICODE_NULL)) \
)

typedef PVOID
(NTAPI *PRTL_ALLOCATE_STRING_ROUTINE (
  IN SIZE_T NumberOfBytes);

#if _WIN32_WINNT >= 0x0600

typedef PVOID
(NTAPI *PRTL_REALLOCATE_STRING_ROUTINE (
  IN SIZE_T NumberOfBytes,
  IN PVOID Buffer);

#endif

typedef VOID
(NTAPI *PRTL_FREE_STRING_ROUTINE (
  IN PVOID Buffer);

extern const PRTL_ALLOCATE_STRING_ROUTINE RtlAllocateStringRoutine;
extern const PRTL_FREE_STRING_ROUTINE RtlFreeStringRoutine;

#if _WIN32_WINNT >= 0x0600
extern const PRTL_REALLOCATE_STRING_ROUTINE RtlReallocateStringRoutine;
#endif

typedef struct _GENERATE_NAME_CONTEXT {
  USHORT Checksum;
  BOOLEAN CheckSumInserted;
  UCHAR NameLength;
  WCHAR NameBuffer[8];
  ULONG ExtensionLength;
  WCHAR ExtensionBuffer[4];
  ULONG LastIndexValue;
} GENERATE_NAME_CONTEXT, *PGENERATE_NAME_CONTEXT;

typedef struct _PREFIX_TABLE_ENTRY {
  CSHORT NodeTypeCode;
  CSHORT NameLength;
  struct _PREFIX_TABLE_ENTRY *NextPrefixTree;
  RTL_SPLAY_LINKS Links;
  PSTRING Prefix;
} PREFIX_TABLE_ENTRY, *PPREFIX_TABLE_ENTRY;

typedef struct _PREFIX_TABLE {
  CSHORT NodeTypeCode;
  CSHORT NameLength;
  PPREFIX_TABLE_ENTRY NextPrefixTree;
} PREFIX_TABLE, *PPREFIX_TABLE;

typedef struct _UNICODE_PREFIX_TABLE_ENTRY {
  CSHORT NodeTypeCode;
  CSHORT NameLength;
  struct _UNICODE_PREFIX_TABLE_ENTRY *NextPrefixTree;
  struct _UNICODE_PREFIX_TABLE_ENTRY *CaseMatch;
  RTL_SPLAY_LINKS Links;
  PUNICODE_STRING Prefix;
} UNICODE_PREFIX_TABLE_ENTRY, *PUNICODE_PREFIX_TABLE_ENTRY;

typedef struct _UNICODE_PREFIX_TABLE {
  CSHORT NodeTypeCode;
  CSHORT NameLength;
  PUNICODE_PREFIX_TABLE_ENTRY NextPrefixTree;
  PUNICODE_PREFIX_TABLE_ENTRY LastNextEntry;
} UNICODE_PREFIX_TABLE, *PUNICODE_PREFIX_TABLE;

#define COMPRESSION_FORMAT_NONE         (0x0000)
#define COMPRESSION_FORMAT_DEFAULT      (0x0001)
#define COMPRESSION_FORMAT_LZNT1        (0x0002)
#define COMPRESSION_ENGINE_STANDARD     (0x0000)
#define COMPRESSION_ENGINE_MAXIMUM      (0x0100)
#define COMPRESSION_ENGINE_HIBER        (0x0200)

typedef struct _COMPRESSED_DATA_INFO {
  USHORT CompressionFormatAndEngine;
  UCHAR CompressionUnitShift;
  UCHAR ChunkShift;
  UCHAR ClusterShift;
  UCHAR Reserved;
  USHORT NumberOfChunks;
  ULONG CompressedChunkSizes[ANYSIZE_ARRAY];
} COMPRESSED_DATA_INFO, *PCOMPRESSED_DATA_INFO;

#define RtlOffsetToPointer(B,O)  ((PCHAR)( ((PCHAR)(B)) + ((ULONG_PTR)(O))  ))
#define RtlPointerToOffset(B,P)  ((ULONG)( ((PCHAR)(P)) - ((PCHAR)(B))  ))

#define MAX_UNICODE_STACK_BUFFER_LENGTH 256

#define RTL_SYSTEM_VOLUME_INFORMATION_FOLDER    L"System Volume Information"

#define DEVICE_TYPE ULONG

#define FILE_DEVICE_BEEP                0x00000001
#define FILE_DEVICE_CD_ROM              0x00000002
#define FILE_DEVICE_CD_ROM_FILE_SYSTEM  0x00000003
#define FILE_DEVICE_CONTROLLER          0x00000004
#define FILE_DEVICE_DATALINK            0x00000005
#define FILE_DEVICE_DFS                 0x00000006
#define FILE_DEVICE_DISK                0x00000007
#define FILE_DEVICE_DISK_FILE_SYSTEM    0x00000008
#define FILE_DEVICE_FILE_SYSTEM         0x00000009
#define FILE_DEVICE_INPORT_PORT         0x0000000a
#define FILE_DEVICE_KEYBOARD            0x0000000b
#define FILE_DEVICE_MAILSLOT            0x0000000c
#define FILE_DEVICE_MIDI_IN             0x0000000d
#define FILE_DEVICE_MIDI_OUT            0x0000000e
#define FILE_DEVICE_MOUSE               0x0000000f
#define FILE_DEVICE_MULTI_UNC_PROVIDER  0x00000010
#define FILE_DEVICE_NAMED_PIPE          0x00000011
#define FILE_DEVICE_NETWORK             0x00000012
#define FILE_DEVICE_NETWORK_BROWSER     0x00000013
#define FILE_DEVICE_NETWORK_FILE_SYSTEM 0x00000014
#define FILE_DEVICE_NULL                0x00000015
#define FILE_DEVICE_PARALLEL_PORT       0x00000016
#define FILE_DEVICE_PHYSICAL_NETCARD    0x00000017
#define FILE_DEVICE_PRINTER             0x00000018
#define FILE_DEVICE_SCANNER             0x00000019
#define FILE_DEVICE_SERIAL_MOUSE_PORT   0x0000001a
#define FILE_DEVICE_SERIAL_PORT         0x0000001b
#define FILE_DEVICE_SCREEN              0x0000001c
#define FILE_DEVICE_SOUND               0x0000001d
#define FILE_DEVICE_STREAMS             0x0000001e
#define FILE_DEVICE_TAPE                0x0000001f
#define FILE_DEVICE_TAPE_FILE_SYSTEM    0x00000020
#define FILE_DEVICE_TRANSPORT           0x00000021
#define FILE_DEVICE_UNKNOWN             0x00000022
#define FILE_DEVICE_VIDEO               0x00000023
#define FILE_DEVICE_VIRTUAL_DISK        0x00000024
#define FILE_DEVICE_WAVE_IN             0x00000025
#define FILE_DEVICE_WAVE_OUT            0x00000026
#define FILE_DEVICE_8042_PORT           0x00000027
#define FILE_DEVICE_NETWORK_REDIRECTOR  0x00000028
#define FILE_DEVICE_BATTERY             0x00000029
#define FILE_DEVICE_BUS_EXTENDER        0x0000002a
#define FILE_DEVICE_MODEM               0x0000002b
#define FILE_DEVICE_VDM                 0x0000002c
#define FILE_DEVICE_MASS_STORAGE        0x0000002d
#define FILE_DEVICE_SMB                 0x0000002e
#define FILE_DEVICE_KS                  0x0000002f
#define FILE_DEVICE_CHANGER             0x00000030
#define FILE_DEVICE_SMARTCARD           0x00000031
#define FILE_DEVICE_ACPI                0x00000032
#define FILE_DEVICE_DVD                 0x00000033
#define FILE_DEVICE_FULLSCREEN_VIDEO    0x00000034
#define FILE_DEVICE_DFS_FILE_SYSTEM     0x00000035
#define FILE_DEVICE_DFS_VOLUME          0x00000036
#define FILE_DEVICE_SERENUM             0x00000037
#define FILE_DEVICE_TERMSRV             0x00000038
#define FILE_DEVICE_KSEC                0x00000039
#define FILE_DEVICE_FIPS                0x0000003A
#define FILE_DEVICE_INFINIBAND          0x0000003B
#define FILE_DEVICE_VMBUS               0x0000003E
#define FILE_DEVICE_CRYPT_PROVIDER      0x0000003F
#define FILE_DEVICE_WPD                 0x00000040
#define FILE_DEVICE_BLUETOOTH           0x00000041
#define FILE_DEVICE_MT_COMPOSITE        0x00000042
#define FILE_DEVICE_MT_TRANSPORT        0x00000043
#define FILE_DEVICE_BIOMETRIC           0x00000044
#define FILE_DEVICE_PMI                 0x00000045

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#define DEVICE_TYPE_FROM_CTL_CODE(ctrlCode)     (((ULONG)(ctrlCode & 0xffff0000)) >> 16)
#define METHOD_FROM_CTL_CODE(ctrlCode)          ((ULONG)(ctrlCode & 3))

#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3
#define METHOD_DIRECT_TO_HARDWARE       METHOD_IN_DIRECT
#define METHOD_DIRECT_FROM_HARDWARE     METHOD_OUT_DIRECT

#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS             (FILE_ANY_ACCESS)
#define FILE_READ_ACCESS                ( 0x0001 )
#define FILE_WRITE_ACCESS               ( 0x0002 )

typedef ULONG  LSA_OPERATIONAL_MODE, *PLSA_OPERATIONAL_MODE;

typedef enum _SECURITY_LOGON_TYPE {
  UndefinedLogonType = 0,
  Interactive = 2,
  Network,
  Batch,
  Service,
  Proxy,
  Unlock,
  NetworkCleartext,
  NewCredentials,
#if (_WIN32_WINNT >= 0x0501)
  RemoteInteractive,
  CachedInteractive,
#endif
#if (_WIN32_WINNT >= 0x0502)
  CachedRemoteInteractive,
  CachedUnlock
#endif
} SECURITY_LOGON_TYPE, *PSECURITY_LOGON_TYPE;

#ifndef _NTLSA_AUDIT_
#define _NTLSA_AUDIT_

typedef enum _SE_ADT_PARAMETER_TYPE {
  SeAdtParmTypeNone = 0,
  SeAdtParmTypeString,
  SeAdtParmTypeFileSpec,
  SeAdtParmTypeUlong,
  SeAdtParmTypeSid,
  SeAdtParmTypeLogonId,
  SeAdtParmTypeNoLogonId,
  SeAdtParmTypeAccessMask,
  SeAdtParmTypePrivs,
  SeAdtParmTypeObjectTypes,
  SeAdtParmTypeHexUlong,
  SeAdtParmTypePtr,
  SeAdtParmTypeTime,
  SeAdtParmTypeGuid,
  SeAdtParmTypeLuid,
  SeAdtParmTypeHexInt64,
  SeAdtParmTypeStringList,
  SeAdtParmTypeSidList,
  SeAdtParmTypeDuration,
  SeAdtParmTypeUserAccountControl,
  SeAdtParmTypeNoUac,
  SeAdtParmTypeMessage,
  SeAdtParmTypeDateTime,
  SeAdtParmTypeSockAddr,
  SeAdtParmTypeSD,
  SeAdtParmTypeLogonHours,
  SeAdtParmTypeLogonIdNoSid,
  SeAdtParmTypeUlongNoConv,
  SeAdtParmTypeSockAddrNoPort,
  SeAdtParmTypeAccessReason
} SE_ADT_PARAMETER_TYPE, *PSE_ADT_PARAMETER_TYPE;

#ifndef GUID_DEFINED
#include <guiddef.h>
#endif

typedef struct _SE_ADT_OBJECT_TYPE {
  GUID ObjectType;
  USHORT Flags;
#define SE_ADT_OBJECT_ONLY 0x1
  USHORT Level;
  ACCESS_MASK AccessMask;
} SE_ADT_OBJECT_TYPE, *PSE_ADT_OBJECT_TYPE;

typedef struct _SE_ADT_PARAMETER_ARRAY_ENTRY {
  SE_ADT_PARAMETER_TYPE Type;
  ULONG Length;
  ULONG_PTR Data[2];
  PVOID Address;
} SE_ADT_PARAMETER_ARRAY_ENTRY, *PSE_ADT_PARAMETER_ARRAY_ENTRY;

typedef struct _SE_ADT_ACCESS_REASON {
  ACCESS_MASK AccessMask;
  ULONG  AccessReasons[32];
  ULONG  ObjectTypeIndex;
  ULONG AccessGranted;
  PSECURITY_DESCRIPTOR SecurityDescriptor;
} SE_ADT_ACCESS_REASON, *PSE_ADT_ACCESS_REASON;

#define SE_MAX_AUDIT_PARAMETERS 32
#define SE_MAX_GENERIC_AUDIT_PARAMETERS 28

typedef struct _SE_ADT_PARAMETER_ARRAY {
  ULONG CategoryId;
  ULONG AuditId;
  ULONG ParameterCount;
  ULONG Length;
  USHORT FlatSubCategoryId;
  USHORT Type;
  ULONG Flags;
  SE_ADT_PARAMETER_ARRAY_ENTRY Parameters[ SE_MAX_AUDIT_PARAMETERS ];
} SE_ADT_PARAMETER_ARRAY, *PSE_ADT_PARAMETER_ARRAY;

#define SE_ADT_PARAMETERS_SELF_RELATIVE     0x00000001
#define SE_ADT_PARAMETERS_SEND_TO_LSA       0x00000002
#define SE_ADT_PARAMETER_EXTENSIBLE_AUDIT   0x00000004
#define SE_ADT_PARAMETER_GENERIC_AUDIT      0x00000008
#define SE_ADT_PARAMETER_WRITE_SYNCHRONOUS  0x00000010

#define LSAP_SE_ADT_PARAMETER_ARRAY_TRUE_SIZE(AuditParameters)    \
     ( sizeof(SE_ADT_PARAMETER_ARRAY) -                           \
       sizeof(SE_ADT_PARAMETER_ARRAY_ENTRY) *                     \
       (SE_MAX_AUDIT_PARAMETERS - AuditParameters->ParameterCount) )

#endif /* _NTLSA_AUDIT_ */

#pragma pack(push,4)

#ifndef VER_PRODUCTBUILD
#define VER_PRODUCTBUILD 10000
#endif

#define EX_PUSH_LOCK ULONG_PTR
#define PEX_PUSH_LOCK PULONG_PTR

#include "csq.h"

#ifdef _NTOSKRNL_
extern PUCHAR                       FsRtlLegalAnsiCharacterArray;
#else
extern DECLSPEC_IMPORT PUCHAR       FsRtlLegalAnsiCharacterArray;
#endif
extern PACL                         SePublicDefaultDacl;
extern PACL                         SeSystemDefaultDacl;

extern KSPIN_LOCK                   IoStatisticsLock;
extern ULONG                        IoReadOperationCount;
extern ULONG                        IoWriteOperationCount;
extern ULONG                        IoOtherOperationCount;
extern LARGE_INTEGER                IoReadTransferCount;
extern LARGE_INTEGER                IoWriteTransferCount;
extern LARGE_INTEGER                IoOtherTransferCount;

#define ANSI_DOS_STAR                   ('<')
#define ANSI_DOS_QM                     ('>')
#define ANSI_DOS_DOT                    ('"')

#define DOS_STAR                        (L'<')
#define DOS_QM                          (L'>')
#define DOS_DOT                         (L'"')

#define FILE_ACTION_ADDED                   0x00000001
#define FILE_ACTION_REMOVED                 0x00000002
#define FILE_ACTION_MODIFIED                0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005
#define FILE_ACTION_ADDED_STREAM            0x00000006
#define FILE_ACTION_REMOVED_STREAM          0x00000007
#define FILE_ACTION_MODIFIED_STREAM         0x00000008
#define FILE_ACTION_REMOVED_BY_DELETE       0x00000009
#define FILE_ACTION_ID_NOT_TUNNELLED        0x0000000A
#define FILE_ACTION_TUNNELLED_ID_COLLISION  0x0000000B
/* end  winnt.h */

#define FILE_EA_TYPE_BINARY             0xfffe
#define FILE_EA_TYPE_ASCII              0xfffd
#define FILE_EA_TYPE_BITMAP             0xfffb
#define FILE_EA_TYPE_METAFILE           0xfffa
#define FILE_EA_TYPE_ICON               0xfff9
#define FILE_EA_TYPE_EA                 0xffee
#define FILE_EA_TYPE_MVMT               0xffdf
#define FILE_EA_TYPE_MVST               0xffde
#define FILE_EA_TYPE_ASN1               0xffdd
#define FILE_EA_TYPE_FAMILY_IDS         0xff01

#define FILE_NEED_EA                    0x00000080

/* also in winnt.h */
#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002
#define FILE_NOTIFY_CHANGE_NAME         0x00000003
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040
#define FILE_NOTIFY_CHANGE_EA           0x00000080
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100
#define FILE_NOTIFY_CHANGE_STREAM_NAME  0x00000200
#define FILE_NOTIFY_CHANGE_STREAM_SIZE  0x00000400
#define FILE_NOTIFY_CHANGE_STREAM_WRITE 0x00000800
#define FILE_NOTIFY_VALID_MASK          0x00000fff
/* end winnt.h */

#define FILE_OPLOCK_BROKEN_TO_LEVEL_2   0x00000007
#define FILE_OPLOCK_BROKEN_TO_NONE      0x00000008

#define FILE_OPBATCH_BREAK_UNDERWAY     0x00000009

#define FILE_CASE_SENSITIVE_SEARCH      0x00000001
#define FILE_CASE_PRESERVED_NAMES       0x00000002
#define FILE_UNICODE_ON_DISK            0x00000004
#define FILE_PERSISTENT_ACLS            0x00000008
#define FILE_FILE_COMPRESSION           0x00000010
#define FILE_VOLUME_QUOTAS              0x00000020
#define FILE_SUPPORTS_SPARSE_FILES      0x00000040
#define FILE_SUPPORTS_REPARSE_POINTS    0x00000080
#define FILE_SUPPORTS_REMOTE_STORAGE    0x00000100
#define FS_LFN_APIS                     0x00004000
#define FILE_VOLUME_IS_COMPRESSED       0x00008000
#define FILE_SUPPORTS_OBJECT_IDS        0x00010000
#define FILE_SUPPORTS_ENCRYPTION        0x00020000
#define FILE_NAMED_STREAMS              0x00040000
#define FILE_READ_ONLY_VOLUME           0x00080000
#define FILE_SEQUENTIAL_WRITE_ONCE      0x00100000
#define FILE_SUPPORTS_TRANSACTIONS      0x00200000
    
#define FILE_PIPE_BYTE_STREAM_TYPE      0x00000000
#define FILE_PIPE_MESSAGE_TYPE          0x00000001

#define FILE_PIPE_BYTE_STREAM_MODE      0x00000000
#define FILE_PIPE_MESSAGE_MODE          0x00000001

#define FILE_PIPE_QUEUE_OPERATION       0x00000000
#define FILE_PIPE_COMPLETE_OPERATION    0x00000001

#define FILE_PIPE_INBOUND               0x00000000
#define FILE_PIPE_OUTBOUND              0x00000001
#define FILE_PIPE_FULL_DUPLEX           0x00000002

#define FILE_PIPE_DISCONNECTED_STATE    0x00000001
#define FILE_PIPE_LISTENING_STATE       0x00000002
#define FILE_PIPE_CONNECTED_STATE       0x00000003
#define FILE_PIPE_CLOSING_STATE         0x00000004

#define FILE_PIPE_CLIENT_END            0x00000000
#define FILE_PIPE_SERVER_END            0x00000001

#define FILE_PIPE_READ_DATA             0x00000000
#define FILE_PIPE_WRITE_SPACE           0x00000001

#define FILE_STORAGE_TYPE_SPECIFIED             0x00000041  /* FILE_DIRECTORY_FILE | FILE_NON_DIRECTORY_FILE */
#define FILE_STORAGE_TYPE_DEFAULT               (StorageTypeDefault << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_DIRECTORY             (StorageTypeDirectory << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_FILE                  (StorageTypeFile << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_DOCFILE               (StorageTypeDocfile << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_JUNCTION_POINT        (StorageTypeJunctionPoint << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_CATALOG               (StorageTypeCatalog << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_STRUCTURED_STORAGE    (StorageTypeStructuredStorage << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_EMBEDDING             (StorageTypeEmbedding << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_STREAM                (StorageTypeStream << FILE_STORAGE_TYPE_SHIFT)
#define FILE_MINIMUM_STORAGE_TYPE               FILE_STORAGE_TYPE_DEFAULT
#define FILE_MAXIMUM_STORAGE_TYPE               FILE_STORAGE_TYPE_STREAM
#define FILE_STORAGE_TYPE_MASK                  0x000f0000
#define FILE_STORAGE_TYPE_SHIFT                 16

#define FILE_VC_QUOTA_NONE              0x00000000
#define FILE_VC_QUOTA_TRACK             0x00000001
#define FILE_VC_QUOTA_ENFORCE           0x00000002
#define FILE_VC_QUOTA_MASK              0x00000003

#define FILE_VC_QUOTAS_LOG_VIOLATIONS   0x00000004
#define FILE_VC_CONTENT_INDEX_DISABLED  0x00000008

#define FILE_VC_LOG_QUOTA_THRESHOLD     0x00000010
#define FILE_VC_LOG_QUOTA_LIMIT         0x00000020
#define FILE_VC_LOG_VOLUME_THRESHOLD    0x00000040
#define FILE_VC_LOG_VOLUME_LIMIT        0x00000080

#define FILE_VC_QUOTAS_INCOMPLETE       0x00000100
#define FILE_VC_QUOTAS_REBUILDING       0x00000200

#define FILE_VC_VALID_MASK              0x000003ff

#define FSRTL_FLAG_FILE_MODIFIED        (0x01)
#define FSRTL_FLAG_FILE_LENGTH_CHANGED  (0x02)
#define FSRTL_FLAG_LIMIT_MODIFIED_PAGES (0x04)
#define FSRTL_FLAG_ACQUIRE_MAIN_RSRC_EX (0x08)
#define FSRTL_FLAG_ACQUIRE_MAIN_RSRC_SH (0x10)
#define FSRTL_FLAG_USER_MAPPED_FILE     (0x20)
#define FSRTL_FLAG_ADVANCED_HEADER      (0x40)
#define FSRTL_FLAG_EOF_ADVANCE_ACTIVE   (0x80)

#define FSRTL_FLAG2_DO_MODIFIED_WRITE        (0x01)
#define FSRTL_FLAG2_SUPPORTS_FILTER_CONTEXTS (0x02)
#define FSRTL_FLAG2_PURGE_WHEN_MAPPED        (0x04)
#define FSRTL_FLAG2_IS_PAGING_FILE           (0x08)

#define FSRTL_FSP_TOP_LEVEL_IRP         (0x01)
#define FSRTL_CACHE_TOP_LEVEL_IRP       (0x02)
#define FSRTL_MOD_WRITE_TOP_LEVEL_IRP   (0x03)
#define FSRTL_FAST_IO_TOP_LEVEL_IRP     (0x04)
#define FSRTL_MAX_TOP_LEVEL_IRP_FLAG    (0x04)

#define FSRTL_VOLUME_DISMOUNT           1
#define FSRTL_VOLUME_DISMOUNT_FAILED    2
#define FSRTL_VOLUME_LOCK               3
#define FSRTL_VOLUME_LOCK_FAILED        4
#define FSRTL_VOLUME_UNLOCK             5
#define FSRTL_VOLUME_MOUNT              6

#define FSRTL_WILD_CHARACTER            0x08

#define FSRTL_FAT_LEGAL                 0x01
#define FSRTL_HPFS_LEGAL                0x02
#define FSRTL_NTFS_LEGAL                0x04
#define FSRTL_WILD_CHARACTER            0x08
#define FSRTL_OLE_LEGAL                 0x10
#define FSRTL_NTFS_STREAM_LEGAL         0x14

#ifdef _X86_
#define HARDWARE_PTE    HARDWARE_PTE_X86
#define PHARDWARE_PTE   PHARDWARE_PTE_X86
#endif

#define IO_CHECK_CREATE_PARAMETERS      0x0200
#define IO_ATTACH_DEVICE                0x0400

#define IO_ATTACH_DEVICE_API            0x80000000

#define IO_FILE_OBJECT_NON_PAGED_POOL_CHARGE    64
#define IO_FILE_OBJECT_PAGED_POOL_CHARGE        1024

#define IO_TYPE_APC                     18
#define IO_TYPE_DPC                     19
#define IO_TYPE_DEVICE_QUEUE            20
#define IO_TYPE_EVENT_PAIR              21
#define IO_TYPE_INTERRUPT               22
#define IO_TYPE_PROFILE                 23

#define IRP_BEING_VERIFIED              0x10

#define MAILSLOT_CLASS_FIRSTCLASS       1
#define MAILSLOT_CLASS_SECONDCLASS      2

#define MAILSLOT_SIZE_AUTO              0

#define MEM_DOS_LIM                     0x40000000

#define MCB_FLAG_RAISE_ON_ALLOCATION_FAILURE 1

#define OB_TYPE_TYPE                    1
#define OB_TYPE_DIRECTORY               2
#define OB_TYPE_SYMBOLIC_LINK           3
#define OB_TYPE_TOKEN                   4
#define OB_TYPE_PROCESS                 5
#define OB_TYPE_THREAD                  6
#define OB_TYPE_EVENT                   7
#define OB_TYPE_EVENT_PAIR              8
#define OB_TYPE_MUTANT                  9
#define OB_TYPE_SEMAPHORE               10
#define OB_TYPE_TIMER                   11
#define OB_TYPE_PROFILE                 12
#define OB_TYPE_WINDOW_STATION          13
#define OB_TYPE_DESKTOP                 14
#define OB_TYPE_SECTION                 15
#define OB_TYPE_KEY                     16
#define OB_TYPE_PORT                    17
#define OB_TYPE_ADAPTER                 18
#define OB_TYPE_CONTROLLER              19
#define OB_TYPE_DEVICE                  20
#define OB_TYPE_DRIVER                  21
#define OB_TYPE_IO_COMPLETION           22
#define OB_TYPE_FILE                    23

#define PIN_WAIT                        (1)
#define PIN_EXCLUSIVE                   (2)
#define PIN_NO_READ                     (4)
#define PIN_IF_BCB                      (8)

#define SEC_BASED	0x00200000

#define SECURITY_WORLD_SID_AUTHORITY    {0,0,0,0,0,1}
#define SECURITY_WORLD_RID              (0x00000000L)

/* end winnt.h */

#define TOKEN_HAS_TRAVERSE_PRIVILEGE    0x01
#define TOKEN_HAS_BACKUP_PRIVILEGE      0x02
#define TOKEN_HAS_RESTORE_PRIVILEGE     0x04
#define TOKEN_HAS_ADMIN_GROUP           0x08
#define TOKEN_WRITE_RESTRICTED          0x08
#define TOKEN_IS_RESTRICTED             0x10
#define SE_BACKUP_PRIVILEGES_CHECKED    0x0100

#define VACB_MAPPING_GRANULARITY        (0x40000)
#define VACB_OFFSET_SHIFT               (18)

#define FSCTL_REQUEST_OPLOCK_LEVEL_1    CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_OPLOCK_LEVEL_2    CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_BATCH_OPLOCK      CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPLOCK_BREAK_ACKNOWLEDGE  CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPBATCH_ACK_CLOSE_PENDING CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPLOCK_BREAK_NOTIFY       CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_LOCK_VOLUME               CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_UNLOCK_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DISMOUNT_VOLUME           CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  8, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_IS_VOLUME_MOUNTED         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_IS_PATHNAME_VALID         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_MARK_VOLUME_DIRTY         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_QUERY_RETRIEVAL_POINTERS  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 14,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_GET_COMPRESSION           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SET_COMPRESSION           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 16, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)


#define FSCTL_MARK_AS_SYSTEM_HIVE       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 19,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_OPLOCK_BREAK_ACK_NO_2     CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 20, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_INVALIDATE_VOLUMES        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_QUERY_FAT_BPB             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 22, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_FILTER_OPLOCK     CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 23, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_FILESYSTEM_GET_STATISTICS CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 24, METHOD_BUFFERED, FILE_ANY_ACCESS)

#if (VER_PRODUCTBUILD >= 1381)

#define FSCTL_GET_NTFS_VOLUME_DATA      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 25, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_NTFS_FILE_RECORD      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 26, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_VOLUME_BITMAP         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 27,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_GET_RETRIEVAL_POINTERS    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 28,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_MOVE_FILE                 CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 29, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_IS_VOLUME_DIRTY           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 30, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_HFS_INFORMATION       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 31, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_ALLOW_EXTENDED_DASD_IO    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 32, METHOD_NEITHER,  FILE_ANY_ACCESS)

#endif /* (VER_PRODUCTBUILD >= 1381) */

#if (VER_PRODUCTBUILD >= 2195)

#define FSCTL_READ_PROPERTY_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 33, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_WRITE_PROPERTY_DATA       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 34, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_FIND_FILES_BY_SID         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 35, METHOD_NEITHER, FILE_ANY_ACCESS)

#define FSCTL_DUMP_PROPERTY_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 37,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_SET_OBJECT_ID             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 38, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_GET_OBJECT_ID             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 39, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_OBJECT_ID          CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 40, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_SET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 41, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_GET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_REPARSE_POINT      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 43, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_ENUM_USN_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 44,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_SECURITY_ID_CHECK         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 45,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_READ_USN_JOURNAL          CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 46,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_SET_OBJECT_ID_EXTENDED    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 47, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_CREATE_OR_GET_OBJECT_ID   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 48, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SET_SPARSE                CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_SET_ZERO_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_QUERY_ALLOCATED_RANGES    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 51,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_ENABLE_UPGRADE            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 52, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_SET_ENCRYPTION            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 53, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_ENCRYPTION_FSCTL_IO       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 54,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_WRITE_RAW_ENCRYPTED       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 55,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_READ_RAW_ENCRYPTED        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 56,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_CREATE_USN_JOURNAL        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 57,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_READ_FILE_USN_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 58,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_WRITE_USN_CLOSE_RECORD    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 59,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_EXTEND_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 60, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_QUERY_USN_JOURNAL         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_USN_JOURNAL        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_MARK_HANDLE               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 63, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SIS_COPYFILE              CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 64, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SIS_LINK_FILES            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 65, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_HSM_MSG                   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 66, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_NSS_CONTROL               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 67, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_HSM_DATA                  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 68, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_RECALL_FILE               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 69, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NSS_RCONTROL              CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 70, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_READ_FROM_PLEX            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 71, METHOD_OUT_DIRECT, FILE_READ_DATA)
#define FSCTL_FILE_PREFETCH             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#endif /* (VER_PRODUCTBUILD >= 2195) */

#define FSCTL_MAILSLOT_PEEK             CTL_CODE(FILE_DEVICE_MAILSLOT, 0, METHOD_NEITHER, FILE_READ_DATA)

#define FSCTL_NETWORK_SET_CONFIGURATION_INFO    CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 102, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_CONFIGURATION_INFO    CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 103, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_CONNECTION_INFO       CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 104, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_ENUMERATE_CONNECTIONS     CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 105, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_DELETE_CONNECTION         CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 107, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_STATISTICS            CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 116, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_SET_DOMAIN_NAME           CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 120, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_REMOTE_BOOT_INIT_SCRT     CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 250, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_PIPE_ASSIGN_EVENT         CTL_CODE(FILE_DEVICE_NAMED_PIPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_DISCONNECT           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_LISTEN               CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_PEEK                 CTL_CODE(FILE_DEVICE_NAMED_PIPE, 3, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_QUERY_EVENT          CTL_CODE(FILE_DEVICE_NAMED_PIPE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_TRANSCEIVE           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 5, METHOD_NEITHER,  FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_WAIT                 CTL_CODE(FILE_DEVICE_NAMED_PIPE, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_IMPERSONATE          CTL_CODE(FILE_DEVICE_NAMED_PIPE, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_CLIENT_PROCESS   CTL_CODE(FILE_DEVICE_NAMED_PIPE, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_QUERY_CLIENT_PROCESS CTL_CODE(FILE_DEVICE_NAMED_PIPE, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_INTERNAL_READ        CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2045, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_INTERNAL_WRITE       CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2046, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_TRANSCEIVE  CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2047, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_READ_OVFLOW CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2048, METHOD_BUFFERED, FILE_READ_DATA)

#define IOCTL_REDIR_QUERY_PATH          CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 99, METHOD_NEITHER, FILE_ANY_ACCESS)

typedef PVOID OPLOCK, *POPLOCK;

//
// Forwarders
//
struct _RTL_AVL_TABLE;
struct _RTL_GENERIC_TABLE;

typedef ULONG LBN;
typedef LBN *PLBN;

typedef ULONG VBN;
typedef VBN *PVBN;

typedef PVOID PNOTIFY_SYNC;

typedef enum _FAST_IO_POSSIBLE {
    FastIoIsNotPossible,
    FastIoIsPossible,
    FastIoIsQuestionable
} FAST_IO_POSSIBLE;

typedef enum _FILE_STORAGE_TYPE {
    StorageTypeDefault = 1,
    StorageTypeDirectory,
    StorageTypeFile,
    StorageTypeJunctionPoint,
    StorageTypeCatalog,
    StorageTypeStructuredStorage,
    StorageTypeEmbedding,
    StorageTypeStream
} FILE_STORAGE_TYPE;

typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectTypesInformation,
    ObjectHandleFlagInformation,
    ObjectSessionInformation,
    MaxObjectInfoClass
} OBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_BASIC_INFORMATION
{
    ULONG Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG HandleCount;
    ULONG PointerCount;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    ULONG Reserved[ 3 ];
    ULONG NameInfoSize;
    ULONG TypeInfoSize;
    ULONG SecurityDescriptorSize;
    LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

typedef struct _KAPC_STATE {
    LIST_ENTRY  ApcListHead[2];
    PKPROCESS   Process;
    BOOLEAN     KernelApcInProgress;
    BOOLEAN     KernelApcPending;
    BOOLEAN     UserApcPending;
} KAPC_STATE, *PKAPC_STATE, *RESTRICTED_POINTER PRKAPC_STATE;
#define KAPC_STATE_ACTUAL_LENGTH (FIELD_OFFSET(KAPC_STATE, UserApcPending) + sizeof(BOOLEAN))

typedef struct _BITMAP_RANGE {
    LIST_ENTRY      Links;
    LONGLONG        BasePage;
    ULONG           FirstDirtyPage;
    ULONG           LastDirtyPage;
    ULONG           DirtyPages;
    PULONG          Bitmap;
} BITMAP_RANGE, *PBITMAP_RANGE;

typedef struct _CACHE_UNINITIALIZE_EVENT {
    struct _CACHE_UNINITIALIZE_EVENT    *Next;
    KEVENT                              Event;
} CACHE_UNINITIALIZE_EVENT, *PCACHE_UNINITIALIZE_EVENT;

typedef struct _CC_FILE_SIZES {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER FileSize;
    LARGE_INTEGER ValidDataLength;
} CC_FILE_SIZES, *PCC_FILE_SIZES;

#define SYMLINK_FLAG_RELATIVE   1

typedef struct _REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    __GNU_EXTENSION union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;



//
// MicroSoft reparse point tags
//
#define IO_REPARSE_TAG_MOUNT_POINT              (0xA0000003L)
#define IO_REPARSE_TAG_HSM                      (0xC0000004L)
#define IO_REPARSE_TAG_DRIVE_EXTENDER           (0x80000005L)
#define IO_REPARSE_TAG_HSM2                     (0x80000006L)
#define IO_REPARSE_TAG_SIS                      (0x80000007L)
#define IO_REPARSE_TAG_DFS                      (0x8000000AL)
#define IO_REPARSE_TAG_FILTER_MANAGER           (0x8000000BL)
#define IO_REPARSE_TAG_SYMLINK                  (0xA000000CL)
#define IO_REPARSE_TAG_IIS_CACHE                (0xA0000010L)
#define IO_REPARSE_TAG_DFSR                     (0x80000012L)

//
// Reserved reparse tags
//
#define IO_REPARSE_TAG_RESERVED_ZERO            (0)
#define IO_REPARSE_TAG_RESERVED_ONE             (1)
#define IO_REPARSE_TAG_RESERVED_RANGE           IO_REPARSE_TAG_RESERVED_ONE


#define REPARSE_DATA_BUFFER_HEADER_SIZE   FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)

typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_ALLOCATION_INFORMATION {
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

typedef struct _FILE_BOTH_DIR_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           FileIndex;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
    LARGE_INTEGER   ChangeTime;
    LARGE_INTEGER   EndOfFile;
    LARGE_INTEGER   AllocationSize;
    ULONG           FileAttributes;
    ULONG           FileNameLength;
    ULONG           EaSize;
    CCHAR           ShortNameLength;
    WCHAR           ShortName[12];
    WCHAR           FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_COMPLETION_INFORMATION {
    HANDLE  Port;
    PVOID   Key;
} FILE_COMPLETION_INFORMATION, *PFILE_COMPLETION_INFORMATION;

typedef struct _FILE_COMPRESSION_INFORMATION {
    LARGE_INTEGER   CompressedFileSize;
    USHORT          CompressionFormat;
    UCHAR           CompressionUnitShift;
    UCHAR           ChunkShift;
    UCHAR           ClusterShift;
    UCHAR           Reserved[3];
} FILE_COMPRESSION_INFORMATION, *PFILE_COMPRESSION_INFORMATION;

typedef struct _FILE_COPY_ON_WRITE_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE  RootDirectory;
    ULONG   FileNameLength;
    WCHAR   FileName[1];
} FILE_COPY_ON_WRITE_INFORMATION, *PFILE_COPY_ON_WRITE_INFORMATION;

typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           FileIndex;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
    LARGE_INTEGER   ChangeTime;
    LARGE_INTEGER   EndOfFile;
    LARGE_INTEGER   AllocationSize;
    ULONG           FileAttributes;
    ULONG           FileNameLength;
    WCHAR           FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIRECTORY_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           FileIndex;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
    LARGE_INTEGER   ChangeTime;
    LARGE_INTEGER   EndOfFile;
    LARGE_INTEGER   AllocationSize;
    ULONG           FileAttributes;
    ULONG           FileNameLength;
    ULONG           EaSize;
    WCHAR           FileName[ANYSIZE_ARRAY];
} FILE_FULL_DIRECTORY_INFORMATION, *PFILE_FULL_DIRECTORY_INFORMATION;
    
typedef struct _FILE_ID_FULL_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

typedef struct _FILE_EA_INFORMATION {
    ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

typedef struct _FILE_FS_ATTRIBUTE_INFORMATION {
    ULONG   FileSystemAttributes;
    ULONG   MaximumComponentNameLength;
    ULONG   FileSystemNameLength;
    WCHAR   FileSystemName[1];
} FILE_FS_ATTRIBUTE_INFORMATION, *PFILE_FS_ATTRIBUTE_INFORMATION;

typedef struct _FILE_FS_CONTROL_INFORMATION {
    LARGE_INTEGER   FreeSpaceStartFiltering;
    LARGE_INTEGER   FreeSpaceThreshold;
    LARGE_INTEGER   FreeSpaceStopFiltering;
    LARGE_INTEGER   DefaultQuotaThreshold;
    LARGE_INTEGER   DefaultQuotaLimit;
    ULONG           FileSystemControlFlags;
} FILE_FS_CONTROL_INFORMATION, *PFILE_FS_CONTROL_INFORMATION;

typedef struct _FILE_FS_FULL_SIZE_INFORMATION {
    LARGE_INTEGER   TotalAllocationUnits;
    LARGE_INTEGER   CallerAvailableAllocationUnits;
    LARGE_INTEGER   ActualAvailableAllocationUnits;
    ULONG           SectorsPerAllocationUnit;
    ULONG           BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION, *PFILE_FS_FULL_SIZE_INFORMATION;

typedef struct _FILE_FS_LABEL_INFORMATION {
    ULONG VolumeLabelLength;
    WCHAR VolumeLabel[1];
} FILE_FS_LABEL_INFORMATION, *PFILE_FS_LABEL_INFORMATION;

#if (VER_PRODUCTBUILD >= 2195)

typedef struct _FILE_FS_OBJECT_ID_INFORMATION {
    UCHAR ObjectId[16];
    UCHAR ExtendedInfo[48];
} FILE_FS_OBJECT_ID_INFORMATION, *PFILE_FS_OBJECT_ID_INFORMATION;

#endif /* (VER_PRODUCTBUILD >= 2195) */

typedef struct _FILE_FS_SIZE_INFORMATION {
    LARGE_INTEGER   TotalAllocationUnits;
    LARGE_INTEGER   AvailableAllocationUnits;
    ULONG           SectorsPerAllocationUnit;
    ULONG           BytesPerSector;
} FILE_FS_SIZE_INFORMATION, *PFILE_FS_SIZE_INFORMATION;

typedef struct _FILE_FS_VOLUME_INFORMATION {
    LARGE_INTEGER   VolumeCreationTime;
    ULONG           VolumeSerialNumber;
    ULONG           VolumeLabelLength;
    BOOLEAN         SupportsObjects;
    WCHAR           VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef struct _FILE_FS_OBJECTID_INFORMATION
{
    UCHAR ObjectId[16];
    UCHAR ExtendedInfo[48];
} FILE_FS_OBJECTID_INFORMATION, *PFILE_FS_OBJECTID_INFORMATION;

typedef struct _FILE_FS_DRIVER_PATH_INFORMATION
{
    BOOLEAN DriverInPath;
    ULONG DriverNameLength;
    WCHAR DriverName[1];
} FILE_FS_DRIVER_PATH_INFORMATION, *PFILE_FS_DRIVER_PATH_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           FileIndex;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
    LARGE_INTEGER   ChangeTime;
    LARGE_INTEGER   EndOfFile;
    LARGE_INTEGER   AllocationSize;
    ULONG           FileAttributes;
    ULONG           FileNameLength;
    ULONG           EaSize;
    WCHAR           FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_GET_EA_INFORMATION {
    ULONG   NextEntryOffset;
    UCHAR   EaNameLength;
    CHAR    EaName[1];
} FILE_GET_EA_INFORMATION, *PFILE_GET_EA_INFORMATION;

typedef struct _FILE_GET_QUOTA_INFORMATION {
    ULONG   NextEntryOffset;
    ULONG   SidLength;
    SID     Sid;
} FILE_GET_QUOTA_INFORMATION, *PFILE_GET_QUOTA_INFORMATION;

typedef struct _FILE_QUOTA_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG SidLength;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER QuotaUsed;
    LARGE_INTEGER QuotaThreshold;
    LARGE_INTEGER QuotaLimit;
    SID Sid;
} FILE_QUOTA_INFORMATION, *PFILE_QUOTA_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION {
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_LINK_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE  RootDirectory;
    ULONG   FileNameLength;
    WCHAR   FileName[1];
} FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

typedef struct _FILE_LOCK_INFO
{
    LARGE_INTEGER StartingByte;
    LARGE_INTEGER Length;
    BOOLEAN ExclusiveLock;
    ULONG Key;
    PFILE_OBJECT FileObject;
    PVOID ProcessId;
    LARGE_INTEGER EndingByte;
} FILE_LOCK_INFO, *PFILE_LOCK_INFO;

typedef struct _FILE_REPARSE_POINT_INFORMATION
{
    LONGLONG FileReference;
    ULONG Tag;
} FILE_REPARSE_POINT_INFORMATION, *PFILE_REPARSE_POINT_INFORMATION;

typedef struct _FILE_MOVE_CLUSTER_INFORMATION
{
    ULONG ClusterCount;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_MOVE_CLUSTER_INFORMATION, *PFILE_MOVE_CLUSTER_INFORMATION;

typedef struct _FILE_NOTIFY_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG Action;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;

/* raw internal file lock struct returned from FsRtlGetNextFileLock */
typedef struct _FILE_SHARED_LOCK_ENTRY {
    PVOID           Unknown1;
    PVOID           Unknown2;
    FILE_LOCK_INFO  FileLock;
} FILE_SHARED_LOCK_ENTRY, *PFILE_SHARED_LOCK_ENTRY;

/* raw internal file lock struct returned from FsRtlGetNextFileLock */
typedef struct _FILE_EXCLUSIVE_LOCK_ENTRY {
    LIST_ENTRY      ListEntry;
    PVOID           Unknown1;
    PVOID           Unknown2;
    FILE_LOCK_INFO  FileLock;
} FILE_EXCLUSIVE_LOCK_ENTRY, *PFILE_EXCLUSIVE_LOCK_ENTRY;

typedef NTSTATUS (NTAPI *PCOMPLETE_LOCK_IRP_ROUTINE) (
    IN PVOID    Context,
    IN PIRP     Irp
);

typedef VOID (NTAPI *PUNLOCK_ROUTINE) (
    IN PVOID            Context,
    IN PFILE_LOCK_INFO  FileLockInfo
);

typedef struct _FILE_LOCK {
    PCOMPLETE_LOCK_IRP_ROUTINE  CompleteLockIrpRoutine;
    PUNLOCK_ROUTINE             UnlockRoutine;
    BOOLEAN                     FastIoIsQuestionable;
    BOOLEAN                     Pad[3];
    PVOID                       LockInformation;
    FILE_LOCK_INFO              LastReturnedLockInfo;
    PVOID                       LastReturnedLock;
} FILE_LOCK, *PFILE_LOCK;

typedef struct _FILE_MAILSLOT_PEEK_BUFFER {
    ULONG ReadDataAvailable;
    ULONG NumberOfMessages;
    ULONG MessageLength;
} FILE_MAILSLOT_PEEK_BUFFER, *PFILE_MAILSLOT_PEEK_BUFFER;

typedef struct _FILE_MAILSLOT_QUERY_INFORMATION {
    ULONG           MaximumMessageSize;
    ULONG           MailslotQuota;
    ULONG           NextMessageSize;
    ULONG           MessagesAvailable;
    LARGE_INTEGER   ReadTimeout;
} FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;

typedef struct _FILE_MAILSLOT_SET_INFORMATION {
    PLARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;

typedef struct _FILE_MODE_INFORMATION {
    ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

typedef struct _FILE_ALL_INFORMATION {
    FILE_BASIC_INFORMATION      BasicInformation;
    FILE_STANDARD_INFORMATION   StandardInformation;
    FILE_INTERNAL_INFORMATION   InternalInformation;
    FILE_EA_INFORMATION         EaInformation;
    FILE_ACCESS_INFORMATION     AccessInformation;
    FILE_POSITION_INFORMATION   PositionInformation;
    FILE_MODE_INFORMATION       ModeInformation;
    FILE_ALIGNMENT_INFORMATION  AlignmentInformation;
    FILE_NAME_INFORMATION       NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

typedef struct _FILE_OBJECTID_INFORMATION {
    LONGLONG        FileReference;
    UCHAR           ObjectId[16];
    _ANONYMOUS_UNION union {
        __GNU_EXTENSION struct {
            UCHAR   BirthVolumeId[16];
            UCHAR   BirthObjectId[16];
            UCHAR   DomainId[16];
        };
        UCHAR       ExtendedInfo[48];
    } DUMMYUNIONNAME;
} FILE_OBJECTID_INFORMATION, *PFILE_OBJECTID_INFORMATION;

typedef struct _FILE_OLE_CLASSID_INFORMATION {
    GUID ClassId;
} FILE_OLE_CLASSID_INFORMATION, *PFILE_OLE_CLASSID_INFORMATION;

typedef struct _FILE_OLE_ALL_INFORMATION {
    FILE_BASIC_INFORMATION          BasicInformation;
    FILE_STANDARD_INFORMATION       StandardInformation;
    FILE_INTERNAL_INFORMATION       InternalInformation;
    FILE_EA_INFORMATION             EaInformation;
    FILE_ACCESS_INFORMATION         AccessInformation;
    FILE_POSITION_INFORMATION       PositionInformation;
    FILE_MODE_INFORMATION           ModeInformation;
    FILE_ALIGNMENT_INFORMATION      AlignmentInformation;
    USN                             LastChangeUsn;
    USN                             ReplicationUsn;
    LARGE_INTEGER                   SecurityChangeTime;
    FILE_OLE_CLASSID_INFORMATION    OleClassIdInformation;
    FILE_OBJECTID_INFORMATION       ObjectIdInformation;
    FILE_STORAGE_TYPE               StorageType;
    ULONG                           OleStateBits;
    ULONG                           OleId;
    ULONG                           NumberOfStreamReferences;
    ULONG                           StreamIndex;
    ULONG                           SecurityId;
    BOOLEAN                         ContentIndexDisable;
    BOOLEAN                         InheritContentIndexDisable;
    FILE_NAME_INFORMATION           NameInformation;
} FILE_OLE_ALL_INFORMATION, *PFILE_OLE_ALL_INFORMATION;

typedef struct _FILE_OLE_DIR_INFORMATION {
    ULONG               NextEntryOffset;
    ULONG               FileIndex;
    LARGE_INTEGER       CreationTime;
    LARGE_INTEGER       LastAccessTime;
    LARGE_INTEGER       LastWriteTime;
    LARGE_INTEGER       ChangeTime;
    LARGE_INTEGER       EndOfFile;
    LARGE_INTEGER       AllocationSize;
    ULONG               FileAttributes;
    ULONG               FileNameLength;
    FILE_STORAGE_TYPE   StorageType;
    GUID                OleClassId;
    ULONG               OleStateBits;
    BOOLEAN             ContentIndexDisable;
    BOOLEAN             InheritContentIndexDisable;
    WCHAR               FileName[1];
} FILE_OLE_DIR_INFORMATION, *PFILE_OLE_DIR_INFORMATION;

typedef struct _FILE_OLE_INFORMATION {
    LARGE_INTEGER                   SecurityChangeTime;
    FILE_OLE_CLASSID_INFORMATION    OleClassIdInformation;
    FILE_OBJECTID_INFORMATION       ObjectIdInformation;
    FILE_STORAGE_TYPE               StorageType;
    ULONG                           OleStateBits;
    BOOLEAN                         ContentIndexDisable;
    BOOLEAN                         InheritContentIndexDisable;
} FILE_OLE_INFORMATION, *PFILE_OLE_INFORMATION;

typedef struct _FILE_OLE_STATE_BITS_INFORMATION {
    ULONG StateBits;
    ULONG StateBitsMask;
} FILE_OLE_STATE_BITS_INFORMATION, *PFILE_OLE_STATE_BITS_INFORMATION;

typedef struct _FILE_PIPE_ASSIGN_EVENT_BUFFER {
    HANDLE  EventHandle;
    ULONG   KeyValue;
} FILE_PIPE_ASSIGN_EVENT_BUFFER, *PFILE_PIPE_ASSIGN_EVENT_BUFFER;

typedef struct _FILE_PIPE_CLIENT_PROCESS_BUFFER {
    PVOID ClientSession;
    PVOID ClientProcess;
} FILE_PIPE_CLIENT_PROCESS_BUFFER, *PFILE_PIPE_CLIENT_PROCESS_BUFFER;

typedef struct _FILE_PIPE_EVENT_BUFFER {
    ULONG NamedPipeState;
    ULONG EntryType;
    ULONG ByteCount;
    ULONG KeyValue;
    ULONG NumberRequests;
} FILE_PIPE_EVENT_BUFFER, *PFILE_PIPE_EVENT_BUFFER;

typedef struct _FILE_PIPE_PEEK_BUFFER
{
    ULONG NamedPipeState;
    ULONG ReadDataAvailable;
    ULONG NumberOfMessages;
    ULONG MessageLength;
    CHAR Data[1];
} FILE_PIPE_PEEK_BUFFER, *PFILE_PIPE_PEEK_BUFFER;

typedef struct _FILE_PIPE_INFORMATION {
    ULONG ReadMode;
    ULONG CompletionMode;
} FILE_PIPE_INFORMATION, *PFILE_PIPE_INFORMATION;

typedef struct _FILE_PIPE_LOCAL_INFORMATION {
    ULONG NamedPipeType;
    ULONG NamedPipeConfiguration;
    ULONG MaximumInstances;
    ULONG CurrentInstances;
    ULONG InboundQuota;
    ULONG ReadDataAvailable;
    ULONG OutboundQuota;
    ULONG WriteQuotaAvailable;
    ULONG NamedPipeState;
    ULONG NamedPipeEnd;
} FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;

typedef struct _FILE_PIPE_REMOTE_INFORMATION {
    LARGE_INTEGER   CollectDataTime;
    ULONG           MaximumCollectionCount;
} FILE_PIPE_REMOTE_INFORMATION, *PFILE_PIPE_REMOTE_INFORMATION;

typedef struct _FILE_PIPE_WAIT_FOR_BUFFER {
    LARGE_INTEGER   Timeout;
    ULONG           NameLength;
    BOOLEAN         TimeoutSpecified;
    WCHAR           Name[1];
} FILE_PIPE_WAIT_FOR_BUFFER, *PFILE_PIPE_WAIT_FOR_BUFFER;

typedef struct _FILE_RENAME_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE  RootDirectory;
    ULONG   FileNameLength;
    WCHAR   FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

typedef struct _FILE_STREAM_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           StreamNameLength;
    LARGE_INTEGER   StreamSize;
    LARGE_INTEGER   StreamAllocationSize;
    WCHAR           StreamName[1];
} FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

typedef struct _FILE_TRACKING_INFORMATION {
    HANDLE  DestinationFile;
    ULONG   ObjectInformationLength;
    CHAR    ObjectInformation[1];
} FILE_TRACKING_INFORMATION, *PFILE_TRACKING_INFORMATION;

#if (VER_PRODUCTBUILD >= 2195)
typedef struct _FILE_ZERO_DATA_INFORMATION {
    LARGE_INTEGER FileOffset;
    LARGE_INTEGER BeyondFinalZero;
} FILE_ZERO_DATA_INFORMATION, *PFILE_ZERO_DATA_INFORMATION;

typedef struct FILE_ALLOCATED_RANGE_BUFFER {
    LARGE_INTEGER FileOffset;
    LARGE_INTEGER Length;
} FILE_ALLOCATED_RANGE_BUFFER, *PFILE_ALLOCATED_RANGE_BUFFER;
#endif /* (VER_PRODUCTBUILD >= 2195) */

#define FSRTL_FCB_HEADER_V0             (0x00)
#define FSRTL_FCB_HEADER_V1             (0x01)


typedef struct _FSRTL_COMMON_FCB_HEADER {
    CSHORT          NodeTypeCode;
    CSHORT          NodeByteSize;
    UCHAR           Flags;
    UCHAR           IsFastIoPossible;
#if (VER_PRODUCTBUILD >= 1381)
    UCHAR           Flags2;
    UCHAR           Reserved;
#endif /* (VER_PRODUCTBUILD >= 1381) */
    PERESOURCE      Resource;
    PERESOURCE      PagingIoResource;
    LARGE_INTEGER   AllocationSize;
    LARGE_INTEGER   FileSize;
    LARGE_INTEGER   ValidDataLength;
} FSRTL_COMMON_FCB_HEADER, *PFSRTL_COMMON_FCB_HEADER;

typedef enum _FSRTL_COMPARISON_RESULT
{
    LessThan = -1,
    EqualTo = 0,
    GreaterThan = 1
} FSRTL_COMPARISON_RESULT;
    
#if (VER_PRODUCTBUILD >= 2600)

typedef struct _FSRTL_ADVANCED_FCB_HEADER {
    CSHORT          NodeTypeCode;
    CSHORT          NodeByteSize;
    UCHAR           Flags;
    UCHAR           IsFastIoPossible;
    UCHAR           Flags2;
    UCHAR           Reserved: 4;
    UCHAR           Version: 4;
    PERESOURCE      Resource;
    PERESOURCE      PagingIoResource;
    LARGE_INTEGER   AllocationSize;
    LARGE_INTEGER   FileSize;
    LARGE_INTEGER   ValidDataLength;
    PFAST_MUTEX     FastMutex;
    LIST_ENTRY      FilterContexts;
    EX_PUSH_LOCK    PushLock;
    PVOID           *FileContextSupportPointer;
} FSRTL_ADVANCED_FCB_HEADER, *PFSRTL_ADVANCED_FCB_HEADER;

typedef struct _FSRTL_PER_STREAM_CONTEXT {
    LIST_ENTRY     Links;
    PVOID          OwnerId;
    PVOID          InstanceId;
    PFREE_FUNCTION FreeCallback;
} FSRTL_PER_STREAM_CONTEXT, *PFSRTL_PER_STREAM_CONTEXT;

typedef struct _FSRTL_PER_FILEOBJECT_CONTEXT
{
    LIST_ENTRY Links;
    PVOID OwnerId;
    PVOID InstanceId;
} FSRTL_PER_FILEOBJECT_CONTEXT, *PFSRTL_PER_FILEOBJECT_CONTEXT;

#endif /* (VER_PRODUCTBUILD >= 2600) */

typedef struct _BASE_MCB
{
    ULONG MaximumPairCount;
    ULONG PairCount;
    USHORT PoolType;
    USHORT Flags;
    PVOID Mapping;
} BASE_MCB, *PBASE_MCB;

typedef struct _LARGE_MCB
{
    PKGUARDED_MUTEX GuardedMutex;
    BASE_MCB BaseMcb;
} LARGE_MCB, *PLARGE_MCB;

typedef struct _MCB
{
    LARGE_MCB DummyFieldThatSizesThisStructureCorrectly;
} MCB, *PMCB;

typedef struct _MAPPING_PAIR {
    ULONGLONG Vcn;
    ULONGLONG Lcn;
} MAPPING_PAIR, *PMAPPING_PAIR;

typedef struct _GET_RETRIEVAL_DESCRIPTOR {
    ULONG           NumberOfPairs;
    ULONGLONG       StartVcn;
    MAPPING_PAIR    Pair[1];
} GET_RETRIEVAL_DESCRIPTOR, *PGET_RETRIEVAL_DESCRIPTOR;

typedef struct _KQUEUE {
    DISPATCHER_HEADER   Header;
    LIST_ENTRY          EntryListHead;
    ULONG               CurrentCount;
    ULONG               MaximumCount;
    LIST_ENTRY          ThreadListHead;
} KQUEUE, *PKQUEUE, *RESTRICTED_POINTER PRKQUEUE;

#define ASSERT_QUEUE(Q) ASSERT(((Q)->Header.Type & KOBJECT_TYPE_MASK) == QueueObject);

typedef struct _MBCB {
    CSHORT          NodeTypeCode;
    CSHORT          NodeIsInZone;
    ULONG           PagesToWrite;
    ULONG           DirtyPages;
    ULONG           Reserved;
    LIST_ENTRY      BitmapRanges;
    LONGLONG        ResumeWritePage;
    BITMAP_RANGE    BitmapRange1;
    BITMAP_RANGE    BitmapRange2;
    BITMAP_RANGE    BitmapRange3;
} MBCB, *PMBCB;

typedef enum _MMFLUSH_TYPE {
  MmFlushForDelete,
  MmFlushForWrite
} MMFLUSH_TYPE;

typedef struct _MOVEFILE_DESCRIPTOR {
     HANDLE         FileHandle;
     ULONG          Reserved;
     LARGE_INTEGER  StartVcn;
     LARGE_INTEGER  TargetLcn;
     ULONG          NumVcns;
     ULONG          Reserved1;
} MOVEFILE_DESCRIPTOR, *PMOVEFILE_DESCRIPTOR;

typedef struct _OBJECT_BASIC_INFO {
    ULONG           Attributes;
    ACCESS_MASK     GrantedAccess;
    ULONG           HandleCount;
    ULONG           ReferenceCount;
    ULONG           PagedPoolUsage;
    ULONG           NonPagedPoolUsage;
    ULONG           Reserved[3];
    ULONG           NameInformationLength;
    ULONG           TypeInformationLength;
    ULONG           SecurityDescriptorLength;
    LARGE_INTEGER   CreateTime;
} OBJECT_BASIC_INFO, *POBJECT_BASIC_INFO;

typedef struct _OBJECT_HANDLE_ATTRIBUTE_INFO {
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_ATTRIBUTE_INFO, *POBJECT_HANDLE_ATTRIBUTE_INFO;

typedef struct _OBJECT_NAME_INFO {
    UNICODE_STRING  ObjectName;
    WCHAR           ObjectNameBuffer[1];
} OBJECT_NAME_INFO, *POBJECT_NAME_INFO;

typedef struct _OBJECT_PROTECTION_INFO {
    BOOLEAN Inherit;
    BOOLEAN ProtectHandle;
} OBJECT_PROTECTION_INFO, *POBJECT_PROTECTION_INFO;

typedef struct _OBJECT_TYPE_INFO {
    UNICODE_STRING  ObjectTypeName;
    UCHAR           Unknown[0x58];
    WCHAR           ObjectTypeNameBuffer[1];
} OBJECT_TYPE_INFO, *POBJECT_TYPE_INFO;

typedef struct _OBJECT_ALL_TYPES_INFO {
    ULONG               NumberOfObjectTypes;
    OBJECT_TYPE_INFO    ObjectsTypeInfo[1];
} OBJECT_ALL_TYPES_INFO, *POBJECT_ALL_TYPES_INFO;

typedef struct _PATHNAME_BUFFER {
    ULONG PathNameLength;
    WCHAR Name[1];
} PATHNAME_BUFFER, *PPATHNAME_BUFFER;

typedef enum _RTL_GENERIC_COMPARE_RESULTS
{
    GenericLessThan,
    GenericGreaterThan,
    GenericEqual
} RTL_GENERIC_COMPARE_RESULTS;

typedef enum _TABLE_SEARCH_RESULT
{
    TableEmptyTree,
    TableFoundNode,
    TableInsertAsLeft,
    TableInsertAsRight
} TABLE_SEARCH_RESULT;

typedef NTSTATUS
(NTAPI *PRTL_AVL_MATCH_FUNCTION)(
    struct _RTL_AVL_TABLE *Table,
    PVOID UserData,
    PVOID MatchData
);

typedef RTL_GENERIC_COMPARE_RESULTS
(NTAPI *PRTL_AVL_COMPARE_ROUTINE) (
    struct _RTL_AVL_TABLE *Table,
    PVOID FirstStruct,
    PVOID SecondStruct
);

typedef RTL_GENERIC_COMPARE_RESULTS
(NTAPI *PRTL_GENERIC_COMPARE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    PVOID FirstStruct,
    PVOID SecondStruct
);

typedef PVOID
(NTAPI *PRTL_GENERIC_ALLOCATE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    CLONG ByteSize
);

typedef VOID
(NTAPI *PRTL_GENERIC_FREE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    PVOID Buffer
);

typedef PVOID
(NTAPI *PRTL_AVL_ALLOCATE_ROUTINE) (
    struct _RTL_AVL_TABLE *Table,
    CLONG ByteSize
);

typedef VOID
(NTAPI *PRTL_AVL_FREE_ROUTINE) (
    struct _RTL_AVL_TABLE *Table,
    PVOID Buffer
);

typedef struct _PUBLIC_BCB {
    CSHORT          NodeTypeCode;
    CSHORT          NodeByteSize;
    ULONG           MappedLength;
    LARGE_INTEGER   MappedFileOffset;
} PUBLIC_BCB, *PPUBLIC_BCB;

typedef struct _QUERY_PATH_REQUEST {
    ULONG                   PathNameLength;
    PIO_SECURITY_CONTEXT    SecurityContext;
    WCHAR                   FilePathName[1];
} QUERY_PATH_REQUEST, *PQUERY_PATH_REQUEST;

typedef struct _QUERY_PATH_RESPONSE {
    ULONG LengthAccepted;
} QUERY_PATH_RESPONSE, *PQUERY_PATH_RESPONSE;

typedef struct _RETRIEVAL_POINTERS_BUFFER {
    ULONG               ExtentCount;
    LARGE_INTEGER       StartingVcn;
    struct {
        LARGE_INTEGER   NextVcn;
        LARGE_INTEGER   Lcn;
    } Extents[1];
} RETRIEVAL_POINTERS_BUFFER, *PRETRIEVAL_POINTERS_BUFFER;

typedef struct _RTL_SPLAY_LINKS {
    struct _RTL_SPLAY_LINKS *Parent;
    struct _RTL_SPLAY_LINKS *LeftChild;
    struct _RTL_SPLAY_LINKS *RightChild;
} RTL_SPLAY_LINKS, *PRTL_SPLAY_LINKS;

typedef struct _RTL_BALANCED_LINKS
{
    struct _RTL_BALANCED_LINKS *Parent;
    struct _RTL_BALANCED_LINKS *LeftChild;
    struct _RTL_BALANCED_LINKS *RightChild;
    CHAR Balance;
    UCHAR Reserved[3];
} RTL_BALANCED_LINKS, *PRTL_BALANCED_LINKS;

typedef struct _RTL_GENERIC_TABLE
{
    PRTL_SPLAY_LINKS TableRoot;
    LIST_ENTRY InsertOrderList;
    PLIST_ENTRY OrderedPointer;
    ULONG WhichOrderedElement;
    ULONG NumberGenericTableElements;
    PRTL_GENERIC_COMPARE_ROUTINE CompareRoutine;
    PRTL_GENERIC_ALLOCATE_ROUTINE AllocateRoutine;
    PRTL_GENERIC_FREE_ROUTINE FreeRoutine;
    PVOID TableContext;
} RTL_GENERIC_TABLE, *PRTL_GENERIC_TABLE;

#undef PRTL_GENERIC_COMPARE_ROUTINE
#undef PRTL_GENERIC_ALLOCATE_ROUTINE
#undef PRTL_GENERIC_FREE_ROUTINE
#undef RTL_GENERIC_TABLE
#undef PRTL_GENERIC_TABLE

#define PRTL_GENERIC_COMPARE_ROUTINE PRTL_AVL_COMPARE_ROUTINE
#define PRTL_GENERIC_ALLOCATE_ROUTINE PRTL_AVL_ALLOCATE_ROUTINE
#define PRTL_GENERIC_FREE_ROUTINE PRTL_AVL_FREE_ROUTINE
#define RTL_GENERIC_TABLE RTL_AVL_TABLE
#define PRTL_GENERIC_TABLE PRTL_AVL_TABLE

#define RtlInitializeGenericTable               RtlInitializeGenericTableAvl
#define RtlInsertElementGenericTable            RtlInsertElementGenericTableAvl
#define RtlInsertElementGenericTableFull        RtlInsertElementGenericTableFullAvl
#define RtlDeleteElementGenericTable            RtlDeleteElementGenericTableAvl
#define RtlLookupElementGenericTable            RtlLookupElementGenericTableAvl
#define RtlLookupElementGenericTableFull        RtlLookupElementGenericTableFullAvl
#define RtlEnumerateGenericTable                RtlEnumerateGenericTableAvl
#define RtlEnumerateGenericTableWithoutSplaying RtlEnumerateGenericTableWithoutSplayingAvl
#define RtlGetElementGenericTable               RtlGetElementGenericTableAvl
#define RtlNumberGenericTableElements           RtlNumberGenericTableElementsAvl
#define RtlIsGenericTableEmpty                  RtlIsGenericTableEmptyAvl

typedef struct _RTL_AVL_TABLE
{
    RTL_BALANCED_LINKS BalancedRoot;
    PVOID OrderedPointer;
    ULONG WhichOrderedElement;
    ULONG NumberGenericTableElements;
    ULONG DepthOfTree;
    PRTL_BALANCED_LINKS RestartKey;
    ULONG DeleteCount;
    PRTL_AVL_COMPARE_ROUTINE CompareRoutine;
    PRTL_AVL_ALLOCATE_ROUTINE AllocateRoutine;
    PRTL_AVL_FREE_ROUTINE FreeRoutine;
    PVOID TableContext;
} RTL_AVL_TABLE, *PRTL_AVL_TABLE;

NTSYSAPI
VOID
NTAPI
RtlInitializeGenericTableAvl(
    PRTL_AVL_TABLE Table,
    PRTL_AVL_COMPARE_ROUTINE CompareRoutine,
    PRTL_AVL_ALLOCATE_ROUTINE AllocateRoutine,
    PRTL_AVL_FREE_ROUTINE FreeRoutine,
    PVOID TableContext
);

NTSYSAPI
PVOID
NTAPI
RtlInsertElementGenericTableAvl (
    PRTL_AVL_TABLE Table,
    PVOID Buffer,
    CLONG BufferSize,
    PBOOLEAN NewElement OPTIONAL
    );
    
NTSYSAPI
BOOLEAN
NTAPI
RtlDeleteElementGenericTableAvl (
    PRTL_AVL_TABLE Table,
    PVOID Buffer
    );
    
NTSYSAPI
PVOID
NTAPI
RtlLookupElementGenericTableAvl (
    PRTL_AVL_TABLE Table,
    PVOID Buffer
    );
    
NTSYSAPI
PVOID
NTAPI
RtlEnumerateGenericTableWithoutSplayingAvl (
    PRTL_AVL_TABLE Table,
    PVOID *RestartKey
    );

#if defined(USE_LPC6432)
#define LPC_CLIENT_ID CLIENT_ID64
#define LPC_SIZE_T ULONGLONG
#define LPC_PVOID ULONGLONG
#define LPC_HANDLE ULONGLONG
#else
#define LPC_CLIENT_ID CLIENT_ID
#define LPC_SIZE_T SIZE_T
#define LPC_PVOID PVOID
#define LPC_HANDLE HANDLE
#endif

typedef struct _PORT_MESSAGE
{
    union
    {
        struct
        {
            CSHORT DataLength;
            CSHORT TotalLength;
        } s1;
        ULONG Length;
    } u1;
    union
    {
        struct
        {
            CSHORT Type;
            CSHORT DataInfoOffset;
        } s2;
        ULONG ZeroInit;
    } u2;
    __GNU_EXTENSION union
    {
        LPC_CLIENT_ID ClientId;
        double DoNotUseThisField;
    };
    ULONG MessageId;
    __GNU_EXTENSION union
    {
        LPC_SIZE_T ClientViewSize;
        ULONG CallbackId;
    };
} PORT_MESSAGE, *PPORT_MESSAGE;

#define LPC_KERNELMODE_MESSAGE      (CSHORT)((USHORT)0x8000)

typedef struct _PORT_VIEW
{
    ULONG Length;
    LPC_HANDLE SectionHandle;
    ULONG SectionOffset;
    LPC_SIZE_T ViewSize;
    LPC_PVOID ViewBase;
    LPC_PVOID ViewRemoteBase;
} PORT_VIEW, *PPORT_VIEW;

typedef struct _REMOTE_PORT_VIEW
{
    ULONG Length;
    LPC_SIZE_T ViewSize;
    LPC_PVOID ViewBase;
} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;

typedef struct _SE_EXPORTS {

    LUID    SeCreateTokenPrivilege;
    LUID    SeAssignPrimaryTokenPrivilege;
    LUID    SeLockMemoryPrivilege;
    LUID    SeIncreaseQuotaPrivilege;
    LUID    SeUnsolicitedInputPrivilege;
    LUID    SeTcbPrivilege;
    LUID    SeSecurityPrivilege;
    LUID    SeTakeOwnershipPrivilege;
    LUID    SeLoadDriverPrivilege;
    LUID    SeCreatePagefilePrivilege;
    LUID    SeIncreaseBasePriorityPrivilege;
    LUID    SeSystemProfilePrivilege;
    LUID    SeSystemtimePrivilege;
    LUID    SeProfileSingleProcessPrivilege;
    LUID    SeCreatePermanentPrivilege;
    LUID    SeBackupPrivilege;
    LUID    SeRestorePrivilege;
    LUID    SeShutdownPrivilege;
    LUID    SeDebugPrivilege;
    LUID    SeAuditPrivilege;
    LUID    SeSystemEnvironmentPrivilege;
    LUID    SeChangeNotifyPrivilege;
    LUID    SeRemoteShutdownPrivilege;

    PSID    SeNullSid;
    PSID    SeWorldSid;
    PSID    SeLocalSid;
    PSID    SeCreatorOwnerSid;
    PSID    SeCreatorGroupSid;

    PSID    SeNtAuthoritySid;
    PSID    SeDialupSid;
    PSID    SeNetworkSid;
    PSID    SeBatchSid;
    PSID    SeInteractiveSid;
    PSID    SeLocalSystemSid;
    PSID    SeAliasAdminsSid;
    PSID    SeAliasUsersSid;
    PSID    SeAliasGuestsSid;
    PSID    SeAliasPowerUsersSid;
    PSID    SeAliasAccountOpsSid;
    PSID    SeAliasSystemOpsSid;
    PSID    SeAliasPrintOpsSid;
    PSID    SeAliasBackupOpsSid;

    PSID    SeAuthenticatedUsersSid;

    PSID    SeRestrictedSid;
    PSID    SeAnonymousLogonSid;

    LUID    SeUndockPrivilege;
    LUID    SeSyncAgentPrivilege;
    LUID    SeEnableDelegationPrivilege;

} SE_EXPORTS, *PSE_EXPORTS;

extern PSE_EXPORTS SeExports;

typedef struct
{
  LARGE_INTEGER StartingLcn;
} STARTING_LCN_INPUT_BUFFER, *PSTARTING_LCN_INPUT_BUFFER;

typedef struct _STARTING_VCN_INPUT_BUFFER {
    LARGE_INTEGER StartingVcn;
} STARTING_VCN_INPUT_BUFFER, *PSTARTING_VCN_INPUT_BUFFER;

typedef struct _SECURITY_CLIENT_CONTEXT {
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_TOKEN               ClientToken;
    BOOLEAN                     DirectlyAccessClientToken;
    BOOLEAN                     DirectAccessEffectiveOnly;
    BOOLEAN                     ServerIsRemote;
    TOKEN_CONTROL               ClientTokenControl;
} SECURITY_CLIENT_CONTEXT, *PSECURITY_CLIENT_CONTEXT;

typedef struct _TUNNEL {
    FAST_MUTEX          Mutex;
    PRTL_SPLAY_LINKS    Cache;
    LIST_ENTRY          TimerQueue;
    USHORT              NumEntries;
} TUNNEL, *PTUNNEL;

typedef struct _VAD_HEADER {
    PVOID       StartVPN;
    PVOID       EndVPN;
    struct _VAD_HEADER* ParentLink;
    struct _VAD_HEADER* LeftLink;
    struct _VAD_HEADER* RightLink;
    ULONG       Flags;          /* LSB = CommitCharge */
    PVOID       ControlArea;
    PVOID       FirstProtoPte;
    PVOID       LastPTE;
    ULONG       Unknown;
    LIST_ENTRY  Secured;
} VAD_HEADER, *PVAD_HEADER;

typedef struct
{
  LARGE_INTEGER StartingLcn;
  LARGE_INTEGER BitmapSize;
  UCHAR Buffer[1];
} VOLUME_BITMAP_BUFFER, *PVOLUME_BITMAP_BUFFER;

#if (VER_PRODUCTBUILD >= 2600)

typedef BOOLEAN
(NTAPI *PFILTER_REPORT_CHANGE) (
    IN PVOID  NotifyContext,
    IN PVOID  FilterContext
);

typedef enum _FS_FILTER_SECTION_SYNC_TYPE {
    SyncTypeOther = 0,
    SyncTypeCreateSection
} FS_FILTER_SECTION_SYNC_TYPE, *PFS_FILTER_SECTION_SYNC_TYPE;

typedef enum _FS_FILTER_STREAM_FO_NOTIFICATION_TYPE {
    NotifyTypeCreate = 0,
    NotifyTypeRetired
} FS_FILTER_STREAM_FO_NOTIFICATION_TYPE, *PFS_FILTER_STREAM_FO_NOTIFICATION_TYPE;

typedef union _FS_FILTER_PARAMETERS {
    struct {
        PLARGE_INTEGER  EndingOffset;
        PERESOURCE *ResourceToRelease;
    } AcquireForModifiedPageWriter;

    struct {
        PERESOURCE  ResourceToRelease;
    } ReleaseForModifiedPageWriter;

    struct {
        FS_FILTER_SECTION_SYNC_TYPE  SyncType;
        ULONG  PageProtection;
    } AcquireForSectionSynchronization;

    struct {
        FS_FILTER_STREAM_FO_NOTIFICATION_TYPE NotificationType;
        BOOLEAN POINTER_ALIGNMENT SafeToRecurse;
    } NotifyStreamFileObject;

    struct {
        PVOID  Argument1;
        PVOID  Argument2;
        PVOID  Argument3;
        PVOID  Argument4;
        PVOID  Argument5;
    } Others;
} FS_FILTER_PARAMETERS, *PFS_FILTER_PARAMETERS;

typedef struct _FS_FILTER_CALLBACK_DATA {
    ULONG                  SizeOfFsFilterCallbackData;
    UCHAR                  Operation;
    UCHAR                  Reserved;
    struct _DEVICE_OBJECT  *DeviceObject;
    struct _FILE_OBJECT    *FileObject;
    FS_FILTER_PARAMETERS   Parameters;
} FS_FILTER_CALLBACK_DATA, *PFS_FILTER_CALLBACK_DATA;

typedef NTSTATUS
(NTAPI *PFS_FILTER_CALLBACK) (
    IN PFS_FILTER_CALLBACK_DATA  Data,
    OUT PVOID                    *CompletionContext
);

typedef VOID
(NTAPI *PFS_FILTER_COMPLETION_CALLBACK) (
    IN PFS_FILTER_CALLBACK_DATA  Data,
    IN NTSTATUS                  OperationStatus,
    IN PVOID                     CompletionContext
);

typedef struct _FS_FILTER_CALLBACKS {
    ULONG                           SizeOfFsFilterCallbacks;
    ULONG                           Reserved;
    PFS_FILTER_CALLBACK             PreAcquireForSectionSynchronization;
    PFS_FILTER_COMPLETION_CALLBACK  PostAcquireForSectionSynchronization;
    PFS_FILTER_CALLBACK             PreReleaseForSectionSynchronization;
    PFS_FILTER_COMPLETION_CALLBACK  PostReleaseForSectionSynchronization;
    PFS_FILTER_CALLBACK             PreAcquireForCcFlush;
    PFS_FILTER_COMPLETION_CALLBACK  PostAcquireForCcFlush;
    PFS_FILTER_CALLBACK             PreReleaseForCcFlush;
    PFS_FILTER_COMPLETION_CALLBACK  PostReleaseForCcFlush;
    PFS_FILTER_CALLBACK             PreAcquireForModifiedPageWriter;
    PFS_FILTER_COMPLETION_CALLBACK  PostAcquireForModifiedPageWriter;
    PFS_FILTER_CALLBACK             PreReleaseForModifiedPageWriter;
    PFS_FILTER_COMPLETION_CALLBACK  PostReleaseForModifiedPageWriter;
} FS_FILTER_CALLBACKS, *PFS_FILTER_CALLBACKS;

typedef struct _READ_LIST {
    PFILE_OBJECT          FileObject;
    ULONG                 NumberOfEntries;
    LOGICAL               IsImage;
    FILE_SEGMENT_ELEMENT  List[ANYSIZE_ARRAY];
} READ_LIST, *PREAD_LIST;

#endif

NTKERNELAPI
BOOLEAN
NTAPI
CcCanIWrite (
    IN PFILE_OBJECT FileObject,
    IN ULONG        BytesToWrite,
    IN BOOLEAN      Wait,
    IN BOOLEAN      Retrying
);

NTKERNELAPI
BOOLEAN
NTAPI
CcCopyRead (
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN ULONG                Length,
    IN BOOLEAN              Wait,
    OUT PVOID               Buffer,
    OUT PIO_STATUS_BLOCK    IoStatus
);

NTKERNELAPI
BOOLEAN
NTAPI
CcCopyWrite (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN BOOLEAN          Wait,
    IN PVOID            Buffer
);

#define CcCopyWriteWontFlush(FO, FOFF, LEN) ((LEN) <= 0x10000)

typedef VOID (NTAPI *PCC_POST_DEFERRED_WRITE) (
    IN PVOID Context1,
    IN PVOID Context2
);

NTKERNELAPI
VOID
NTAPI
CcDeferWrite (
    IN PFILE_OBJECT             FileObject,
    IN PCC_POST_DEFERRED_WRITE  PostRoutine,
    IN PVOID                    Context1,
    IN PVOID                    Context2,
    IN ULONG                    BytesToWrite,
    IN BOOLEAN                  Retrying
);

NTKERNELAPI
VOID
NTAPI
CcFastCopyRead (
    IN PFILE_OBJECT         FileObject,
    IN ULONG                FileOffset,
    IN ULONG                Length,
    IN ULONG                PageCount,
    OUT PVOID               Buffer,
    OUT PIO_STATUS_BLOCK    IoStatus
);

NTKERNELAPI
VOID
NTAPI
CcFastCopyWrite (
    IN PFILE_OBJECT FileObject,
    IN ULONG        FileOffset,
    IN ULONG        Length,
    IN PVOID        Buffer
);

NTKERNELAPI
VOID
NTAPI
CcFlushCache (
    IN PSECTION_OBJECT_POINTERS SectionObjectPointer,
    IN PLARGE_INTEGER           FileOffset OPTIONAL,
    IN ULONG                    Length,
    OUT PIO_STATUS_BLOCK        IoStatus OPTIONAL
);

typedef VOID (NTAPI *PDIRTY_PAGE_ROUTINE) (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN PLARGE_INTEGER   OldestLsn,
    IN PLARGE_INTEGER   NewestLsn,
    IN PVOID            Context1,
    IN PVOID            Context2
);

NTKERNELAPI
LARGE_INTEGER
NTAPI
CcGetDirtyPages (
    IN PVOID                LogHandle,
    IN PDIRTY_PAGE_ROUTINE  DirtyPageRoutine,
    IN PVOID                Context1,
    IN PVOID                Context2
);

NTKERNELAPI
PFILE_OBJECT
NTAPI
CcGetFileObjectFromBcb (
    IN PVOID Bcb
);

NTKERNELAPI
PFILE_OBJECT
NTAPI
CcGetFileObjectFromSectionPtrs (
    IN PSECTION_OBJECT_POINTERS SectionObjectPointer
);

#define CcGetFileSizePointer(FO) (                                     \
    ((PLARGE_INTEGER)((FO)->SectionObjectPointer->SharedCacheMap) + 1) \
)

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
LARGE_INTEGER
NTAPI
CcGetFlushedValidData (
    IN PSECTION_OBJECT_POINTERS SectionObjectPointer,
    IN BOOLEAN                  BcbListHeld
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
LARGE_INTEGER
NTAPI
CcGetLsnForFileObject (
    IN PFILE_OBJECT     FileObject,
    OUT PLARGE_INTEGER  OldestLsn OPTIONAL
);

typedef BOOLEAN (NTAPI *PACQUIRE_FOR_LAZY_WRITE) (
    IN PVOID    Context,
    IN BOOLEAN  Wait
);

typedef VOID (NTAPI *PRELEASE_FROM_LAZY_WRITE) (
    IN PVOID Context
);

typedef BOOLEAN (NTAPI *PACQUIRE_FOR_READ_AHEAD) (
    IN PVOID    Context,
    IN BOOLEAN  Wait
);

typedef VOID (NTAPI *PRELEASE_FROM_READ_AHEAD) (
    IN PVOID Context
);

typedef struct _CACHE_MANAGER_CALLBACKS {
    PACQUIRE_FOR_LAZY_WRITE     AcquireForLazyWrite;
    PRELEASE_FROM_LAZY_WRITE    ReleaseFromLazyWrite;
    PACQUIRE_FOR_READ_AHEAD     AcquireForReadAhead;
    PRELEASE_FROM_READ_AHEAD    ReleaseFromReadAhead;
} CACHE_MANAGER_CALLBACKS, *PCACHE_MANAGER_CALLBACKS;

NTKERNELAPI
VOID
NTAPI
CcInitializeCacheMap (
    IN PFILE_OBJECT             FileObject,
    IN PCC_FILE_SIZES           FileSizes,
    IN BOOLEAN                  PinAccess,
    IN PCACHE_MANAGER_CALLBACKS Callbacks,
    IN PVOID                    LazyWriteContext
);

#define CcIsFileCached(FO) (                                                         \
    ((FO)->SectionObjectPointer != NULL) &&                                          \
    (((PSECTION_OBJECT_POINTERS)(FO)->SectionObjectPointer)->SharedCacheMap != NULL) \
)

extern ULONG CcFastMdlReadWait;

NTKERNELAPI
BOOLEAN
NTAPI
CcIsThereDirtyData (
    IN PVPB Vpb
);

NTKERNELAPI
BOOLEAN
NTAPI
CcMapData (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN ULONG            Flags,
    OUT PVOID           *Bcb,
    OUT PVOID           *Buffer
);

NTKERNELAPI
VOID
NTAPI
CcMdlRead (
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN ULONG                Length,
    OUT PMDL                *MdlChain,
    OUT PIO_STATUS_BLOCK    IoStatus
);

NTKERNELAPI
VOID
NTAPI
CcMdlReadComplete (
    IN PFILE_OBJECT FileObject,
    IN PMDL         MdlChain
);

NTKERNELAPI
VOID
NTAPI
CcMdlWriteComplete (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN PMDL             MdlChain
);

#define MAP_WAIT        1

NTKERNELAPI
BOOLEAN
NTAPI
CcPinMappedData (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN ULONG            Flags,
    IN OUT PVOID        *Bcb
);

NTKERNELAPI
BOOLEAN
NTAPI
CcPinRead (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN ULONG            Flags,
    OUT PVOID           *Bcb,
    OUT PVOID           *Buffer
);

NTKERNELAPI
VOID
NTAPI
CcPrepareMdlWrite (
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN ULONG                Length,
    OUT PMDL                *MdlChain,
    OUT PIO_STATUS_BLOCK    IoStatus
);

NTKERNELAPI
BOOLEAN
NTAPI
CcPreparePinWrite (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length,
    IN BOOLEAN          Zero,
    IN ULONG            Flags,
    OUT PVOID           *Bcb,
    OUT PVOID           *Buffer
);

NTKERNELAPI
BOOLEAN
NTAPI
CcPurgeCacheSection (
    IN PSECTION_OBJECT_POINTERS SectionObjectPointer,
    IN PLARGE_INTEGER           FileOffset OPTIONAL,
    IN ULONG                    Length,
    IN BOOLEAN                  UninitializeCacheMaps
);

#define CcReadAhead(FO, FOFF, LEN) (                \
    if ((LEN) >= 256) {                             \
        CcScheduleReadAhead((FO), (FOFF), (LEN));   \
    }                                               \
)

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
PVOID
NTAPI
CcRemapBcb (
    IN PVOID Bcb
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
VOID
NTAPI
CcRepinBcb (
    IN PVOID Bcb
);

NTKERNELAPI
VOID
NTAPI
CcScheduleReadAhead (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN ULONG            Length
);

NTKERNELAPI
VOID
NTAPI
CcSetAdditionalCacheAttributes (
    IN PFILE_OBJECT FileObject,
    IN BOOLEAN      DisableReadAhead,
    IN BOOLEAN      DisableWriteBehind
);

NTKERNELAPI
VOID
NTAPI
CcSetBcbOwnerPointer (
    IN PVOID Bcb,
    IN PVOID OwnerPointer
);

NTKERNELAPI
VOID
NTAPI
CcSetDirtyPageThreshold (
    IN PFILE_OBJECT FileObject,
    IN ULONG        DirtyPageThreshold
);

NTKERNELAPI
VOID
NTAPI
CcSetDirtyPinnedData (
    IN PVOID            BcbVoid,
    IN PLARGE_INTEGER   Lsn OPTIONAL
);

NTKERNELAPI
VOID
NTAPI
CcSetFileSizes (
    IN PFILE_OBJECT     FileObject,
    IN PCC_FILE_SIZES   FileSizes
);

typedef VOID (NTAPI *PFLUSH_TO_LSN) (
    IN PVOID            LogHandle,
    IN LARGE_INTEGER    Lsn
);

NTKERNELAPI
VOID
NTAPI
CcSetLogHandleForFile (
    IN PFILE_OBJECT     FileObject,
    IN PVOID            LogHandle,
    IN PFLUSH_TO_LSN    FlushToLsnRoutine
);

NTKERNELAPI
VOID
NTAPI
CcSetReadAheadGranularity (
    IN PFILE_OBJECT FileObject,
    IN ULONG        Granularity     /* default: PAGE_SIZE */
                                    /* allowed: 2^n * PAGE_SIZE */
);

NTKERNELAPI
BOOLEAN
NTAPI
CcUninitializeCacheMap (
    IN PFILE_OBJECT                 FileObject,
    IN PLARGE_INTEGER               TruncateSize OPTIONAL,
    IN PCACHE_UNINITIALIZE_EVENT    UninitializeCompleteEvent OPTIONAL
);

NTKERNELAPI
VOID
NTAPI
CcUnpinData (
    IN PVOID Bcb
);

NTKERNELAPI
VOID
NTAPI
CcUnpinDataForThread (
    IN PVOID            Bcb,
    IN ERESOURCE_THREAD ResourceThreadId
);

NTKERNELAPI
VOID
NTAPI
CcUnpinRepinnedBcb (
    IN PVOID                Bcb,
    IN BOOLEAN              WriteThrough,
    OUT PIO_STATUS_BLOCK    IoStatus
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
CcWaitForCurrentLazyWriterActivity (
    VOID
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
BOOLEAN
NTAPI
CcZeroData (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   StartOffset,
    IN PLARGE_INTEGER   EndOffset,
    IN BOOLEAN          Wait
);

NTKERNELAPI
VOID
NTAPI
ExDisableResourceBoostLite (
    IN PERESOURCE  Resource
);

NTKERNELAPI
SIZE_T
NTAPI
ExQueryPoolBlockSize (
    IN PVOID      PoolBlock,
    OUT PBOOLEAN  QuotaCharged
);

#if (VER_PRODUCTBUILD >= 2600)

#ifndef __NTOSKRNL__
NTKERNELAPI
VOID
FASTCALL
ExInitializeRundownProtection (
    IN PEX_RUNDOWN_REF  RunRef
);

NTKERNELAPI
VOID
FASTCALL
ExReInitializeRundownProtection (
    IN PEX_RUNDOWN_REF  RunRef
);

NTKERNELAPI
BOOLEAN
FASTCALL
ExAcquireRundownProtection (
    IN PEX_RUNDOWN_REF  RunRef
);

NTKERNELAPI
BOOLEAN
FASTCALL
ExAcquireRundownProtectionEx (
    IN PEX_RUNDOWN_REF  RunRef,
    IN ULONG            Count
);

NTKERNELAPI
VOID
FASTCALL
ExReleaseRundownProtection (
    IN PEX_RUNDOWN_REF  RunRef
);

NTKERNELAPI
VOID
FASTCALL
ExReleaseRundownProtectionEx (
    IN PEX_RUNDOWN_REF  RunRef,
    IN ULONG            Count
);

NTKERNELAPI
VOID
FASTCALL
ExRundownCompleted (
    IN PEX_RUNDOWN_REF  RunRef
);

NTKERNELAPI
VOID
FASTCALL
ExWaitForRundownProtectionRelease (
    IN PEX_RUNDOWN_REF  RunRef
);

#endif
#endif /* (VER_PRODUCTBUILD >= 2600) */


#define FsRtlSetupAdvancedHeader( _advhdr, _fmutx )                         \
{                                                                           \
    SetFlag( (_advhdr)->Flags, FSRTL_FLAG_ADVANCED_HEADER );                \
    SetFlag( (_advhdr)->Flags2, FSRTL_FLAG2_SUPPORTS_FILTER_CONTEXTS );     \
    (_advhdr)->Version = FSRTL_FCB_HEADER_V1;                               \
    InitializeListHead( &(_advhdr)->FilterContexts );                       \
    if ((_fmutx) != NULL) {                                                 \
        (_advhdr)->FastMutex = (_fmutx);                                    \
    }                                                                       \
    *((PULONG_PTR)(&(_advhdr)->PushLock)) = 0;                              \
    /*ExInitializePushLock( &(_advhdr)->PushLock ); API Not avaliable downlevel*/\
    (_advhdr)->FileContextSupportPointer = NULL;                            \
}

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlAddBaseMcbEntry (
    IN PBASE_MCB  Mcb,
    IN LONGLONG   Vbn,
    IN LONGLONG   Lbn,
    IN LONGLONG   SectorCount
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlAddLargeMcbEntry (
    IN PLARGE_MCB  Mcb,
    IN LONGLONG    Vbn,
    IN LONGLONG    Lbn,
    IN LONGLONG    SectorCount
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlAddMcbEntry (
    IN PMCB   Mcb,
    IN VBN    Vbn,
    IN LBN    Lbn,
    IN ULONG  SectorCount
);

NTKERNELAPI
VOID
NTAPI
FsRtlAddToTunnelCache (
    IN PTUNNEL          Cache,
    IN ULONGLONG        DirectoryKey,
    IN PUNICODE_STRING  ShortName,
    IN PUNICODE_STRING  LongName,
    IN BOOLEAN          KeyByShortName,
    IN ULONG            DataLength,
    IN PVOID            Data
);

#if (VER_PRODUCTBUILD >= 2195)

PFILE_LOCK
NTAPI
FsRtlAllocateFileLock (
    IN PCOMPLETE_LOCK_IRP_ROUTINE   CompleteLockIrpRoutine OPTIONAL,
    IN PUNLOCK_ROUTINE              UnlockRoutine OPTIONAL
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
PVOID
NTAPI
FsRtlAllocatePool (
    IN POOL_TYPE    PoolType,
    IN ULONG        NumberOfBytes
);

NTKERNELAPI
PVOID
NTAPI
FsRtlAllocatePoolWithQuota (
    IN POOL_TYPE    PoolType,
    IN ULONG        NumberOfBytes
);

NTKERNELAPI
PVOID
NTAPI
FsRtlAllocatePoolWithQuotaTag (
    IN POOL_TYPE    PoolType,
    IN ULONG        NumberOfBytes,
    IN ULONG        Tag
);

NTKERNELAPI
PVOID
NTAPI
FsRtlAllocatePoolWithTag (
    IN POOL_TYPE    PoolType,
    IN ULONG        NumberOfBytes,
    IN ULONG        Tag
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlAreNamesEqual (
    IN PCUNICODE_STRING  Name1,
    IN PCUNICODE_STRING  Name2,
    IN BOOLEAN           IgnoreCase,
    IN PCWCH             UpcaseTable OPTIONAL
);

#define FsRtlAreThereCurrentFileLocks(FL) ( \
    ((FL)->FastIoIsQuestionable)            \
)

/*
  FsRtlCheckLockForReadAccess:

  All this really does is pick out the lock parameters from the irp (io stack
  location?), get IoGetRequestorProcess, and pass values on to
  FsRtlFastCheckLockForRead.
*/
NTKERNELAPI
BOOLEAN
NTAPI
FsRtlCheckLockForReadAccess (
    IN PFILE_LOCK   FileLock,
    IN PIRP         Irp
);

/*
  FsRtlCheckLockForWriteAccess:

  All this really does is pick out the lock parameters from the irp (io stack
  location?), get IoGetRequestorProcess, and pass values on to
  FsRtlFastCheckLockForWrite.
*/
NTKERNELAPI
BOOLEAN
NTAPI
FsRtlCheckLockForWriteAccess (
    IN PFILE_LOCK   FileLock,
    IN PIRP         Irp
);

typedef
VOID
(NTAPI*POPLOCK_WAIT_COMPLETE_ROUTINE) (
    IN PVOID    Context,
    IN PIRP     Irp
);

typedef
VOID
(NTAPI*POPLOCK_FS_PREPOST_IRP) (
    IN PVOID    Context,
    IN PIRP     Irp
);

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlCheckOplock (
    IN POPLOCK                          Oplock,
    IN PIRP                             Irp,
    IN PVOID                            Context,
    IN POPLOCK_WAIT_COMPLETE_ROUTINE    CompletionRoutine OPTIONAL,
    IN POPLOCK_FS_PREPOST_IRP           PostIrpRoutine OPTIONAL
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlCopyRead (
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN ULONG                Length,
    IN BOOLEAN              Wait,
    IN ULONG                LockKey,
    OUT PVOID               Buffer,
    OUT PIO_STATUS_BLOCK    IoStatus,
    IN PDEVICE_OBJECT       DeviceObject
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlCopyWrite (
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN ULONG                Length,
    IN BOOLEAN              Wait,
    IN ULONG                LockKey,
    IN PVOID                Buffer,
    OUT PIO_STATUS_BLOCK    IoStatus,
    IN PDEVICE_OBJECT       DeviceObject
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlCurrentBatchOplock (
    IN POPLOCK Oplock
);

NTKERNELAPI
VOID
NTAPI
FsRtlDeleteKeyFromTunnelCache (
    IN PTUNNEL      Cache,
    IN ULONGLONG    DirectoryKey
);

NTKERNELAPI
VOID
NTAPI
FsRtlDeleteTunnelCache (
    IN PTUNNEL Cache
);

NTKERNELAPI
VOID
NTAPI
FsRtlDeregisterUncProvider (
    IN HANDLE Handle
);

NTKERNELAPI
VOID
NTAPI
FsRtlDissectDbcs (
    IN ANSI_STRING    Name,
    OUT PANSI_STRING  FirstPart,
    OUT PANSI_STRING  RemainingPart
);

NTKERNELAPI
VOID
NTAPI
FsRtlDissectName (
    IN UNICODE_STRING    Name,
    OUT PUNICODE_STRING  FirstPart,
    OUT PUNICODE_STRING  RemainingPart
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlDoesDbcsContainWildCards (
    IN PANSI_STRING Name
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlDoesNameContainWildCards (
    IN PUNICODE_STRING Name
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlIsFatDbcsLegal (
    IN ANSI_STRING DbcsName,
    IN BOOLEAN WildCardsPermissible,
    IN BOOLEAN PathNamePermissible,
    IN BOOLEAN LeadingBackslashPermissible
    );


#define FsRtlCompleteRequest(IRP,STATUS) {         \
    (IRP)->IoStatus.Status = (STATUS);             \
    IoCompleteRequest( (IRP), IO_DISK_INCREMENT ); \
}

#define FsRtlEnterFileSystem    KeEnterCriticalRegion

#define FsRtlExitFileSystem     KeLeaveCriticalRegion

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlFastCheckLockForRead (
    IN PFILE_LOCK           FileLock,
    IN PLARGE_INTEGER       FileOffset,
    IN PLARGE_INTEGER       Length,
    IN ULONG                Key,
    IN PFILE_OBJECT         FileObject,
    IN PVOID                Process
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlFastCheckLockForWrite (
    IN PFILE_LOCK           FileLock,
    IN PLARGE_INTEGER       FileOffset,
    IN PLARGE_INTEGER       Length,
    IN ULONG                Key,
    IN PFILE_OBJECT         FileObject,
    IN PVOID                Process
);

#define FsRtlFastLock(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) (       \
     FsRtlPrivateLock(A1, A2, A3, A4, A5, A6, A7, A8, A9, NULL, A10, A11)   \
)

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlFastUnlockAll (
    IN PFILE_LOCK           FileLock,
    IN PFILE_OBJECT         FileObject,
    IN PEPROCESS            Process,
    IN PVOID                Context OPTIONAL
);
/* ret: STATUS_RANGE_NOT_LOCKED */

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlFastUnlockAllByKey (
    IN PFILE_LOCK           FileLock,
    IN PFILE_OBJECT         FileObject,
    IN PEPROCESS            Process,
    IN ULONG                Key,
    IN PVOID                Context OPTIONAL
);
/* ret: STATUS_RANGE_NOT_LOCKED */

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlFastUnlockSingle (
    IN PFILE_LOCK           FileLock,
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN PLARGE_INTEGER       Length,
    IN PEPROCESS            Process,
    IN ULONG                Key,
    IN PVOID                Context OPTIONAL,
    IN BOOLEAN              AlreadySynchronized
);
/* ret:  STATUS_RANGE_NOT_LOCKED */

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlFindInTunnelCache (
    IN PTUNNEL          Cache,
    IN ULONGLONG        DirectoryKey,
    IN PUNICODE_STRING  Name,
    OUT PUNICODE_STRING ShortName,
    OUT PUNICODE_STRING LongName,
    IN OUT PULONG       DataLength,
    OUT PVOID           Data
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
VOID
NTAPI
FsRtlFreeFileLock (
    IN PFILE_LOCK FileLock
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlGetFileSize (
    IN PFILE_OBJECT         FileObject,
    IN OUT PLARGE_INTEGER   FileSize
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlGetNextBaseMcbEntry (
    IN PBASE_MCB   Mcb,
    IN ULONG       RunIndex,
    OUT PLONGLONG  Vbn,
    OUT PLONGLONG  Lbn,
    OUT PLONGLONG  SectorCount
);

/*
  FsRtlGetNextFileLock:

  ret: NULL if no more locks

  Internals:
    FsRtlGetNextFileLock uses FileLock->LastReturnedLockInfo and
    FileLock->LastReturnedLock as storage.
    LastReturnedLock is a pointer to the 'raw' lock inkl. double linked
    list, and FsRtlGetNextFileLock needs this to get next lock on subsequent
    calls with Restart = FALSE.
*/
NTKERNELAPI
PFILE_LOCK_INFO
NTAPI
FsRtlGetNextFileLock (
    IN PFILE_LOCK   FileLock,
    IN BOOLEAN      Restart
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlGetNextLargeMcbEntry (
    IN PLARGE_MCB  Mcb,
    IN ULONG       RunIndex,
    OUT PLONGLONG  Vbn,
    OUT PLONGLONG  Lbn,
    OUT PLONGLONG  SectorCount
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlGetNextMcbEntry (
    IN PMCB     Mcb,
    IN ULONG    RunIndex,
    OUT PVBN    Vbn,
    OUT PLBN    Lbn,
    OUT PULONG  SectorCount
);

#define FsRtlGetPerStreamContextPointer(FO) (   \
    (PFSRTL_ADVANCED_FCB_HEADER)(FO)->FsContext \
)

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeBaseMcb (
    IN PBASE_MCB  Mcb,
    IN POOL_TYPE  PoolType
);

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeFileLock (
    IN PFILE_LOCK                   FileLock,
    IN PCOMPLETE_LOCK_IRP_ROUTINE   CompleteLockIrpRoutine OPTIONAL,
    IN PUNLOCK_ROUTINE              UnlockRoutine OPTIONAL
);

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeLargeMcb (
    IN PLARGE_MCB  Mcb,
    IN POOL_TYPE   PoolType
);

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeMcb (
    IN PMCB       Mcb,
    IN POOL_TYPE  PoolType
);

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeOplock (
    IN OUT POPLOCK Oplock
);

NTKERNELAPI
VOID
NTAPI
FsRtlInitializeTunnelCache (
    IN PTUNNEL Cache
);

#define FsRtlInitPerStreamContext(PSC, O, I, FC) ( \
    (PSC)->OwnerId = (O),                          \
    (PSC)->InstanceId = (I),                       \
    (PSC)->FreeCallback = (FC)                     \
)

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlInsertPerStreamContext (
    IN PFSRTL_ADVANCED_FCB_HEADER  PerStreamContext,
    IN PFSRTL_PER_STREAM_CONTEXT   Ptr
);

#define FsRtlIsAnsiCharacterLegalFat(C, WILD) (                                \
    FlagOn(FsRtlLegalAnsiCharacterArray[(UCHAR)(C)], (FSRTL_FAT_LEGAL) |       \
                                        ((WILD) ? FSRTL_WILD_CHARACTER : 0 ))  \
)

#define FsRtlIsAnsiCharacterLegalHpfs(C, WILD) (                               \
    FlagOn(FsRtlLegalAnsiCharacterArray[(UCHAR)(C)], (FSRTL_HPFS_LEGAL) |      \
                                        ((WILD) ? FSRTL_WILD_CHARACTER : 0 ))  \
)

#define FsRtlIsAnsiCharacterLegalNtfs(C, WILD) (                               \
    FlagOn(FsRtlLegalAnsiCharacterArray[(UCHAR)(C)], (FSRTL_NTFS_LEGAL) |      \
                                        ((WILD) ? FSRTL_WILD_CHARACTER : 0 ))  \
)

#define FsRtlIsAnsiCharacterWild(C) (                                       \
    FlagOn(FsRtlLegalAnsiCharacterArray[(UCHAR)(C)], FSRTL_WILD_CHARACTER ) \
)

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlIsFatDbcsLegal (
    IN ANSI_STRING  DbcsName,
    IN BOOLEAN      WildCardsPermissible,
    IN BOOLEAN      PathNamePermissible,
    IN BOOLEAN      LeadingBackslashPermissible
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlIsHpfsDbcsLegal (
    IN ANSI_STRING  DbcsName,
    IN BOOLEAN      WildCardsPermissible,
    IN BOOLEAN      PathNamePermissible,
    IN BOOLEAN      LeadingBackslashPermissible
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlIsNameInExpression (
    IN PUNICODE_STRING  Expression,
    IN PUNICODE_STRING  Name,
    IN BOOLEAN          IgnoreCase,
    IN PWCHAR           UpcaseTable OPTIONAL
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlIsNtstatusExpected (
    IN NTSTATUS Ntstatus
);

#define NLS_OEM_LEAD_BYTE_INFO NlsOemLeadByteInfo

extern PUSHORT NlsOemLeadByteInfo;

#define FsRtlIsLeadDbcsCharacter(DBCS_CHAR) (                               \
    (BOOLEAN)((UCHAR)(DBCS_CHAR) < 0x80 ? FALSE :                           \
              (NLS_MB_CODE_PAGE_TAG &&                                      \
               (NLS_OEM_LEAD_BYTE_INFO[(UCHAR)(DBCS_CHAR)] != 0)))          \
)

#define FsRtlIsUnicodeCharacterWild(C) (                                    \
    (((C) >= 0x40) ?                                                        \
    FALSE :                                                                 \
    FlagOn(FsRtlLegalAnsiCharacterArray[(C)], FSRTL_WILD_CHARACTER ))       \
)

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupBaseMcbEntry (
    IN PBASE_MCB   Mcb,
    IN LONGLONG    Vbn,
    OUT PLONGLONG  Lbn OPTIONAL,
    OUT PLONGLONG  SectorCountFromLbn OPTIONAL,
    OUT PLONGLONG  StartingLbn OPTIONAL,
    OUT PLONGLONG  SectorCountFromStartingLbn OPTIONAL,
    OUT PULONG     Index OPTIONAL
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLargeMcbEntry (
    IN PLARGE_MCB  Mcb,
    IN LONGLONG    Vbn,
    OUT PLONGLONG  Lbn OPTIONAL,
    OUT PLONGLONG  SectorCountFromLbn OPTIONAL,
    OUT PLONGLONG  StartingLbn OPTIONAL,
    OUT PLONGLONG  SectorCountFromStartingLbn OPTIONAL,
    OUT PULONG     Index OPTIONAL
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLastBaseMcbEntry (
    IN PBASE_MCB   Mcb,
    OUT PLONGLONG  Vbn,
    OUT PLONGLONG  Lbn
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLastLargeMcbEntry (
    IN PLARGE_MCB  Mcb,
    OUT PLONGLONG  Vbn,
    OUT PLONGLONG  Lbn
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLastMcbEntry (
    IN PMCB   Mcb,
    OUT PVBN  Vbn,
    OUT PLBN  Lbn
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLastBaseMcbEntryAndIndex (
    IN PBASE_MCB      OpaqueMcb,
    IN OUT PLONGLONG  LargeVbn,
    IN OUT PLONGLONG  LargeLbn,
    IN OUT PULONG     Index
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupLastLargeMcbEntryAndIndex (
    IN PLARGE_MCB  OpaqueMcb,
    OUT PLONGLONG  LargeVbn,
    OUT PLONGLONG  LargeLbn,
    OUT PULONG     Index
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlLookupMcbEntry (
    IN PMCB     Mcb,
    IN VBN      Vbn,
    OUT PLBN    Lbn,
    OUT PULONG  SectorCount OPTIONAL,
    OUT PULONG  Index
);

NTKERNELAPI
PFSRTL_PER_STREAM_CONTEXT
NTAPI
FsRtlLookupPerStreamContextInternal (
    IN PFSRTL_ADVANCED_FCB_HEADER  StreamContext,
    IN PVOID                       OwnerId OPTIONAL,
    IN PVOID                       InstanceId OPTIONAL
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlMdlReadDev (
    IN PFILE_OBJECT       FileObject,
    IN PLARGE_INTEGER     FileOffset,
    IN ULONG              Length,
    IN ULONG              LockKey,
    OUT PMDL              *MdlChain,
    OUT PIO_STATUS_BLOCK  IoStatus,
    IN PDEVICE_OBJECT     DeviceObject
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlMdlReadComplete (
    IN PFILE_OBJECT     FileObject,
    IN PMDL             MdlChain
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlMdlReadCompleteDev (
    IN PFILE_OBJECT     FileObject,
    IN PMDL             MdlChain,
    IN PDEVICE_OBJECT   DeviceObject
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlPrepareMdlWriteDev (
    IN PFILE_OBJECT       FileObject,
    IN PLARGE_INTEGER     FileOffset,
    IN ULONG              Length,
    IN ULONG              LockKey,
    OUT PMDL              *MdlChain,
    OUT PIO_STATUS_BLOCK  IoStatus,
    IN PDEVICE_OBJECT     DeviceObject
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlMdlWriteComplete (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN PMDL             MdlChain
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlMdlWriteCompleteDev (
    IN PFILE_OBJECT     FileObject,
    IN PLARGE_INTEGER   FileOffset,
    IN PMDL             MdlChain,
    IN PDEVICE_OBJECT   DeviceObject
);

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlNormalizeNtstatus (
    IN NTSTATUS Exception,
    IN NTSTATUS GenericException
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyChangeDirectory (
    IN PNOTIFY_SYNC NotifySync,
    IN PVOID        FsContext,
    IN PSTRING      FullDirectoryName,
    IN PLIST_ENTRY  NotifyList,
    IN BOOLEAN      WatchTree,
    IN ULONG        CompletionFilter,
    IN PIRP         NotifyIrp
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyCleanup (
    IN PNOTIFY_SYNC NotifySync,
    IN PLIST_ENTRY  NotifyList,
    IN PVOID        FsContext
);

typedef BOOLEAN (NTAPI *PCHECK_FOR_TRAVERSE_ACCESS) (
    IN PVOID                        NotifyContext,
    IN PVOID                        TargetContext,
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyFilterChangeDirectory (
    IN PNOTIFY_SYNC                 NotifySync,
    IN PLIST_ENTRY                  NotifyList,
    IN PVOID                        FsContext,
    IN PSTRING                      FullDirectoryName,
    IN BOOLEAN                      WatchTree,
    IN BOOLEAN                      IgnoreBuffer,
    IN ULONG                        CompletionFilter,
    IN PIRP                         NotifyIrp,
    IN PCHECK_FOR_TRAVERSE_ACCESS   TraverseCallback OPTIONAL,
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext OPTIONAL,
    IN PFILTER_REPORT_CHANGE        FilterCallback OPTIONAL);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyFilterReportChange (
    IN PNOTIFY_SYNC   NotifySync,
    IN PLIST_ENTRY    NotifyList,
    IN PSTRING        FullTargetName,
    IN USHORT         TargetNameOffset,
    IN PSTRING        StreamName OPTIONAL,
    IN PSTRING        NormalizedParentName OPTIONAL,
    IN ULONG          FilterMatch,
    IN ULONG          Action,
    IN PVOID          TargetContext,
    IN PVOID          FilterContext);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyFullChangeDirectory (
    IN PNOTIFY_SYNC                 NotifySync,
    IN PLIST_ENTRY                  NotifyList,
    IN PVOID                        FsContext,
    IN PSTRING                      FullDirectoryName,
    IN BOOLEAN                      WatchTree,
    IN BOOLEAN                      IgnoreBuffer,
    IN ULONG                        CompletionFilter,
    IN PIRP                         NotifyIrp,
    IN PCHECK_FOR_TRAVERSE_ACCESS   TraverseCallback OPTIONAL,
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext OPTIONAL
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyFullReportChange (
    IN PNOTIFY_SYNC NotifySync,
    IN PLIST_ENTRY  NotifyList,
    IN PSTRING      FullTargetName,
    IN USHORT       TargetNameOffset,
    IN PSTRING      StreamName OPTIONAL,
    IN PSTRING      NormalizedParentName OPTIONAL,
    IN ULONG        FilterMatch,
    IN ULONG        Action,
    IN PVOID        TargetContext
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyInitializeSync (
    IN PNOTIFY_SYNC *NotifySync
);

NTKERNELAPI
VOID
NTAPI
FsRtlNotifyUninitializeSync (
    IN PNOTIFY_SYNC *NotifySync
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlNotifyVolumeEvent (
    IN PFILE_OBJECT FileObject,
    IN ULONG        EventCode
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
ULONG
NTAPI
FsRtlNumberOfRunsInBaseMcb (
    IN PBASE_MCB Mcb
);

NTKERNELAPI
ULONG
NTAPI
FsRtlNumberOfRunsInLargeMcb (
    IN PLARGE_MCB Mcb
);

NTKERNELAPI
ULONG
NTAPI
FsRtlNumberOfRunsInMcb (
    IN PMCB Mcb
);

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlOplockFsctrl (
    IN POPLOCK  Oplock,
    IN PIRP     Irp,
    IN ULONG    OpenCount
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlOplockIsFastIoPossible (
    IN POPLOCK Oplock
);

typedef VOID
(NTAPI *PFSRTL_STACK_OVERFLOW_ROUTINE) (
    IN PVOID    Context,
    IN PKEVENT  Event
);

NTKERNELAPI
VOID
NTAPI
FsRtlPostPagingFileStackOverflow (
    IN PVOID                          Context,
    IN PKEVENT                        Event,
    IN PFSRTL_STACK_OVERFLOW_ROUTINE  StackOverflowRoutine
);

NTKERNELAPI
VOID
NTAPI
FsRtlPostStackOverflow (
    IN PVOID                          Context,
    IN PKEVENT                        Event,
    IN PFSRTL_STACK_OVERFLOW_ROUTINE  StackOverflowRoutine
);

/*
  FsRtlPrivateLock:

  ret: IoStatus->Status: STATUS_PENDING, STATUS_LOCK_NOT_GRANTED

  Internals:
    -Calls IoCompleteRequest if Irp
    -Uses exception handling / ExRaiseStatus with STATUS_INSUFFICIENT_RESOURCES
*/
NTKERNELAPI
BOOLEAN
NTAPI
FsRtlPrivateLock (
    IN PFILE_LOCK           FileLock,
    IN PFILE_OBJECT         FileObject,
    IN PLARGE_INTEGER       FileOffset,
    IN PLARGE_INTEGER       Length,
    IN PEPROCESS            Process,
    IN ULONG                Key,
    IN BOOLEAN              FailImmediately,
    IN BOOLEAN              ExclusiveLock,
    OUT PIO_STATUS_BLOCK    IoStatus,
    IN PIRP                 Irp OPTIONAL,
    IN PVOID                Context,
    IN BOOLEAN              AlreadySynchronized
);

/*
  FsRtlProcessFileLock:

  ret:
    -STATUS_INVALID_DEVICE_REQUEST
    -STATUS_RANGE_NOT_LOCKED from unlock routines.
    -STATUS_PENDING, STATUS_LOCK_NOT_GRANTED from FsRtlPrivateLock
    (redirected IoStatus->Status).

  Internals:
    -switch ( Irp->CurrentStackLocation->MinorFunction )
        lock: return FsRtlPrivateLock;
        unlocksingle: return FsRtlFastUnlockSingle;
        unlockall: return FsRtlFastUnlockAll;
        unlockallbykey: return FsRtlFastUnlockAllByKey;
        default: IofCompleteRequest with STATUS_INVALID_DEVICE_REQUEST;
                 return STATUS_INVALID_DEVICE_REQUEST;

    -'AllwaysZero' is passed thru as 'AllwaysZero' to lock / unlock routines.
    -'Irp' is passet thru as 'Irp' to FsRtlPrivateLock.
*/
NTKERNELAPI
NTSTATUS
NTAPI
FsRtlProcessFileLock (
    IN PFILE_LOCK   FileLock,
    IN PIRP         Irp,
    IN PVOID        Context OPTIONAL
);

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlRegisterUncProvider (
    IN OUT PHANDLE      MupHandle,
    IN PUNICODE_STRING  RedirectorDeviceName,
    IN BOOLEAN          MailslotsSupported
);

NTKERNELAPI
VOID
NTAPI
FsRtlRemoveBaseMcbEntry (
    IN PBASE_MCB  Mcb,
    IN LONGLONG   Vbn,
    IN LONGLONG   SectorCount
);

NTKERNELAPI
VOID
NTAPI
FsRtlRemoveLargeMcbEntry (
    IN PLARGE_MCB  Mcb,
    IN LONGLONG    Vbn,
    IN LONGLONG    SectorCount
);

NTKERNELAPI
VOID
NTAPI
FsRtlRemoveMcbEntry (
    IN PMCB   Mcb,
    IN VBN    Vbn,
    IN ULONG  SectorCount
);

NTKERNELAPI
PFSRTL_PER_STREAM_CONTEXT
NTAPI
FsRtlRemovePerStreamContext (
    IN PFSRTL_ADVANCED_FCB_HEADER  StreamContext,
    IN PVOID                       OwnerId OPTIONAL,
    IN PVOID                       InstanceId OPTIONAL
);

NTKERNELAPI
VOID
NTAPI
FsRtlResetBaseMcb (
    IN PBASE_MCB Mcb
);

NTKERNELAPI
VOID
NTAPI
FsRtlResetLargeMcb (
    IN PLARGE_MCB  Mcb,
    IN BOOLEAN     SelfSynchronized
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlSplitBaseMcb (
    IN PBASE_MCB  Mcb,
    IN LONGLONG   Vbn,
    IN LONGLONG   Amount
);

NTKERNELAPI
BOOLEAN
NTAPI
FsRtlSplitLargeMcb (
    IN PLARGE_MCB  Mcb,
    IN LONGLONG    Vbn,
    IN LONGLONG    Amount
);

#define FsRtlSupportsPerStreamContexts(FO) (                       \
    (BOOLEAN)((NULL != FsRtlGetPerStreamContextPointer(FO) &&     \
              FlagOn(FsRtlGetPerStreamContextPointer(FO)->Flags2, \
              FSRTL_FLAG2_SUPPORTS_FILTER_CONTEXTS))               \
)

NTKERNELAPI
VOID
NTAPI
FsRtlTruncateBaseMcb (
    IN PBASE_MCB  Mcb,
    IN LONGLONG   Vbn
);

NTKERNELAPI
VOID
NTAPI
FsRtlTruncateLargeMcb (
    IN PLARGE_MCB  Mcb,
    IN LONGLONG    Vbn
);

NTKERNELAPI
VOID
NTAPI
FsRtlTruncateMcb (
    IN PMCB  Mcb,
    IN VBN   Vbn
);

NTKERNELAPI
VOID
NTAPI
FsRtlUninitializeBaseMcb (
    IN PBASE_MCB Mcb
);

NTKERNELAPI
VOID
NTAPI
FsRtlUninitializeFileLock (
    IN PFILE_LOCK FileLock
);

NTKERNELAPI
VOID
NTAPI
FsRtlUninitializeLargeMcb (
    IN PLARGE_MCB Mcb
);

NTKERNELAPI
VOID
NTAPI
FsRtlUninitializeMcb (
    IN PMCB Mcb
);

NTKERNELAPI
VOID
NTAPI
FsRtlUninitializeOplock (
    IN OUT POPLOCK Oplock
);

NTKERNELAPI
UCHAR
NTAPI
KeSetIdealProcessorThread(
    IN OUT PKTHREAD Thread,
    IN UCHAR Processor
);

NTKERNELAPI
NTSTATUS
NTAPI
IoAttachDeviceToDeviceStackSafe(
    IN PDEVICE_OBJECT   SourceDevice,
    IN PDEVICE_OBJECT   TargetDevice,
    OUT PDEVICE_OBJECT  *AttachedToDeviceObject
);

NTKERNELAPI
VOID
NTAPI
IoAcquireVpbSpinLock (
    OUT PKIRQL Irql
);

NTKERNELAPI
NTSTATUS
NTAPI
IoCheckDesiredAccess (
    IN OUT PACCESS_MASK DesiredAccess,
    IN ACCESS_MASK      GrantedAccess
);

NTKERNELAPI
NTSTATUS
NTAPI
IoCheckEaBufferValidity (
    IN PFILE_FULL_EA_INFORMATION    EaBuffer,
    IN ULONG                        EaLength,
    OUT PULONG                      ErrorOffset
);

NTKERNELAPI
NTSTATUS
NTAPI
IoCheckFunctionAccess (
    IN ACCESS_MASK              GrantedAccess,
    IN UCHAR                    MajorFunction,
    IN UCHAR                    MinorFunction,
    IN ULONG                    IoControlCode,
    IN PVOID                    Argument1 OPTIONAL,
    IN PVOID                    Argument2 OPTIONAL
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
IoCheckQuotaBufferValidity (
    IN PFILE_QUOTA_INFORMATION  QuotaBuffer,
    IN ULONG                    QuotaLength,
    OUT PULONG                  ErrorOffset
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
PFILE_OBJECT
NTAPI
IoCreateStreamFileObject (
    IN PFILE_OBJECT     FileObject OPTIONAL,
    IN PDEVICE_OBJECT   DeviceObject OPTIONAL
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
PFILE_OBJECT
NTAPI
IoCreateStreamFileObjectLite (
    IN PFILE_OBJECT     FileObject OPTIONAL,
    IN PDEVICE_OBJECT   DeviceObject OPTIONAL
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
BOOLEAN
NTAPI
IoFastQueryNetworkAttributes (
    IN POBJECT_ATTRIBUTES               ObjectAttributes,
    IN ACCESS_MASK                      DesiredAccess,
    IN ULONG                            OpenOptions,
    OUT PIO_STATUS_BLOCK                IoStatus,
    OUT PFILE_NETWORK_OPEN_INFORMATION  Buffer
);

NTKERNELAPI
PDEVICE_OBJECT
NTAPI
IoGetAttachedDevice (
    IN PDEVICE_OBJECT DeviceObject
);

NTKERNELAPI
PDEVICE_OBJECT
NTAPI
IoGetBaseFileSystemDeviceObject (
    IN PFILE_OBJECT FileObject
);

#if (VER_PRODUCTBUILD >= 2600)

NTKERNELAPI
PDEVICE_OBJECT
NTAPI
IoGetDeviceAttachmentBaseRef (
    IN PDEVICE_OBJECT DeviceObject
);

NTKERNELAPI
NTSTATUS
NTAPI
IoGetDiskDeviceObject (
    IN PDEVICE_OBJECT   FileSystemDeviceObject,
    OUT PDEVICE_OBJECT  *DiskDeviceObject
);

NTKERNELAPI
PDEVICE_OBJECT
NTAPI
IoGetLowerDeviceObject (
    IN PDEVICE_OBJECT DeviceObject
);

#endif /* (VER_PRODUCTBUILD >= 2600) */

NTKERNELAPI
PEPROCESS
NTAPI
IoGetRequestorProcess (
    IN PIRP Irp
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
ULONG
NTAPI
IoGetRequestorProcessId (
    IN PIRP Irp
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
PIRP
NTAPI
IoGetTopLevelIrp (
    VOID
);

#define IoIsFileOpenedExclusively(FileObject) ( \
    (BOOLEAN) !(                                \
    (FileObject)->SharedRead ||                 \
    (FileObject)->SharedWrite ||                \
    (FileObject)->SharedDelete                  \
    )                                           \
)

NTKERNELAPI
BOOLEAN
NTAPI
IoIsOperationSynchronous (
    IN PIRP Irp
);

NTKERNELAPI
BOOLEAN
NTAPI
IoIsSystemThread (
    IN PETHREAD Thread
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
BOOLEAN
NTAPI
IoIsValidNameGraftingBuffer (
    IN PIRP                 Irp,
    IN PREPARSE_DATA_BUFFER ReparseBuffer
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
NTSTATUS
NTAPI
IoPageRead (
    IN PFILE_OBJECT         FileObject,
    IN PMDL                 Mdl,
    IN PLARGE_INTEGER       Offset,
    IN PKEVENT              Event,
    OUT PIO_STATUS_BLOCK    IoStatusBlock
);

NTKERNELAPI
NTSTATUS
NTAPI
IoQueryFileInformation (
    IN PFILE_OBJECT             FileObject,
    IN FILE_INFORMATION_CLASS   FileInformationClass,
    IN ULONG                    Length,
    OUT PVOID                   FileInformation,
    OUT PULONG                  ReturnedLength
);

NTKERNELAPI
NTSTATUS
NTAPI
IoQueryVolumeInformation (
    IN PFILE_OBJECT         FileObject,
    IN FS_INFORMATION_CLASS FsInformationClass,
    IN ULONG                Length,
    OUT PVOID               FsInformation,
    OUT PULONG              ReturnedLength
);

NTKERNELAPI
VOID
NTAPI
IoQueueThreadIrp(
    IN PIRP Irp
);

NTKERNELAPI
VOID
NTAPI
IoRegisterFileSystem (
    IN OUT PDEVICE_OBJECT DeviceObject
);

#if (VER_PRODUCTBUILD >= 1381)

typedef VOID (NTAPI *PDRIVER_FS_NOTIFICATION) (
    IN PDEVICE_OBJECT DeviceObject,
    IN BOOLEAN        DriverActive
);

NTKERNELAPI
NTSTATUS
NTAPI
IoRegisterFsRegistrationChange (
    IN PDRIVER_OBJECT           DriverObject,
    IN PDRIVER_FS_NOTIFICATION  DriverNotificationRoutine
);

#endif /* (VER_PRODUCTBUILD >= 1381) */

NTKERNELAPI
VOID
NTAPI
IoReleaseVpbSpinLock (
    IN KIRQL Irql
);

NTKERNELAPI
VOID
NTAPI
IoSetDeviceToVerify (
    IN PETHREAD         Thread,
    IN PDEVICE_OBJECT   DeviceObject
);

NTKERNELAPI
NTSTATUS
NTAPI
IoSetInformation (
    IN PFILE_OBJECT             FileObject,
    IN FILE_INFORMATION_CLASS   FileInformationClass,
    IN ULONG                    Length,
    IN PVOID                    FileInformation
);

NTKERNELAPI
VOID
NTAPI
IoSetTopLevelIrp (
    IN PIRP Irp
);

NTKERNELAPI
NTSTATUS
NTAPI
IoSynchronousPageWrite (
    IN PFILE_OBJECT         FileObject,
    IN PMDL                 Mdl,
    IN PLARGE_INTEGER       FileOffset,
    IN PKEVENT              Event,
    OUT PIO_STATUS_BLOCK    IoStatusBlock
);

NTKERNELAPI
PEPROCESS
NTAPI
IoThreadToProcess (
    IN PETHREAD Thread
);

NTKERNELAPI
VOID
NTAPI
IoUnregisterFileSystem (
    IN OUT PDEVICE_OBJECT DeviceObject
);

#if (VER_PRODUCTBUILD >= 1381)

NTKERNELAPI
VOID
NTAPI
IoUnregisterFsRegistrationChange (
    IN PDRIVER_OBJECT           DriverObject,
    IN PDRIVER_FS_NOTIFICATION  DriverNotificationRoutine
);

#endif /* (VER_PRODUCTBUILD >= 1381) */

NTKERNELAPI
NTSTATUS
NTAPI
IoVerifyVolume (
    IN PDEVICE_OBJECT   DeviceObject,
    IN BOOLEAN          AllowRawMount
);

#if !defined (_M_AMD64)

NTHALAPI
KIRQL
FASTCALL
KeAcquireQueuedSpinLock (
    IN KSPIN_LOCK_QUEUE_NUMBER Number
);

NTHALAPI
VOID
FASTCALL
KeReleaseQueuedSpinLock (
    IN KSPIN_LOCK_QUEUE_NUMBER Number,
    IN KIRQL OldIrql
);

NTHALAPI
KIRQL
FASTCALL
KeAcquireSpinLockRaiseToSynch(
    IN OUT PKSPIN_LOCK SpinLock
);

NTHALAPI
LOGICAL
FASTCALL
KeTryToAcquireQueuedSpinLock(
  KSPIN_LOCK_QUEUE_NUMBER Number,
  PKIRQL OldIrql);

#else

NTKERNELAPI
KIRQL
FASTCALL
KeAcquireQueuedSpinLock (
    IN KSPIN_LOCK_QUEUE_NUMBER Number
);

NTKERNELAPI
VOID
FASTCALL
KeReleaseQueuedSpinLock (
    IN KSPIN_LOCK_QUEUE_NUMBER Number,
    IN KIRQL OldIrql
);

NTKERNELAPI
KIRQL
KeAcquireSpinLockRaiseToSynch(
    IN OUT PKSPIN_LOCK SpinLock
);

NTKERNELAPI
LOGICAL
KeTryToAcquireQueuedSpinLock(
  KSPIN_LOCK_QUEUE_NUMBER Number,
  PKIRQL OldIrql);

#endif

NTKERNELAPI
VOID
NTAPI
KeAttachProcess (
    IN PKPROCESS Process
);

NTKERNELAPI
VOID
NTAPI
KeDetachProcess (
    VOID
);

NTKERNELAPI
VOID
NTAPI
KeInitializeQueue (
    IN PRKQUEUE Queue,
    IN ULONG    Count OPTIONAL
);

NTKERNELAPI
LONG
NTAPI
KeInsertHeadQueue (
    IN PRKQUEUE     Queue,
    IN PLIST_ENTRY  Entry
);

NTKERNELAPI
LONG
NTAPI
KeInsertQueue (
    IN PRKQUEUE     Queue,
    IN PLIST_ENTRY  Entry
);

NTKERNELAPI
LONG
NTAPI
KeReadStateQueue (
    IN PRKQUEUE Queue
);

NTKERNELAPI
PLIST_ENTRY
NTAPI
KeRemoveQueue (
    IN PRKQUEUE         Queue,
    IN KPROCESSOR_MODE  WaitMode,
    IN PLARGE_INTEGER   Timeout OPTIONAL
);

NTKERNELAPI
PLIST_ENTRY
NTAPI
KeRundownQueue (
    IN PRKQUEUE Queue
);

NTKERNELAPI
VOID
NTAPI
KeInitializeMutant (
    IN PRKMUTANT  Mutant,
    IN BOOLEAN    InitialOwner
);

NTKERNELAPI
LONG
NTAPI
KeReadStateMutant (
    IN PRKMUTANT  Mutant
);

NTKERNELAPI
LONG
NTAPI
KeReleaseMutant (
    IN PRKMUTANT  Mutant,
    IN KPRIORITY  Increment,
    IN BOOLEAN    Abandoned,
    IN BOOLEAN    Wait
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
VOID
NTAPI
KeStackAttachProcess (
    IN PKPROCESS    Process,
    OUT PKAPC_STATE ApcState
);

NTKERNELAPI
VOID
NTAPI
KeUnstackDetachProcess (
    IN PKAPC_STATE ApcState
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
BOOLEAN
NTAPI
KeSetKernelStackSwapEnable(
    IN BOOLEAN Enable
);

NTKERNELAPI
BOOLEAN
NTAPI
MmCanFileBeTruncated (
    IN PSECTION_OBJECT_POINTERS     SectionObjectPointer,
    IN PLARGE_INTEGER               NewFileSize
);

NTKERNELAPI
BOOLEAN
NTAPI
MmFlushImageSection (
    IN PSECTION_OBJECT_POINTERS     SectionObjectPointer,
    IN MMFLUSH_TYPE                 FlushType
);

NTKERNELAPI
BOOLEAN
NTAPI
MmForceSectionClosed (
    IN PSECTION_OBJECT_POINTERS SectionObjectPointer,
    IN BOOLEAN                  DelayClose
);

#if (VER_PRODUCTBUILD >= 1381)

NTKERNELAPI
BOOLEAN
NTAPI
MmIsRecursiveIoFault (
    VOID
);

#else

#define MmIsRecursiveIoFault() (                            \
    (PsGetCurrentThread()->DisablePageFaultClustering) |    \
    (PsGetCurrentThread()->ForwardClusterOnly)              \
)

#endif


NTKERNELAPI
BOOLEAN
NTAPI
MmSetAddressRangeModified (
    IN PVOID    Address,
    IN SIZE_T    Length
);

NTKERNELAPI
NTSTATUS
NTAPI
ObCreateObject (
    IN KPROCESSOR_MODE      ObjectAttributesAccessMode OPTIONAL,
    IN POBJECT_TYPE         ObjectType,
    IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
    IN KPROCESSOR_MODE      AccessMode,
    IN OUT PVOID            ParseContext OPTIONAL,
    IN ULONG                ObjectSize,
    IN ULONG                PagedPoolCharge OPTIONAL,
    IN ULONG                NonPagedPoolCharge OPTIONAL,
    OUT PVOID               *Object
);

NTKERNELAPI
ULONG
NTAPI
ObGetObjectPointerCount (
    IN PVOID Object
);

#if (NTDDI_VERSION >= NTDDI_WIN2K)

NTKERNELAPI
NTSTATUS
NTAPI
ObInsertObject (
  IN PVOID Object,
  IN PACCESS_STATE PassedAccessState OPTIONAL,
  IN ACCESS_MASK DesiredAccess OPTIONAL,
  IN ULONG ObjectPointerBias,
  OUT PVOID *NewObject OPTIONAL,
  OUT PHANDLE Handle OPTIONAL);

NTKERNELAPI
NTSTATUS
NTAPI
ObOpenObjectByPointer (
  IN PVOID Object,
  IN ULONG HandleAttributes,
  IN PACCESS_STATE PassedAccessState OPTIONAL,
  IN ACCESS_MASK DesiredAccess OPTIONAL,
  IN POBJECT_TYPE ObjectType OPTIONAL,
  IN KPROCESSOR_MODE AccessMode,
  OUT PHANDLE Handle);

NTKERNELAPI
VOID
NTAPI
ObMakeTemporaryObject (
  IN PVOID Object);

NTKERNELAPI
NTSTATUS
NTAPI
ObQueryObjectAuditingByHandle (
  IN HANDLE Handle,
  OUT PBOOLEAN GenerateOnClose);

#endif

NTKERNELAPI
NTSTATUS
NTAPI
ObQueryNameString (
    IN PVOID                        Object,
    OUT POBJECT_NAME_INFORMATION    ObjectNameInfo,
    IN ULONG                        Length,
    OUT PULONG                      ReturnLength
);

NTKERNELAPI
NTSTATUS
NTAPI
ObReferenceObjectByName (
    IN PUNICODE_STRING  ObjectName,
    IN ULONG            Attributes,
    IN PACCESS_STATE    PassedAccessState OPTIONAL,
    IN ACCESS_MASK      DesiredAccess OPTIONAL,
    IN POBJECT_TYPE     ObjectType,
    IN KPROCESSOR_MODE  AccessMode,
    IN OUT PVOID        ParseContext OPTIONAL,
    OUT PVOID           *Object
);

NTKERNELAPI
NTSTATUS
NTAPI
PsAssignImpersonationToken (
    IN PETHREAD     Thread,
    IN HANDLE       Token
);

NTKERNELAPI
VOID
NTAPI
PsChargePoolQuota (
    IN PEPROCESS    Process,
    IN POOL_TYPE    PoolType,
    IN SIZE_T       Amount
);

NTKERNELAPI
NTSTATUS
NTAPI
PsChargeProcessPoolQuota (
    IN PEPROCESS    Process,
    IN POOL_TYPE    PoolType,
    IN SIZE_T       Amount
);

#define PsDereferenceImpersonationToken(T)  \
            {if (ARGUMENT_PRESENT(T)) {     \
                (ObDereferenceObject((T))); \
            } else {                        \
                ;                           \
            }                               \
}

#define PsDereferencePrimaryToken(T) (ObDereferenceObject((T)))

NTKERNELAPI
BOOLEAN
NTAPI
PsDisableImpersonation(
    IN PETHREAD                 Thread,
    IN PSE_IMPERSONATION_STATE  ImpersonationState
);

NTKERNELAPI
LARGE_INTEGER
NTAPI
PsGetProcessExitTime (
    VOID
);

NTKERNELAPI
NTSTATUS
NTAPI
PsImpersonateClient(
    IN PETHREAD                      Thread,
    IN PACCESS_TOKEN                 Token,
    IN BOOLEAN                       CopyOnOpen,
    IN BOOLEAN                       EffectiveOnly,
    IN SECURITY_IMPERSONATION_LEVEL  ImpersonationLevel
);

NTKERNELAPI
BOOLEAN
NTAPI
PsIsSystemThread(
    IN PETHREAD Thread
);

NTKERNELAPI
BOOLEAN
NTAPI
PsIsThreadTerminating (
    IN PETHREAD Thread
);

NTKERNELAPI
NTSTATUS
NTAPI
PsLookupProcessByProcessId (
    IN HANDLE       ProcessId,
    OUT PEPROCESS   *Process
);

NTKERNELAPI
NTSTATUS
NTAPI
PsLookupProcessThreadByCid (
    IN PCLIENT_ID   Cid,
    OUT PEPROCESS   *Process OPTIONAL,
    OUT PETHREAD    *Thread
);

NTKERNELAPI
NTSTATUS
NTAPI
PsLookupThreadByThreadId (
    IN HANDLE       UniqueThreadId,
    OUT PETHREAD    *Thread
);

NTKERNELAPI
PACCESS_TOKEN
NTAPI
PsReferenceImpersonationToken (
    IN PETHREAD                         Thread,
    OUT PBOOLEAN                        CopyOnUse,
    OUT PBOOLEAN                        EffectiveOnly,
    OUT PSECURITY_IMPERSONATION_LEVEL   Level
);

NTKERNELAPI
HANDLE
NTAPI
PsReferencePrimaryToken (
    IN PEPROCESS Process
);

NTKERNELAPI
VOID
NTAPI
PsRestoreImpersonation(
    IN PETHREAD                 Thread,
    IN PSE_IMPERSONATION_STATE  ImpersonationState
);

NTKERNELAPI
VOID
NTAPI
PsReturnPoolQuota (
    IN PEPROCESS    Process,
    IN POOL_TYPE    PoolType,
    IN SIZE_T       Amount
);

NTKERNELAPI
VOID
NTAPI
PsRevertToSelf (
    VOID
);

NTSYSAPI
VOID
NTAPI
RtlGenerate8dot3Name (
    IN PUNICODE_STRING              Name,
    IN BOOLEAN                      AllowExtendedCharacters,
    IN OUT PGENERATE_NAME_CONTEXT   Context,
    OUT PUNICODE_STRING             Name8dot3
);

NTSYSAPI
VOID
NTAPI
RtlSecondsSince1970ToTime (
    IN ULONG            SecondsSince1970,
    OUT PLARGE_INTEGER  Time
);

NTSYSAPI
NTSTATUS
NTAPI
RtlSetSaclSecurityDescriptor (
    IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN BOOLEAN                  SaclPresent,
    IN PACL                     Sacl,
    IN BOOLEAN                  SaclDefaulted
);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToCountedOemString (
    IN OUT POEM_STRING  DestinationString,
    IN PCUNICODE_STRING SourceString,
    IN BOOLEAN          AllocateDestinationString
);

/* RTL Splay Tree Functions */
NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlSplay(PRTL_SPLAY_LINKS Links);

NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlDelete(PRTL_SPLAY_LINKS Links);

NTSYSAPI
VOID
NTAPI
RtlDeleteNoSplay(
    PRTL_SPLAY_LINKS Links,
    PRTL_SPLAY_LINKS *Root
);

NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlSubtreeSuccessor(PRTL_SPLAY_LINKS Links);

NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlSubtreePredecessor(PRTL_SPLAY_LINKS Links);

NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlRealSuccessor(PRTL_SPLAY_LINKS Links);

NTSYSAPI
PRTL_SPLAY_LINKS
NTAPI
RtlRealPredecessor(PRTL_SPLAY_LINKS Links);

#define RtlIsLeftChild(Links) \
    (RtlLeftChild(RtlParent(Links)) == (PRTL_SPLAY_LINKS)(Links))

#define RtlIsRightChild(Links) \
    (RtlRightChild(RtlParent(Links)) == (PRTL_SPLAY_LINKS)(Links))

#define RtlRightChild(Links) \
    ((PRTL_SPLAY_LINKS)(Links))->RightChild

#define RtlIsRoot(Links) \
    (RtlParent(Links) == (PRTL_SPLAY_LINKS)(Links))

#define RtlLeftChild(Links) \
    ((PRTL_SPLAY_LINKS)(Links))->LeftChild

#define RtlParent(Links) \
    ((PRTL_SPLAY_LINKS)(Links))->Parent

#define RtlInitializeSplayLinks(Links)                  \
    {                                                   \
        PRTL_SPLAY_LINKS _SplayLinks;                   \
        _SplayLinks = (PRTL_SPLAY_LINKS)(Links);        \
        _SplayLinks->Parent = _SplayLinks;              \
        _SplayLinks->LeftChild = NULL;                  \
        _SplayLinks->RightChild = NULL;                 \
    }

#define RtlInsertAsLeftChild(ParentLinks,ChildLinks)    \
    {                                                   \
        PRTL_SPLAY_LINKS _SplayParent;                  \
        PRTL_SPLAY_LINKS _SplayChild;                   \
        _SplayParent = (PRTL_SPLAY_LINKS)(ParentLinks); \
        _SplayChild = (PRTL_SPLAY_LINKS)(ChildLinks);   \
        _SplayParent->LeftChild = _SplayChild;          \
        _SplayChild->Parent = _SplayParent;             \
    }

#define RtlInsertAsRightChild(ParentLinks,ChildLinks)   \
    {                                                   \
        PRTL_SPLAY_LINKS _SplayParent;                  \
        PRTL_SPLAY_LINKS _SplayChild;                   \
        _SplayParent = (PRTL_SPLAY_LINKS)(ParentLinks); \
        _SplayChild = (PRTL_SPLAY_LINKS)(ChildLinks);   \
        _SplayParent->RightChild = _SplayChild;         \
        _SplayChild->Parent = _SplayParent;             \
    }

//
// RTL time functions
//

NTKERNELAPI
NTSTATUS
NTAPI
SeAppendPrivileges (
    PACCESS_STATE   AccessState,
    PPRIVILEGE_SET  Privileges
);

NTKERNELAPI
BOOLEAN
NTAPI
SeAuditingFileEvents (
    IN BOOLEAN              AccessGranted,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor
);

NTKERNELAPI
BOOLEAN
NTAPI
SeAuditingFileOrGlobalEvents (
    IN BOOLEAN                      AccessGranted,
    IN PSECURITY_DESCRIPTOR         SecurityDescriptor,
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext
);

NTKERNELAPI
VOID
NTAPI
SeCaptureSubjectContext (
    OUT PSECURITY_SUBJECT_CONTEXT SubjectContext
);

NTKERNELAPI
NTSTATUS
NTAPI
SeCreateClientSecurity (
    IN PETHREAD                     Thread,
    IN PSECURITY_QUALITY_OF_SERVICE QualityOfService,
    IN BOOLEAN                      RemoteClient,
    OUT PSECURITY_CLIENT_CONTEXT    ClientContext
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
SeCreateClientSecurityFromSubjectContext (
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext,
    IN PSECURITY_QUALITY_OF_SERVICE QualityOfService,
    IN BOOLEAN                      ServerIsRemote,
    OUT PSECURITY_CLIENT_CONTEXT    ClientContext
);

#endif /* (VER_PRODUCTBUILD >= 2195) */


#define SeLengthSid( Sid ) \
    (8 + (4 * ((SID *)Sid)->SubAuthorityCount))

#define SeDeleteClientSecurity(C)  {                                           \
            if (SeTokenType((C)->ClientToken) == TokenPrimary) {               \
                PsDereferencePrimaryToken( (C)->ClientToken );                 \
            } else {                                                           \
                PsDereferenceImpersonationToken( (C)->ClientToken );           \
            }                                                                  \
}

NTKERNELAPI
VOID
NTAPI
SeDeleteObjectAuditAlarm (
    IN PVOID    Object,
    IN HANDLE   Handle
);

#define SeEnableAccessToExports() SeExports = *(PSE_EXPORTS *)SeExports;

NTKERNELAPI
VOID
NTAPI
SeFreePrivileges (
    IN PPRIVILEGE_SET Privileges
);

NTKERNELAPI
VOID
NTAPI
SeImpersonateClient (
    IN PSECURITY_CLIENT_CONTEXT ClientContext,
    IN PETHREAD                 ServerThread OPTIONAL
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
SeImpersonateClientEx (
    IN PSECURITY_CLIENT_CONTEXT ClientContext,
    IN PETHREAD                 ServerThread OPTIONAL
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
VOID
NTAPI
SeLockSubjectContext (
    IN PSECURITY_SUBJECT_CONTEXT SubjectContext
);

NTKERNELAPI
NTSTATUS
NTAPI
SeMarkLogonSessionForTerminationNotification (
    IN PLUID LogonId
);

NTKERNELAPI
VOID
NTAPI
SeOpenObjectAuditAlarm (
    IN PUNICODE_STRING      ObjectTypeName,
    IN PVOID                Object OPTIONAL,
    IN PUNICODE_STRING      AbsoluteObjectName OPTIONAL,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN PACCESS_STATE        AccessState,
    IN BOOLEAN              ObjectCreated,
    IN BOOLEAN              AccessGranted,
    IN KPROCESSOR_MODE      AccessMode,
    OUT PBOOLEAN            GenerateOnClose
);

NTKERNELAPI
VOID
NTAPI
SeOpenObjectForDeleteAuditAlarm (
    IN PUNICODE_STRING      ObjectTypeName,
    IN PVOID                Object OPTIONAL,
    IN PUNICODE_STRING      AbsoluteObjectName OPTIONAL,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN PACCESS_STATE        AccessState,
    IN BOOLEAN              ObjectCreated,
    IN BOOLEAN              AccessGranted,
    IN KPROCESSOR_MODE      AccessMode,
    OUT PBOOLEAN            GenerateOnClose
);

NTKERNELAPI
BOOLEAN
NTAPI
SePrivilegeCheck (
    IN OUT PPRIVILEGE_SET           RequiredPrivileges,
    IN PSECURITY_SUBJECT_CONTEXT    SubjectContext,
    IN KPROCESSOR_MODE              AccessMode
);

NTKERNELAPI
NTSTATUS
NTAPI
SeQueryAuthenticationIdToken (
    IN PACCESS_TOKEN    Token,
    OUT PLUID           LogonId
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
SeQueryInformationToken (
    IN PACCESS_TOKEN           Token,
    IN TOKEN_INFORMATION_CLASS TokenInformationClass,
    OUT PVOID                  *TokenInformation
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
NTSTATUS
NTAPI
SeQuerySecurityDescriptorInfo (
    IN PSECURITY_INFORMATION    SecurityInformation,
    OUT PSECURITY_DESCRIPTOR    SecurityDescriptor,
    IN OUT PULONG               Length,
    IN PSECURITY_DESCRIPTOR     *ObjectsSecurityDescriptor
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
SeQuerySessionIdToken (
    IN PACCESS_TOKEN    Token,
    IN PULONG           SessionId
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

#define SeQuerySubjectContextToken( SubjectContext )                \
    ( ARGUMENT_PRESENT(                                             \
        ((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->ClientToken   \
        ) ?                                                         \
    ((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->ClientToken :     \
    ((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->PrimaryToken )

typedef NTSTATUS (NTAPI *PSE_LOGON_SESSION_TERMINATED_ROUTINE) (
    IN PLUID LogonId
);

NTKERNELAPI
NTSTATUS
NTAPI
SeRegisterLogonSessionTerminatedRoutine (
    IN PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
);

NTKERNELAPI
VOID
NTAPI
SeReleaseSubjectContext (
    IN PSECURITY_SUBJECT_CONTEXT SubjectContext
);

NTKERNELAPI
VOID
NTAPI
SeSetAccessStateGenericMapping (
    PACCESS_STATE       AccessState,
    PGENERIC_MAPPING    GenericMapping
);

NTKERNELAPI
NTSTATUS
NTAPI
SeSetSecurityDescriptorInfo (
    IN PVOID                    Object OPTIONAL,
    IN PSECURITY_INFORMATION    SecurityInformation,
    IN PSECURITY_DESCRIPTOR     SecurityDescriptor,
    IN OUT PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
    IN POOL_TYPE                PoolType,
    IN PGENERIC_MAPPING         GenericMapping
);

#if (VER_PRODUCTBUILD >= 2195)

NTKERNELAPI
NTSTATUS
NTAPI
SeSetSecurityDescriptorInfoEx (
    IN PVOID                    Object OPTIONAL,
    IN PSECURITY_INFORMATION    SecurityInformation,
    IN PSECURITY_DESCRIPTOR     ModificationDescriptor,
    IN OUT PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
    IN ULONG                    AutoInheritFlags,
    IN POOL_TYPE                PoolType,
    IN PGENERIC_MAPPING         GenericMapping
);

NTKERNELAPI
BOOLEAN
NTAPI
SeTokenIsAdmin (
    IN PACCESS_TOKEN Token
);

NTKERNELAPI
BOOLEAN
NTAPI
SeTokenIsRestricted (
    IN PACCESS_TOKEN Token
);


NTSTATUS
NTAPI
SeLocateProcessImageName(
    IN PEPROCESS Process,
    OUT PUNICODE_STRING *pImageFileName
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTKERNELAPI
TOKEN_TYPE
NTAPI
SeTokenType (
    IN PACCESS_TOKEN Token
);

NTKERNELAPI
VOID
NTAPI
SeUnlockSubjectContext (
    IN PSECURITY_SUBJECT_CONTEXT SubjectContext
);

NTKERNELAPI
NTSTATUS
NTAPI
SeUnregisterLogonSessionTerminatedRoutine (
    IN PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwAdjustPrivilegesToken (
    IN HANDLE               TokenHandle,
    IN BOOLEAN              DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES    NewState,
    IN ULONG                BufferLength,
    OUT PTOKEN_PRIVILEGES   PreviousState OPTIONAL,
    OUT PULONG              ReturnLength
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwAlertThread (
    IN HANDLE ThreadHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAllocateVirtualMemory (
    IN HANDLE       ProcessHandle,
    IN OUT PVOID    *BaseAddress,
    IN ULONG_PTR    ZeroBits,
    IN OUT PSIZE_T  RegionSize,
    IN ULONG        AllocationType,
    IN ULONG        Protect
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheckAndAuditAlarm (
    IN PUNICODE_STRING      SubsystemName,
    IN PVOID                HandleId,
    IN PUNICODE_STRING      ObjectTypeName,
    IN PUNICODE_STRING      ObjectName,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN ACCESS_MASK          DesiredAccess,
    IN PGENERIC_MAPPING     GenericMapping,
    IN BOOLEAN              ObjectCreation,
    OUT PACCESS_MASK        GrantedAccess,
    OUT PBOOLEAN            AccessStatus,
    OUT PBOOLEAN            GenerateOnClose
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFile (
    IN HANDLE               FileHandle,
    OUT PIO_STATUS_BLOCK    IoStatusBlock
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwClearEvent (
    IN HANDLE EventHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCloseObjectAuditAlarm (
    IN PUNICODE_STRING  SubsystemName,
    IN PVOID            HandleId,
    IN BOOLEAN          GenerateOnClose
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSection (
    OUT PHANDLE             SectionHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER       MaximumSize OPTIONAL,
    IN ULONG                SectionPageProtection,
    IN ULONG                AllocationAttributes,
    IN HANDLE               FileHandle OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSymbolicLinkObject (
    OUT PHANDLE             SymbolicLinkHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes,
    IN PUNICODE_STRING      TargetName
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteFile (
    IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteValueKey (
    IN HANDLE           Handle,
    IN PUNICODE_STRING  Name
);


#if (NTDDI_VERSION >= NTDDI_WIN2K)
NTSYSAPI
NTSTATUS
NTAPI
ZwDeviceIoControlFile (
  IN HANDLE FileHandle,
  IN HANDLE Event OPTIONAL,
  IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
  IN PVOID ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK IoStatusBlock,
  IN ULONG IoControlCode,
  IN PVOID InputBuffer OPTIONAL,
  IN ULONG InputBufferLength,
  OUT PVOID OutputBuffer OPTIONAL,
  IN ULONG OutputBufferLength);
#endif

NTSYSAPI
NTSTATUS
NTAPI
ZwDisplayString (
    IN PUNICODE_STRING String
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateObject (
    IN HANDLE       SourceProcessHandle,
    IN HANDLE       SourceHandle,
    IN HANDLE       TargetProcessHandle OPTIONAL,
    OUT PHANDLE     TargetHandle OPTIONAL,
    IN ACCESS_MASK  DesiredAccess,
    IN ULONG        HandleAttributes,
    IN ULONG        Options
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateToken (
    IN HANDLE               ExistingTokenHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes,
    IN BOOLEAN              EffectiveOnly,
    IN TOKEN_TYPE           TokenType,
    OUT PHANDLE             NewTokenHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushInstructionCache (
    IN HANDLE   ProcessHandle,
    IN PVOID    BaseAddress OPTIONAL,
    IN ULONG    FlushSize
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushBuffersFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushVirtualMemory (
    IN HANDLE               ProcessHandle,
    IN OUT PVOID            *BaseAddress,
    IN OUT PULONG           FlushSize,
    OUT PIO_STATUS_BLOCK    IoStatusBlock
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwFreeVirtualMemory (
    IN HANDLE       ProcessHandle,
    IN OUT PVOID    *BaseAddress,
    IN OUT PSIZE_T  RegionSize,
    IN ULONG        FreeType
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFsControlFile (
    IN HANDLE               FileHandle,
    IN HANDLE               Event OPTIONAL,
    IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
    IN PVOID                ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN ULONG                FsControlCode,
    IN PVOID                InputBuffer OPTIONAL,
    IN ULONG                InputBufferLength,
    OUT PVOID               OutputBuffer OPTIONAL,
    IN ULONG                OutputBufferLength
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwInitiatePowerAction (
    IN POWER_ACTION         SystemAction,
    IN SYSTEM_POWER_STATE   MinSystemState,
    IN ULONG                Flags,
    IN BOOLEAN              Asynchronous
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver (
    /* "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\<DriverName>" */
    IN PUNICODE_STRING RegistryPath
);

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadKey (
    IN POBJECT_ATTRIBUTES KeyObjectAttributes,
    IN POBJECT_ATTRIBUTES FileObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeKey (
    IN HANDLE               KeyHandle,
    IN HANDLE               EventHandle OPTIONAL,
    IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
    IN PVOID                ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN ULONG                NotifyFilter,
    IN BOOLEAN              WatchSubtree,
    IN PVOID                Buffer,
    IN ULONG                BufferLength,
    IN BOOLEAN              Asynchronous
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenDirectoryObject (
    OUT PHANDLE             DirectoryHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenEvent (
    OUT PHANDLE             EventHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcess (
    OUT PHANDLE             ProcessHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes,
    IN PCLIENT_ID           ClientId OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcessToken (
    IN HANDLE       ProcessHandle,
    IN ACCESS_MASK  DesiredAccess,
    OUT PHANDLE     TokenHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenThread (
    OUT PHANDLE             ThreadHandle,
    IN ACCESS_MASK          DesiredAccess,
    IN POBJECT_ATTRIBUTES   ObjectAttributes,
    IN PCLIENT_ID           ClientId
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenThreadToken (
    IN HANDLE       ThreadHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN BOOLEAN      OpenAsSelf,
    OUT PHANDLE     TokenHandle
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwPowerInformation (
    IN POWER_INFORMATION_LEVEL  PowerInformationLevel,
    IN PVOID                    InputBuffer OPTIONAL,
    IN ULONG                    InputBufferLength,
    OUT PVOID                   OutputBuffer OPTIONAL,
    IN ULONG                    OutputBufferLength
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwPulseEvent (
    IN HANDLE   EventHandle,
    OUT PLONG   PreviousState OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDefaultLocale (
    IN BOOLEAN  ThreadOrSystem,
    OUT PLCID   Locale
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile (
    IN HANDLE                   FileHandle,
    IN HANDLE                   Event OPTIONAL,
    IN PIO_APC_ROUTINE          ApcRoutine OPTIONAL,
    IN PVOID                    ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK        IoStatusBlock,
    OUT PVOID                   FileInformation,
    IN ULONG                    Length,
    IN FILE_INFORMATION_CLASS   FileInformationClass,
    IN BOOLEAN                  ReturnSingleEntry,
    IN PUNICODE_STRING          FileName OPTIONAL,
    IN BOOLEAN                  RestartScan
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryObject (
    IN HANDLE       DirectoryHandle,
    OUT PVOID       Buffer,
    IN ULONG        Length,
    IN BOOLEAN      ReturnSingleEntry,
    IN BOOLEAN      RestartScan,
    IN OUT PULONG   Context,
    OUT PULONG      ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryEaFile (
    IN HANDLE               FileHandle,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    OUT PVOID               Buffer,
    IN ULONG                Length,
    IN BOOLEAN              ReturnSingleEntry,
    IN PVOID                EaList OPTIONAL,
    IN ULONG                EaListLength,
    IN PULONG               EaIndex OPTIONAL,
    IN BOOLEAN              RestartScan
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess (
    IN HANDLE           ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID           ProcessInformation,
    IN ULONG            ProcessInformationLength,
    OUT PULONG          ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationToken (
    IN HANDLE                   TokenHandle,
    IN TOKEN_INFORMATION_CLASS  TokenInformationClass,
    OUT PVOID                   TokenInformation,
    IN ULONG                    Length,
    OUT PULONG                  ResultLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySecurityObject (
    IN HANDLE                   FileHandle,
    IN SECURITY_INFORMATION     SecurityInformation,
    OUT PSECURITY_DESCRIPTOR    SecurityDescriptor,
    IN ULONG                    Length,
    OUT PULONG                  ResultLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVolumeInformationFile (
    IN HANDLE               FileHandle,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    OUT PVOID               FsInformation,
    IN ULONG                Length,
    IN FS_INFORMATION_CLASS FsInformationClass
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReplaceKey (
    IN POBJECT_ATTRIBUTES   NewFileObjectAttributes,
    IN HANDLE               KeyHandle,
    IN POBJECT_ATTRIBUTES   OldFileObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwResetEvent (
    IN HANDLE   EventHandle,
    OUT PLONG   PreviousState OPTIONAL
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwRestoreKey (
    IN HANDLE   KeyHandle,
    IN HANDLE   FileHandle,
    IN ULONG    Flags
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwSaveKey (
    IN HANDLE KeyHandle,
    IN HANDLE FileHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetDefaultLocale (
    IN BOOLEAN  ThreadOrSystem,
    IN LCID     Locale
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwSetDefaultUILanguage (
    IN LANGID LanguageId
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEaFile (
    IN HANDLE               FileHandle,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    OUT PVOID               Buffer,
    IN ULONG                Length
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEvent (
    IN HANDLE   EventHandle,
    OUT PLONG   PreviousState OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationProcess (
    IN HANDLE           ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    IN PVOID            ProcessInformation,
    IN ULONG            ProcessInformationLength
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSecurityObject (
    IN HANDLE               Handle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSystemTime (
    IN PLARGE_INTEGER   NewTime,
    OUT PLARGE_INTEGER  OldTime OPTIONAL
);

#if (VER_PRODUCTBUILD >= 2195)

NTSYSAPI
NTSTATUS
NTAPI
ZwSetVolumeInformationFile (
    IN HANDLE               FileHandle,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN PVOID                FsInformation,
    IN ULONG                Length,
    IN FS_INFORMATION_CLASS FsInformationClass
);

#endif /* (VER_PRODUCTBUILD >= 2195) */

NTSYSAPI
NTSTATUS
NTAPI
ZwTerminateProcess (
    IN HANDLE   ProcessHandle OPTIONAL,
    IN NTSTATUS ExitStatus
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver (
    /* "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\<DriverName>" */
    IN PUNICODE_STRING RegistryPath
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadKey (
    IN POBJECT_ATTRIBUTES KeyObjectAttributes
);

#if (NTDDI_VERSION >= NTDDI_WIN2K)
NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForSingleObject (
  IN HANDLE Handle,
  IN BOOLEAN Alertable,
  IN PLARGE_INTEGER Timeout OPTIONAL);
#endif

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForMultipleObjects (
    IN ULONG            HandleCount,
    IN PHANDLE          Handles,
    IN WAIT_TYPE        WaitType,
    IN BOOLEAN          Alertable,
    IN PLARGE_INTEGER   Timeout OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwYieldExecution (
    VOID
);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
