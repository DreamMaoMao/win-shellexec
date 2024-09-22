#include <windows.h>
#include <iostream>
#include <string>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

int main(int argc, char* argv[]) {
    if (argc < 2) {
        MessageBox(NULL, "Usage: <command> [arguments]", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    std::string command = argv[1];
    std::string parameters;

    for (int i = 2; i < argc; ++i) {
        parameters += argv[i];
        if (i < argc - 1) {
            parameters += " ";
        }
    }

    // 保存当前光标状态
    HCURSOR hOriginalCursor = GetCursor();

    // 设置光标为默认箭头
    HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
    SetCursor(hCursor);

    // 使用 CreateProcess 代替 ShellExecuteEx
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    std::string fullCommand = command + " " + parameters;

    if (!CreateProcess(NULL, const_cast<char*>(fullCommand.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::string errorMsg = "CreateProcess failed: " + std::to_string(GetLastError());
        MessageBox(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // 等待进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // 恢复原始光标状态
    SetCursor(hOriginalCursor);

    return 0;
}
