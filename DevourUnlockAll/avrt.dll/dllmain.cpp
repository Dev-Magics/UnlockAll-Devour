#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    static bool alreadyExecuted = false;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        if (alreadyExecuted) return TRUE;
        alreadyExecuted = true;

        LoadLibraryA("InlineHooking.dll");
    }

    return TRUE;
}
