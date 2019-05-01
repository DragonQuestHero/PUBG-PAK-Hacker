#pragma once
#include "Common_Header.h"

class MiniFilter_Monitor
{
public:
	MiniFilter_Monitor(DRIVER_OBJECT *Drive_Object)
	{
		_Drive_Object = Drive_Object;
		KeInitializeSpinLock(&_Lock);
		InitializeListHead(&_File_Path);
	}
	~MiniFilter_Monitor()
	{
	}
public:
	bool Init();
	bool Start_Watch();
	static bool GetRule();
	static void ClearRule();
	static void Add_Rule(RULE_FILE_PATH *rule);
	static bool Check_File_Name(const UNICODE_STRING*);
	static void GetPathRule();
	static bool Rule(PFLT_CALLBACK_DATA Data);
private:
	DRIVER_OBJECT *_Drive_Object = nullptr;
	FLT_REGISTRATION _Filter_Registration;
	static KSPIN_LOCK _Lock;
	static LIST_ENTRY _File_Path;
	static PFLT_FILTER _Filter_Handle;
	static FLT_OPERATION_REGISTRATION _CallBacks[];
public:
	static NTSTATUS NTAPI Filter_Unload(FLT_FILTER_UNLOAD_FLAGS Flags)
	{
		FltUnregisterFilter(_Filter_Handle);
		return STATUS_SUCCESS;
	}
private:
	static NTSTATUS NTAPI Instance_Setup(PCFLT_RELATED_OBJECTS FltObjects,
		_In_ FLT_INSTANCE_SETUP_FLAGS Flags,
		_In_ DEVICE_TYPE VolumeDeviceType,
		_In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType)
	{
		return (VolumeDeviceType != FILE_DEVICE_CD_ROM_FILE_SYSTEM) ?
		STATUS_SUCCESS : STATUS_FLT_DO_NOT_ATTACH;
	}
public:
	static UNICODE_STRING* Get_File_Path(PFLT_CALLBACK_DATA Data);
	static NTSTATUS CleanFileIdBothDirectoryInformation(PFILE_ID_BOTH_DIR_INFORMATION info);
	static FLT_PREOP_CALLBACK_STATUS FltDirCtrlPreOperation(PFLT_CALLBACK_DATA Data,
		PCFLT_RELATED_OBJECTS FltObjects, PVOID *CompletionContext);
	static FLT_POSTOP_CALLBACK_STATUS FltDirCtrlPostOperation(PFLT_CALLBACK_DATA Data,
		PCFLT_RELATED_OBJECTS FltObjects, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
	static NTSTATUS CleanFileBothDirectoryInformation(PFILE_BOTH_DIR_INFORMATION info);
	static NTSTATUS CleanFileDirectoryInformation(PFILE_DIRECTORY_INFORMATION info);
	static NTSTATUS CleanFileIdFullDirectoryInformation(PFILE_ID_FULL_DIR_INFORMATION info);
	static NTSTATUS CleanFileNamesInformation(PFILE_NAMES_INFORMATION info);
	static NTSTATUS CleanFileFullDirectoryInformation(PFILE_FULL_DIR_INFORMATION info);
};

