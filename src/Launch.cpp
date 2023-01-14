#include "Launch.h"

#include <Windows.h>
#include <oleacc.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include "Shlwapi.h"

#include <iostream>
#include <thread>
#include <fstream>
#include <filesystem>
#include <stdexcept>



void tryHideRocketLeagueWindow(DWORD pid) {
    EnumWindows([](HWND hWnd, LPARAM pid) -> BOOL {
        DWORD pidWnd;
        GetWindowThreadProcessId(hWnd, &pidWnd);
        if (pid == pidWnd) {
            if (0)
                ShowWindow(hWnd, SW_HIDE); // hides the game
            PostMessageA(hWnd, WM_KILLFOCUS, NULL, NULL); // unfocuses the game, causing it to mute itself
        }
        return true;
    }, pid);
}

void mute(DWORD RLPid) {
    using namespace std::chrono_literals;
    IMMDevice* mmDevice;
    IMMDeviceEnumerator* mmDeviceEnum = nullptr;
    IAudioSessionManager2* sessionManager;
    IAudioSessionEnumerator* sessionEnum;
    ISimpleAudioVolume* audioVolume;
    int sessionCount;

    while (true) {
        while (true) {
            if (S_OK == CoCreateInstance(__uuidof(MMDeviceEnumerator), 0, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&mmDeviceEnum))
                if (mmDeviceEnum != nullptr)
                    break;
            std::this_thread::sleep_for(10ms);
        }
        mmDeviceEnum->GetDefaultAudioEndpoint(eRender, eMultimedia, &mmDevice);
        mmDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, 0, (void**)&sessionManager);
        sessionManager->GetSessionEnumerator(&sessionEnum);

        sessionEnum->GetCount(&sessionCount);
        if (sessionCount > 0)
            break;

        std::this_thread::sleep_for(10ms);

    }
    for (int i = 0; i < sessionCount; i++) {
        IAudioSessionControl* sessionControl;
        IAudioSessionControl2* sessionControl2;
        sessionEnum->GetSession(i, &sessionControl);
        sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);

        DWORD pid;
        sessionControl2->GetProcessId(&pid);
        if (RLPid == pid) {
            sessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&audioVolume);
            audioVolume->SetMute(true, 0);
            audioVolume->Release();
        }

        sessionControl->Release();
        sessionControl2->Release();
    }

    sessionEnum->Release();
    sessionManager->Release();
    mmDevice->Release();
    mmDeviceEnum->Release();
}




TCHAR exePath[] = TEXT("C:\\Program Files\\Epic Games\\rocketleague\\Binaries\\Win64\\RocketLeague.exe");
TCHAR launchCommand[] = TEXT("RocketLeague.exe -EpicPortal -nomovie -SimulationInstance");
//TCHAR exePath[] = TEXT("C:\\Games\\Steamapps\\common\\rocketleague\\Binaries\\Win64\\RocketLeague.exe.unpacked.exe");
//TCHAR launchCommand[] = TEXT("RocketLeague.exe.unpacked.exe");

DWORD launchRocketLeague(HANDLE& hProcess) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcess(exePath,   // the path
        launchCommand,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    if (pi.dwProcessId == 0)
        throw std::runtime_error("Failed to launch rocket league");

    hProcess = pi.hProcess;
    CloseHandle(pi.hThread);

    // we dont have to mute, since we are launching with default settings and background audio is off by default
    //mute(pi.dwProcessId);

    return pi.dwProcessId;
}


#define BAKKESMOD_KEY L"Software\\BakkesMod\\AppPath"
#define BAKKESMOD_SUBKEY L"BakkesModPath"
std::wstring getBakkesModPath() {
    HKEY hKey;
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG nError;

    nError = RegOpenKeyExW(HKEY_CURRENT_USER, BAKKESMOD_KEY, 0, KEY_ALL_ACCESS, &hKey);
    if (ERROR_SUCCESS == nError) {
        nError = RegQueryValueExW(hKey, BAKKESMOD_SUBKEY, 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
        RegCloseKey(hKey);
        if (ERROR_SUCCESS == nError) {
            return szBuffer;
        }
    }
    return std::wstring();
}

void editBMCfg(std::wstring bmPath) {
    std::ifstream ifile(bmPath + L"cfg\\plugins.cfg", std::fstream::in);
    std::string line;
    while (std::getline(ifile, line))
        if (line == "plugin load rlsimbakkesplugin")
            return;
    ifile.close();
    std::ofstream ofile(bmPath + L"cfg\\plugins.cfg", std::fstream::out | std::fstream::app);
    ofile << "\nplugin load rlsimbakkesplugin";
    ofile.close();
}


void injectDLL(HANDLE hProcess, std::wstring path) {
    LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
    size_t pathSize = (path.size() + 1) * sizeof(wchar_t);
    LPVOID dereercomp = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, dereercomp, path.c_str(), pathSize, NULL);
    HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
    WaitForSingleObject(remoteThread, INFINITE);
    VirtualFreeEx(hProcess, dereercomp, pathSize, MEM_RELEASE);
    CloseHandle(remoteThread);
}



void injectBM(HANDLE hProcess) {
    auto bmPath = getBakkesModPath();
    injectDLL(hProcess, bmPath + L"dll\\bakkesmod.dll");
}
