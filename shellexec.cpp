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

    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = "open"; // Use "open" verb to avoid terminal issues
    sei.lpFile = command.c_str(); // Command to execute
    sei.lpParameters = parameters.c_str(); // Command parameters
    sei.lpDirectory = NULL; // Use current directory
    sei.nShow = SW_HIDE; // Hide the window
    sei.hInstApp = NULL;

    if (!ShellExecuteEx(&sei)) {
        std::cerr << "ShellExecuteEx failed: " << GetLastError() << std::endl;
        return 1;
    }

    // Wait for the process to finish
    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);

    return 0;
}
