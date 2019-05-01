#include "IO_Control.h"
#include "Kernel_Force_Delete.h"

#define DEVICE_NAME L"\\Device\\caonimabbb"
#define LINK_NAME L"\\??\\caonimabbb"

#define TEST_1 CTL_CODE(FILE_DEVICE_UNKNOWN,0x7100,METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define TEST_2 CTL_CODE(FILE_DEVICE_UNKNOWN,0x7101,METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define TEST_3 CTL_CODE(FILE_DEVICE_UNKNOWN,0x7102,METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define TEST_4 CTL_CODE(FILE_DEVICE_UNKNOWN,0x7103,METHOD_BUFFERED ,FILE_ANY_ACCESS)

DRIVER_OBJECT *IO_Control::Driver_Object = nullptr;

NTSTATUS IO_Control::Create_IO_Control()
{
	VMProtectBeginMutation("Create_IO_Control");
	
	NTSTATUS status = 0;
	//创建设备对象
	RtlInitUnicodeString(&Device_Name, DEVICE_NAME);
	status = IoCreateDevice(Driver_Object, 0, &Device_Name, FILE_DEVICE_UNKNOWN, 0, FALSE, &Device_Object);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	Device_Object->Flags |= DO_BUFFERED_IO;
	//创建符号连接
	RtlInitUnicodeString(&Link_Name, LINK_NAME);
	status = IoCreateSymbolicLink(&Link_Name, &Device_Name);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(Device_Object);
		return status;
	}

	Driver_Object->MajorFunction[IRP_MJ_CREATE] = IO_Control::IO_Default;
	Driver_Object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IO_Control::Code_Control_Center;

	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS IO_Control::Delete_IO_Control()
{
	VMProtectBeginMutation("Delete_IO_Control");
	IoDeleteSymbolicLink(&Link_Name);
	IoDeleteDevice(Device_Object);
	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS IO_Control::IO_Default(PDEVICE_OBJECT  DeviceObject, PIRP  pIrp)
{
	VMProtectBeginMutation("IO_Default");
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	VMProtectEnd();
	return STATUS_SUCCESS;
}

NTSTATUS IO_Control::Code_Control_Center(PDEVICE_OBJECT  DeviceObject, PIRP  pIrp)
{
	VMProtectBeginMutation("Code_Control_Center");
	PIO_STACK_LOCATION irp = IoGetCurrentIrpStackLocation(pIrp);
	ULONG Io_Control_Code = irp->Parameters.DeviceIoControl.IoControlCode;
	ULONG Input_Lenght = irp->Parameters.DeviceIoControl.InputBufferLength;
	ULONG Output_Lenght = irp->Parameters.DeviceIoControl.OutputBufferLength;

	if (Io_Control_Code == TEST_1)
	{
		MiniFilter_Monitor::ClearRule();
	}
	if (Io_Control_Code == TEST_2)
	{
		RULE_FILE_PATH *temp_rule = new RULE_FILE_PATH();
		RtlCopyMemory(temp_rule, pIrp->AssociatedIrp.SystemBuffer, Input_Lenght);
		MiniFilter_Monitor::Add_Rule(temp_rule);
	}
	if (Io_Control_Code == TEST_3)
	{
		ULONG64 temp_addr = 0;
		RtlCopyMemory(&temp_addr, pIrp->AssociatedIrp.SystemBuffer, Input_Lenght);
		_MiProcessLoaderEntry MiProcessLoaderEntry = (_MiProcessLoaderEntry)temp_addr;
		if (MiProcessLoaderEntry)
		{
			MiProcessLoaderEntry(Driver_Object->DriverSection, 0);
			Driver_Object->DriverSection = NULL;
			Driver_Object->DriverStart = NULL;
			Driver_Object->DriverSize = NULL;
			Driver_Object->DriverUnload = NULL;
			Driver_Object->DriverInit = NULL;
			Driver_Object->DeviceObject = NULL;
		}
	}
	if (Io_Control_Code == TEST_4)
	{
		WCHAR *temp_path = new WCHAR[Input_Lenght];
		RtlCopyMemory(temp_path, pIrp->AssociatedIrp.SystemBuffer, Input_Lenght);
		Kernel_Force_Delete::Delete_File_Mode1(temp_path);
	}


	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	VMProtectEnd();
	return STATUS_SUCCESS;
}