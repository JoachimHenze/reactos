#ifndef __INCLUDE_DDK_LDRFUNCS_H
#define __INCLUDE_DDK_LDRFUNCS_H
/* $Id: ldrfuncs.h,v 1.5.22.1 2004/06/27 01:13:11 hyperion Exp $ */

NTSTATUS STDCALL
LdrAccessResource(IN  PVOID BaseAddress,
                  IN  PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
                  OUT PVOID *Resource OPTIONAL,
                  OUT PULONG Size OPTIONAL);

NTSTATUS STDCALL
LdrFindResource_U(IN  PVOID BaseAddress,
                  IN  PLDR_RESOURCE_INFO ResourceInfo,
                  IN  ULONG Level,
                  OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry);

/*NTSTATUS STDCALL 
LdrFindResourceDirectory_U(	IN PVOID   						BaseAddress,
							IN PLDR_RESOURCE_INFO			ResourceInfo,
							IN  ULONG						Level,
							OUT PIMAGE_RESOURCE_DIRECTORY	*ResourceDirectory);
*/
NTSTATUS STDCALL
LdrEnumResources(IN PVOID						BaseAddress,
				 IN PLDR_RESOURCE_INFO			ResourceInfo,
				 IN  ULONG						Level,
				 IN OUT PULONG					ResourceCount,
				 OUT PVOID						Resources  OPTIONAL);

#endif /* __INCLUDE_DDK_LDRFUNCS_H */
