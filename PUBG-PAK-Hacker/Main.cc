#include "MiniFilter-Monitor.h"
#include "IO_Control.h"
#include "Kernel_Force_Delete.h"

MiniFilter_Monitor *_MiniFilter_Monitor = nullptr;
IO_Control *_IO_Control = nullptr;

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT drive_object, PUNICODE_STRING path)
{
	VMProtectBeginMutation("DriverEntry");

	_MiniFilter_Monitor = new MiniFilter_Monitor(drive_object);
	if (_MiniFilter_Monitor->Init())
	{
		_MiniFilter_Monitor->Start_Watch();
	}
	_IO_Control = new IO_Control(drive_object);
	_IO_Control->Create_IO_Control();

	Kernel_Force_Delete::Delete_File_Mode1(L"\\??\\C:\\Kernel-Minifilter.sys");
	VMProtectEnd();

	return STATUS_SUCCESS;
}