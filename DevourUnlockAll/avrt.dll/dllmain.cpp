// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    static bool alreadyExecuted = false;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        if (alreadyExecuted) return TRUE;
        alreadyExecuted = true;

        LoadLibraryA("main.dll");
    }

    return TRUE;
}
