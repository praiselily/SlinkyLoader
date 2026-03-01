#include <windows.h>
#include <tlhelp32.h>
#include <urlmon.h>
#include <shellapi.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")
struct DLLEntry {
    std::string name;
    std::string url;
};

const std::vector<DLLEntry> DLLS = {
    { "slinky_library.dll", "https://github.com/praiselily/SlinkyLoader/releases/download/Cracked/slinky_library.dll" },
    { "slinkyhook.dll",     "https://github.com/praiselily/SlinkyLoader/releases/download/Cracked/slinkyhook.dll"     }
};
HANDLE hConsole;

enum Color {
    C_WHITE   = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    C_GREY    = FOREGROUND_INTENSITY,
    C_GREEN   = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    C_YELLOW  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    C_RED     = FOREGROUND_RED | FOREGROUND_INTENSITY,
    C_CYAN    = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    C_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

void SetColor(Color c) { SetConsoleTextAttribute(hConsole, (WORD)c); }
void ResetColor()       { SetConsoleTextAttribute(hConsole, C_WHITE); }

void Divider() {
    SetColor(C_GREY);
    std::cout << "  -----------------------------------------------------\n";
    ResetColor();
}

void Log(const std::string& type, const std::string& msg) {
    if      (type == "ok")   { SetColor(C_GREEN);  std::cout << "  [+] "; }
    else if (type == "info") { SetColor(C_CYAN);   std::cout << "  [*] "; }
    else if (type == "warn") { SetColor(C_YELLOW); std::cout << "  [!] "; }
    else if (type == "err")  { SetColor(C_RED);    std::cout << "  [x] "; }
    ResetColor();
    std::cout << msg << "\n";
}

void Prompt(const std::string& msg) {
    SetColor(C_YELLOW); std::cout << "  [?] ";
    ResetColor();       std::cout << msg;
}

void PrintBanner() {
    SetColor(C_CYAN);
    std::cout << "\n";
    std::cout << "      __    __\n";
    std::cout << "      \\/----\\/\n";
    std::cout << "       \\0  0/    WOOF!\n";
    std::cout << "       _\\  /_\n";
    std::cout << "     _|  \\/  |_\n";
    std::cout << "    | | |  | | |\n";
    std::cout << "   _| | |  | | |_\n";
    std::cout << "  \"---|_|--|_|---\"\n";
    SetColor(C_MAGENTA);
    std::cout << "\n            - SLINKY!\n";
    SetColor(C_GREY);
    std::cout << "\n  -----------------------------------------------------\n";
    std::cout << "        DLLs provided by Pluto Solutions\n";
    std::cout << "               Made with love by lily\n";
    std::cout << "  -----------------------------------------------------\n";
    ResetColor();
    std::cout << "\n";
}
std::string GetCurrentDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    return path.substr(0, path.find_last_of("\\/"));
}
bool DLLExists(const std::string& path) {
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}
bool AddDefenderExclusion(const std::string& path) {
    std::string args = "-Command \"Add-MpPreference -ExclusionPath '" + path + "'\"";
    HINSTANCE result = ShellExecuteA(NULL, "runas", "powershell.exe", args.c_str(), NULL, SW_HIDE);
    Sleep(2000);
    return (intptr_t)result > 32;
}
bool DownloadDLL(const std::string& url, const std::string& outputPath) {
    DeleteFileA(outputPath.c_str());
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), outputPath.c_str(), 0, NULL);
    return SUCCEEDED(hr);
}
DWORD FindJavawPID() {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, "javaw.exe") == 0) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &pe));
    }
    CloseHandle(snapshot);
    return pid;
}
bool InjectDLL(DWORD pid, const std::string& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        Log("err", "Failed to open process. Error: " + std::to_string(GetLastError()));
        return false;
    }
    BOOL injectorWow64 = FALSE, targetWow64 = FALSE;
    IsWow64Process(GetCurrentProcess(), &injectorWow64);
    IsWow64Process(hProcess, &targetWow64);
    if (injectorWow64 != targetWow64) {
        Log("err", "Architecture mismatch between injector and target process.");
        CloseHandle(hProcess);
        return false;
    }
    SIZE_T pathSize = dllPath.size() + 1;
    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        Log("err", "VirtualAllocEx failed. Error: " + std::to_string(GetLastError()));
        CloseHandle(hProcess);
        return false;
    }
    if (!WriteProcessMemory(hProcess, remoteMemory, dllPath.c_str(), pathSize, NULL)) {
        Log("err", "WriteProcessMemory failed. Error: " + std::to_string(GetLastError()));
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    LPVOID loadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!loadLibAddr) {
        Log("err", "Failed to get LoadLibraryA address.");
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibAddr, remoteMemory, 0, NULL);
    if (!hThread) {
        Log("err", "CreateRemoteThread failed. Error: " + std::to_string(GetLastError()));
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    WaitForSingleObject(hThread, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);
    if (exitCode == 0) {
        Log("err", "LoadLibrary returned NULL - DLL may be corrupt or wrong architecture.");
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return true;
}
int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitleA("Pluto Solutions - Injector");
    PrintBanner();
    std::string currentDir = GetCurrentDir();
    Divider();
    AddDefenderExclusion(currentDir);
    Log("ok", "Defender exclusion applied.");
    std::cout << "\n";
    Divider();
    bool anyMissing = false;
    for (const auto& dll : DLLS) {
        if (!DLLExists(currentDir + "\\" + dll.name)) {
            anyMissing = true;
            break;
        }
    }
    if (anyMissing) {
        Log("info", "Extracting slinky_library and slinkyhook...");
        for (const auto& dll : DLLS) {
            std::string dllPath = currentDir + "\\" + dll.name;
            if (!DLLExists(dllPath)) {
                if (!DownloadDLL(dll.url, dllPath)) {
                    Log("err", "Failed to obtain " + dll.name + ". Exiting.");
                    std::cout << "\n";
                    Prompt("Press Enter to exit...");
                    std::cin.get();
                    return 1;
                }
            }
        }
        Log("ok", "Modules ready.");
    }
    std::cout << "\n";
    Divider();
    DWORD targetPID = 0;
    DWORD javawPID  = FindJavawPID();
    if (javawPID != 0) {
        Log("ok", "Found javaw.exe (PID: " + std::to_string(javawPID) + ") - injecting automatically...");
        targetPID = javawPID;
    } else {
        Log("warn", "No javaw.exe process found.");
        Prompt("Enter target PID: ");
        std::cin >> targetPID;
    }
    std::cout << "\n";
    Divider();
    bool allSuccess = true;
    for (const auto& dll : DLLS) {
        std::string dllPath = currentDir + "\\" + dll.name;
        Log("info", "Injecting " + dll.name + "...");
        if (InjectDLL(targetPID, dllPath)) {
            Log("ok", dll.name + " injected successfully.");
        } else {
            Log("err", dll.name + " injection failed.");
            allSuccess = false;
        }
    }
    std::cout << "\n";
    Divider();
    if (!allSuccess) {
        Log("warn", "One or more injections failed. Check errors above.");
        std::cout << "\n";
        Divider();
    }
    SetColor(C_GREY);
    std::cout << "  https://github.com/praiselily/SlinkyLoader\n";
    ResetColor();
    Divider();
    std::cout << "\n";
    Prompt("Press Enter to exit...");
    std::cin.ignore();
    std::cin.get();
    return 0;
}
