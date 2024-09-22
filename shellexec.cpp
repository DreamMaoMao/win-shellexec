#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <string>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [arguments]" << std::endl;
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

    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = "open"; // 使用 "open" 动词以避免终端问题
    sei.lpFile = command.c_str(); // 要执行的命令
    sei.lpParameters = parameters.c_str(); // 命令参数
    sei.lpDirectory = NULL; // 使用当前目录
    sei.nShow = SW_HIDE; // 隐藏窗口
    sei.hInstApp = NULL;

    if (!ShellExecuteEx(&sei)) {
        std::cerr << "ShellExecuteEx failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 等待进程结束
    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);

    // 恢复原始光标状态
    SetCursor(hOriginalCursor);

    return 0;
}
