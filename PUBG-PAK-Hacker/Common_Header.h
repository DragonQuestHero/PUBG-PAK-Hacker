#pragma once

#include <ntifs.h>
#include <ntstrsafe.h>
#include <Fltkernel.h>
#include "VMProtectDDK.h"


#ifdef _AMD64_
static void *operator new (size_t lBlockSize)
{
	return ExAllocatePoolWithTag(NonPagedPool, lBlockSize, 111);
}

static void operator delete(void *p)
{
	if (!p)
	{
		return;
	}
	ExFreePoolWithTag(p, 111);
}
#else
static void * __CRTDECL operator new (size_t lBlockSize)
{
	return ExAllocatePoolWithTag(NonPagedPool, lBlockSize, 111);
}

static void __CRTDECL operator delete(void *p)
{
	if (!p)
	{
		return;
	}
	ExFreePoolWithTag(p, 111);
}
#endif


struct RULE_FILE_PATH
{
	LIST_ENTRY ListEntry;
	WCHAR File_Path[1024];
	WCHAR File_Name[1024];
	RULE_FILE_PATH()
	{
		RtlZeroMemory(File_Path, 1024);
		RtlZeroMemory(File_Name, 1024);
	}
};

typedef NTSTATUS(__fastcall *_MiProcessLoaderEntry)(PVOID pDriverSection, int bLoad);