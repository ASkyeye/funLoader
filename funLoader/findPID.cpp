// original findPID.cpp class was found from EVA2 project from Orca, modified by me 
#include <windows.h>
#include <iostream>
#include <string>
#include <Tlhelp32.h>
#include "connector.h"
#include "sysopen.h"
#ifdef __cplusplus
extern "C" {
#endif 


    DWORD FindProcessId(const std::wstring& processName);


    int find() {
        std::wstring processName = L"explorer.exe";
        DWORD processID = FindProcessId(processName);
        return processID;
    }
    /*int find2() {
        std::wstring processName = L"notepad.exe";
        DWORD processID = FindProcessId(processName);
        return processID;
    
    }*/


    DWORD FindProcessId(const std::wstring& processName) {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);
        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (processesSnapshot == INVALID_HANDLE_VALUE)
            return 0;
        Process32First(processesSnapshot, &processInfo);
        if (!processName.compare(processInfo.szExeFile))
        {
            NtClose(processesSnapshot);
            return processInfo.th32ProcessID;
        }
        while (Process32Next(processesSnapshot, &processInfo))
        {
            if (!processName.compare(processInfo.szExeFile))
            {
                NtClose(processesSnapshot);
                return processInfo.th32ProcessID;
            }
        }
        NtClose(processesSnapshot);
        return 0;
    }

#ifdef __cplusplus
}
#endif
