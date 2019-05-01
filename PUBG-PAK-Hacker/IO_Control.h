#pragma once
#include "Common_Header.h"
#include "MiniFilter-Monitor.h"


class IO_Control
{
public:
	IO_Control(PDRIVER_OBJECT drive_object)
	{
		Driver_Object = drive_object;
	}
	~IO_Control() = default;
public:
	NTSTATUS Create_IO_Control();
	NTSTATUS Delete_IO_Control();
private:
	static NTSTATUS IO_Default(PDEVICE_OBJECT  DeviceObject, PIRP  pIrp);
	static NTSTATUS Code_Control_Center(PDEVICE_OBJECT  DeviceObject, PIRP  pIrp);
private:
	static DRIVER_OBJECT *Driver_Object;
	DEVICE_OBJECT *Device_Object = nullptr;
	UNICODE_STRING Device_Name;
	UNICODE_STRING Link_Name;
};

