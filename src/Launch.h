#pragma once

#include <Windows.h>
#include <string>

DWORD launchRocketLeague(HANDLE& hProcess);
void injectDLLs(HANDLE hProcess);

void tryHideRocketLeagueWindow(DWORD pid);