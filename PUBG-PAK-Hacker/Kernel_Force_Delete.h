#include "Common_Header.h"
#include "NtSysAPI_Func.hpp"


class Kernel_Force_Delete
{
public:
	Kernel_Force_Delete()
	{
		
	}
	~Kernel_Force_Delete() = default;
public:
	static bool Delete_File_Mode1(wchar_t *);
};



