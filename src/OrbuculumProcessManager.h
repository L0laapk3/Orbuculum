#pragma once


#ifndef NOMINMAX 
#define NOMINMAX
#endif
#include <windows.h>


class OrbuculumProcessManager {
protected:
	HANDLE hProcess = NULL;
	DWORD pid = 0;
public:
	OrbuculumProcessManager();
	~OrbuculumProcessManager();
};