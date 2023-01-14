#pragma once


#ifndef NOMINMAX 
#define NOMINMAX
#endif
#include <windows.h>


class OrbuculumProcess {
protected:
	HANDLE hProcess = NULL;
	DWORD pid = 0;
public:
	OrbuculumProcess();
	~OrbuculumProcess();
};