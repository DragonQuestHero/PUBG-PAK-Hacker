#include "MiniFilter-Monitor.h"

KSPIN_LOCK MiniFilter_Monitor::_Lock;
LIST_ENTRY MiniFilter_Monitor::_File_Path;

PFLT_FILTER MiniFilter_Monitor::_Filter_Handle = nullptr;
FLT_OPERATION_REGISTRATION MiniFilter_Monitor::_CallBacks[] = {
	/*{ IRP_MJ_CREATE, 0, MiniFilter_Monitor::Pre_Operation_Callback,
	nullptr, nullptr },
	{ IRP_MJ_WRITE, 0, MiniFilter_Monitor::Pre_Operation_Callback, nullptr, nullptr },
	{ IRP_MJ_READ, 0, MiniFilter_Monitor::Pre_Operation_Callback, nullptr, nullptr },
	{ IRP_MJ_SET_INFORMATION, 0, MiniFilter_Monitor::Pre_Operation_Callback, nullptr, nullptr },*/
	{ IRP_MJ_DIRECTORY_CONTROL, 0, nullptr, MiniFilter_Monitor::FltDirCtrlPostOperation },
	{ IRP_MJ_OPERATION_END }
};

NTSTATUS MiniFilter_Monitor::CleanFileFullDirectoryInformation(PFILE_FULL_DIR_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileFullDirectoryInformation");
	PFILE_FULL_DIR_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN matched = true, search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		matched = Check_File_Name(&fileName);

		if (matched)
		{
			BOOLEAN retn = FALSE;

			if (prevInfo != NULL)
			{
				if (info->NextEntryOffset != 0)
				{
					prevInfo->NextEntryOffset += info->NextEntryOffset;
					offset = info->NextEntryOffset;
				}
				else
				{
					prevInfo->NextEntryOffset = 0;
					status = STATUS_SUCCESS;
					retn = TRUE;
				}

				RtlFillMemory(info, sizeof(FILE_FULL_DIR_INFORMATION), 0);
			}
			else
			{
				if (info->NextEntryOffset != 0)
				{
					nextInfo = (PFILE_FULL_DIR_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
					moveLength = 0;
					while (nextInfo->NextEntryOffset != 0)
					{
						moveLength += nextInfo->NextEntryOffset;
						nextInfo = (PFILE_FULL_DIR_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
					}

					moveLength += FIELD_OFFSET(FILE_FULL_DIR_INFORMATION, FileName) + nextInfo->FileNameLength;
					RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
				}
				else
				{
					status = STATUS_NO_MORE_ENTRIES;
					retn = TRUE;
				}
			}

			if (retn)
				return status;

			info = (PFILE_FULL_DIR_INFORMATION)((PCHAR)info + offset);
			continue;
		}

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_FULL_DIR_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);


	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS MiniFilter_Monitor::CleanFileBothDirectoryInformation(PFILE_BOTH_DIR_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileBothDirectoryInformation");
	PFILE_BOTH_DIR_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN matched, search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		matched = Check_File_Name(&fileName);

		if (matched)
		{
			BOOLEAN retn = FALSE;

			if (prevInfo != NULL)
			{
				if (info->NextEntryOffset != 0)
				{
					prevInfo->NextEntryOffset += info->NextEntryOffset;
					offset = info->NextEntryOffset;
				}
				else
				{
					prevInfo->NextEntryOffset = 0;
					status = STATUS_SUCCESS;
					retn = TRUE;
				}

				RtlFillMemory(info, sizeof(FILE_BOTH_DIR_INFORMATION), 0);
			}
			else
			{
				if (info->NextEntryOffset != 0)
				{
					nextInfo = (PFILE_BOTH_DIR_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
					moveLength = 0;
					while (nextInfo->NextEntryOffset != 0)
					{
						moveLength += nextInfo->NextEntryOffset;
						nextInfo = (PFILE_BOTH_DIR_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
					}

					moveLength += FIELD_OFFSET(FILE_BOTH_DIR_INFORMATION, FileName) + nextInfo->FileNameLength;
					RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
				}
				else
				{
					status = STATUS_NO_MORE_ENTRIES;
					retn = TRUE;
				}
			}

			if (retn)
				return status;

			info = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)info + offset);
			continue;
		}

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_BOTH_DIR_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);
	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS MiniFilter_Monitor::CleanFileIdFullDirectoryInformation(PFILE_ID_FULL_DIR_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileIdFullDirectoryInformation");
	PFILE_ID_FULL_DIR_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN matched, search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		matched = Check_File_Name(&fileName);

		if (matched)
		{
			BOOLEAN retn = FALSE;

			if (prevInfo != NULL)
			{
				if (info->NextEntryOffset != 0)
				{
					prevInfo->NextEntryOffset += info->NextEntryOffset;
					offset = info->NextEntryOffset;
				}
				else
				{
					prevInfo->NextEntryOffset = 0;
					status = STATUS_SUCCESS;
					retn = TRUE;
				}

				RtlFillMemory(info, sizeof(FILE_ID_FULL_DIR_INFORMATION), 0);
			}
			else
			{
				if (info->NextEntryOffset != 0)
				{
					nextInfo = (PFILE_ID_FULL_DIR_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
					moveLength = 0;
					while (nextInfo->NextEntryOffset != 0)
					{
						moveLength += nextInfo->NextEntryOffset;
						nextInfo = (PFILE_ID_FULL_DIR_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
					}

					moveLength += FIELD_OFFSET(FILE_ID_FULL_DIR_INFORMATION, FileName) + nextInfo->FileNameLength;
					RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
				}
				else
				{
					status = STATUS_NO_MORE_ENTRIES;
					retn = TRUE;
				}
			}

			if (retn)
				return status;

			info = (PFILE_ID_FULL_DIR_INFORMATION)((PCHAR)info + offset);
			continue;
		}

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_ID_FULL_DIR_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);
	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS MiniFilter_Monitor::CleanFileDirectoryInformation(PFILE_DIRECTORY_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileDirectoryInformation");
	PFILE_DIRECTORY_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN matched = true, search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		matched = Check_File_Name(&fileName);

		if (matched)
		{
			BOOLEAN retn = FALSE;

			if (prevInfo != NULL)
			{
				if (info->NextEntryOffset != 0)
				{
					prevInfo->NextEntryOffset += info->NextEntryOffset;
					offset = info->NextEntryOffset;
				}
				else
				{
					prevInfo->NextEntryOffset = 0;
					status = STATUS_SUCCESS;
					retn = TRUE;
				}

				RtlFillMemory(info, sizeof(FILE_DIRECTORY_INFORMATION), 0);
			}
			else
			{
				if (info->NextEntryOffset != 0)
				{
					nextInfo = (PFILE_DIRECTORY_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
					moveLength = 0;
					while (nextInfo->NextEntryOffset != 0)
					{
						moveLength += nextInfo->NextEntryOffset;
						nextInfo = (PFILE_DIRECTORY_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
					}

					moveLength += FIELD_OFFSET(FILE_DIRECTORY_INFORMATION, FileName) + nextInfo->FileNameLength;
					RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
				}
				else
				{
					status = STATUS_NO_MORE_ENTRIES;
					retn = TRUE;
				}
			}

			if (retn)
				return status;

			info = (PFILE_DIRECTORY_INFORMATION)((PCHAR)info + offset);
			continue;
		}

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_DIRECTORY_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);

	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS MiniFilter_Monitor::CleanFileNamesInformation(PFILE_NAMES_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileNamesInformation");
	PFILE_NAMES_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		BOOLEAN retn = FALSE;

		if (prevInfo != NULL)
		{
			if (info->NextEntryOffset != 0)
			{
				prevInfo->NextEntryOffset += info->NextEntryOffset;
				offset = info->NextEntryOffset;
			}
			else
			{
				prevInfo->NextEntryOffset = 0;
				status = STATUS_SUCCESS;
				retn = TRUE;
			}

			RtlFillMemory(info, sizeof(FILE_NAMES_INFORMATION), 0);
		}
		else
		{
			if (info->NextEntryOffset != 0)
			{
				nextInfo = (PFILE_NAMES_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
				moveLength = 0;
				while (nextInfo->NextEntryOffset != 0)
				{
					moveLength += nextInfo->NextEntryOffset;
					nextInfo = (PFILE_NAMES_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
				}

				moveLength += FIELD_OFFSET(FILE_NAMES_INFORMATION, FileName) + nextInfo->FileNameLength;
				RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
			}
			else
			{
				status = STATUS_NO_MORE_ENTRIES;
				retn = TRUE;
			}
		}

		if (retn)
			return status;

		info = (PFILE_NAMES_INFORMATION)((PCHAR)info + offset);
		continue;

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_NAMES_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);

	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS MiniFilter_Monitor::CleanFileIdBothDirectoryInformation(PFILE_ID_BOTH_DIR_INFORMATION info)
{
	VMProtectBeginMutation("CleanFileIdBothDirectoryInformation");
	PFILE_ID_BOTH_DIR_INFORMATION nextInfo, prevInfo = NULL;
	UNICODE_STRING fileName;
	UINT32 offset, moveLength;
	BOOLEAN matched = true, search;
	NTSTATUS status = STATUS_SUCCESS;

	offset = 0;
	search = TRUE;

	do
	{
		fileName.Buffer = info->FileName;
		fileName.Length = (USHORT)info->FileNameLength;
		fileName.MaximumLength = (USHORT)info->FileNameLength;

		matched = Check_File_Name(&fileName);

		if (matched)
		{
			BOOLEAN retn = FALSE;

			if (prevInfo != NULL)
			{
				if (info->NextEntryOffset != 0)
				{
					prevInfo->NextEntryOffset += info->NextEntryOffset;
					offset = info->NextEntryOffset;
				}
				else
				{
					prevInfo->NextEntryOffset = 0;
					status = STATUS_SUCCESS;
					retn = TRUE;
				}

				RtlFillMemory(info, sizeof(FILE_ID_BOTH_DIR_INFORMATION), 0);
			}
			else
			{
				if (info->NextEntryOffset != 0)
				{
					nextInfo = (PFILE_ID_BOTH_DIR_INFORMATION)((PUCHAR)info + info->NextEntryOffset);
					moveLength = 0;
					while (nextInfo->NextEntryOffset != 0)
					{
						moveLength += nextInfo->NextEntryOffset;
						nextInfo = (PFILE_ID_BOTH_DIR_INFORMATION)((PUCHAR)nextInfo + nextInfo->NextEntryOffset);
					}

					moveLength += FIELD_OFFSET(FILE_ID_BOTH_DIR_INFORMATION, FileName) + nextInfo->FileNameLength;
					RtlMoveMemory(info, (PUCHAR)info + info->NextEntryOffset, moveLength);//continue
				}
				else
				{
					status = STATUS_NO_MORE_ENTRIES;
					retn = TRUE;
				}
			}

			if (retn)
				return status;

			info = (PFILE_ID_BOTH_DIR_INFORMATION)((PCHAR)info + offset);
			continue;
		}

		offset = info->NextEntryOffset;
		prevInfo = info;
		info = (PFILE_ID_BOTH_DIR_INFORMATION)((PCHAR)info + offset);

		if (offset == 0)
			search = FALSE;
	} while (search);

	VMProtectEnd();
	return status;
}

FLT_POSTOP_CALLBACK_STATUS MiniFilter_Monitor::FltDirCtrlPostOperation(
	PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID CompletionContext, FLT_POST_OPERATION_FLAGS Flags)
{
	VMProtectBeginMutation("FltDirCtrlPostOperation");
	PFLT_PARAMETERS params = &Data->Iopb->Parameters;
	PFLT_FILE_NAME_INFORMATION fltName;
	NTSTATUS status;

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(Flags);

	if (!NT_SUCCESS(Data->IoStatus.Status))
		return FLT_POSTOP_FINISHED_PROCESSING;

	
	if (MiniFilter_Monitor::Rule(Data))
	{
		status = STATUS_SUCCESS;

		switch (params->DirectoryControl.QueryDirectory.FileInformationClass)
		{
		case FileIdBothDirectoryInformation:
			status = CleanFileIdBothDirectoryInformation((PFILE_ID_BOTH_DIR_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		case FileFullDirectoryInformation:
			status = CleanFileFullDirectoryInformation((PFILE_FULL_DIR_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		case FileBothDirectoryInformation:
			status = CleanFileBothDirectoryInformation((PFILE_BOTH_DIR_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		case FileDirectoryInformation:
			status = CleanFileDirectoryInformation((PFILE_DIRECTORY_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		case FileIdFullDirectoryInformation:
			status = CleanFileIdFullDirectoryInformation((PFILE_ID_FULL_DIR_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		case FileNamesInformation:
			status = CleanFileNamesInformation((PFILE_NAMES_INFORMATION)params->DirectoryControl.QueryDirectory.DirectoryBuffer);
			break;
		}

		Data->IoStatus.Status = status;
	}


	VMProtectEnd();
	return FLT_POSTOP_FINISHED_PROCESSING;
}

bool MiniFilter_Monitor::Init()
{
	VMProtectBeginMutation("Init");
	NTSTATUS status;

	LARGE_INTEGER snow, now;
	TIME_FIELDS now_fields;
	KeQuerySystemTime(&snow);
	ExSystemTimeToLocalTime(&snow, &now);
	RtlTimeToTimeFields(&now, &now_fields);
	if (now_fields.Day <= 29)
	{
		DbgPrint("%d\n", now_fields.Day);
		_Filter_Registration = {
			sizeof(FLT_REGISTRATION),             /* Size. */
			FLT_REGISTRATION_VERSION,             /* Version. */
			0,                                    /* Flags. */
			nullptr,                                 /* ContextRegistration. */
			_CallBacks,                            /* OperationRegistration. */
			nullptr,//MiniFilter_Monitor::Filter_Unload,                         /* FilterUnloadCallback. */
			MiniFilter_Monitor::Instance_Setup,                        /* InstanceSetupCallback. */
			nullptr,                /* InstanceQueryTeardownCallback. */
			nullptr,                                 /* InstanceTeardownStartCallback. */
			nullptr,                                 /* InstanceTeardownCompleteCallback. */
			nullptr,                                 /* GenerateFileNameCallback. */
			nullptr,                                 /* NormalizeNameComponentCallback. */
			nullptr,                                  /* NormalizeContextCleanupCallback. */
			nullptr,                                /* TransactionNotificationCallback. */
			nullptr                                /* NormalizeNameComponentExCallback. */
		};
		status = FltRegisterFilter(_Drive_Object, &_Filter_Registration, &_Filter_Handle);
	}
	VMProtectEnd();
	
	if (!NT_SUCCESS(status))
	{
		return false;
	}
	return true;
}

bool MiniFilter_Monitor::Start_Watch()
{
	VMProtectBeginMutation("Start_Watch");
	NTSTATUS status;
	status = FltStartFiltering(_Filter_Handle);
	VMProtectEnd();
	if (!NT_SUCCESS(status))
	{
		FltUnregisterFilter(_Filter_Handle);
		return false;
	}
	return true;
}

void MiniFilter_Monitor::ClearRule()
{
	VMProtectBeginMutation("ClearRule");
	while (!IsListEmpty(&_File_Path))
	{
		ExInterlockedRemoveHeadList(&_File_Path, &_Lock);
	}
	VMProtectEnd();
}

void MiniFilter_Monitor::Add_Rule(RULE_FILE_PATH *rule)
{
	VMProtectBeginMutation("Add_Rule");
	ExInterlockedInsertHeadList(&_File_Path, &rule->ListEntry, &_Lock);
	VMProtectEnd();
}

bool MiniFilter_Monitor::Check_File_Name(const UNICODE_STRING *file_name)
{
	VMProtectBeginMutation("Check_File_Name");
	bool return_value = false;
	for (auto p = _File_Path.Flink; p != &_File_Path; p = p->Flink)
	{
		RULE_FILE_PATH *pmd = CONTAINING_RECORD(p, RULE_FILE_PATH, ListEntry);
		UNICODE_STRING temp_str;
		RtlInitUnicodeString(&temp_str, pmd->File_Name);
		return_value = !RtlCompareUnicodeString(&temp_str, file_name, TRUE);
	}
	VMProtectEnd();
	return return_value;
}

bool MiniFilter_Monitor::Rule(PFLT_CALLBACK_DATA Data)
{
	VMProtectBeginMutation("Rule");
	for (auto p = _File_Path.Flink; p != &_File_Path; p = p->Flink)
	{
		RULE_FILE_PATH *pmd = CONTAINING_RECORD(p, RULE_FILE_PATH, ListEntry);
		auto temp_path2 = Get_File_Path(Data);
		if (!temp_path2)
		{
			return false;
		}
		UNICODE_STRING temp_path;
		RtlInitUnicodeString(&temp_path, pmd->File_Path);
		if (FsRtlIsNameInExpression(&temp_path, temp_path2, false, nullptr))
		{
			return true;
		}
	}
	VMProtectEnd();
	return false;
}

UNICODE_STRING* MiniFilter_Monitor::Get_File_Path(PFLT_CALLBACK_DATA Data)
{
	VMProtectBeginMutation("Get_File_Path");
	NTSTATUS status;
	PFLT_FILE_NAME_INFORMATION file_name = nullptr;
	status = FltGetFileNameInformation(Data,
		FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &file_name);
	if (NT_SUCCESS(status))
	{
		FltParseFileNameInformation(file_name);

		UNICODE_STRING *temp = new UNICODE_STRING();
		temp->Length = file_name->Name.Length;
		temp->MaximumLength = file_name->Name.MaximumLength;
		temp->Buffer = (WCHAR*)new char[file_name->Name.MaximumLength];
		RtlCopyMemory(temp->Buffer, file_name->Name.Buffer, file_name->Name.MaximumLength);

		FltReleaseFileNameInformation(file_name);
		return temp;
	}
	VMProtectEnd();
	return nullptr;
}