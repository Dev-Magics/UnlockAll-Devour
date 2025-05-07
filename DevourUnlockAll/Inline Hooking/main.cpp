#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <MinHook.h>
#include <iostream>
#include <string>

#if _WIN64
#pragma comment(lib, "libMinHook.x64.lib")
#else
#pragma comment(lib, "libMinHook.x86.lib")
#endif

bool(__fastcall* PlayerHasItem_o)(DWORD*, uint32_t, int32_t, DWORD*);
bool(__fastcall* UserHasItem_o)(DWORD*, int32_t, DWORD*);

bool __stdcall PlayerHasItem_h(DWORD* __this, uint32_t connectionId, int32_t steamItemDefID, DWORD* method) {
    return true;
}

bool __stdcall UserHasItem_h(DWORD* __this, int32_t steamItemDefID, DWORD* method) {
    return true;
}

bool(__fastcall* HasOutfit_o)(DWORD*, const char*, const char*, DWORD*);
bool(__fastcall* isrobeunlocked_o)(DWORD*, const char*, const char*, DWORD*);
bool(__fastcall* ischaracterunlocked_o)(DWORD*, const char*, DWORD*);
void(__fastcall* setlockedoutfit_o)(DWORD*, bool, DWORD*);
void(__fastcall* setlockedpet_o)(DWORD*, bool, DWORD*);
void(__fastcall* setlockedflashlight_o)(DWORD*, bool, int32_t, DWORD*);
void(__fastcall* setlockedemote_o)(DWORD*, bool, int32_t, DWORD*);
void(__fastcall* setlockedperks_o)(DWORD*, bool, int32_t, DWORD*);

void(__fastcall* setplayerrank_o)(DWORD*, int32_t, int32_t, int32_t, DWORD*);
void(__fastcall* setrank_o)(DWORD*, int32_t, DWORD*);

bool __stdcall HasOutfit_h(DWORD* __this, const char* robe, const char* character, DWORD* method) {
    return true;
}

bool __stdcall isrobeunlocked_h(DWORD* __this, const char* robe, const char* character, DWORD* method) {
    return true;
}

bool __stdcall ischaracterunlocked_h(DWORD* __this, const char* prefab, DWORD* method) {
    return true;
}

void __stdcall setlockedoutfit_h(DWORD* __this, bool locked, DWORD* method) {
    setlockedoutfit_o(__this, false, method);
}

void __stdcall setlockedflashlight_h(DWORD* __this, bool locked, int32_t cost, DWORD* method) {
    setlockedflashlight_o(__this, false, 0, method);
}

void __stdcall setlockedpet_h(DWORD* __this, bool locked, DWORD* method) {
    setlockedpet_o(__this, false, method);
}

void __stdcall setlockedemote_h(DWORD* __this, bool locked, int32_t cost, DWORD* method) {
    setlockedemote_o(__this, false, 0, method);
}

void __stdcall setlockedperks_h(DWORD* __this, bool locked, int32_t cost, DWORD* method) {
    setlockedperks_o(__this, false, 0, method);
}

void __stdcall setplayerrank_h(DWORD* __this, int32_t connectionid, int32_t rank, int32_t exp, DWORD* method) {
    setplayerrank_o(__this, connectionid, 666, 666, method);
}

void __stdcall setrank_h(DWORD* __this, int32_t rank, DWORD* method) {
    setrank_o(__this, 666, method);
}

DWORD WINAPI MainThread(LPVOID)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "[*] DLL Injected !" << std::endl;

    uintptr_t gameAssembly = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");
    if (!gameAssembly) {
        std::cout << "[-] GameAssembly.dll not found." << std::endl;
        return 1;
    }

    if (MH_Initialize() != MH_OK) {
        std::cout << "[-] MH_Initialize failed." << std::endl;
        return 1;
    }

    struct HookInfo {
        uintptr_t offset;
        LPVOID hookFunc;
        LPVOID* original;
        const char* name;
    };

    HookInfo hooks[] = {
        { 0x6AFD40, (LPVOID)HasOutfit_h, (LPVOID*)&HasOutfit_o, "HasOutfit" },
        { 0x6AE740, (LPVOID)isrobeunlocked_h, (LPVOID*)&isrobeunlocked_o, "isrobeunlocked" },
        { 0x6ACDF0, (LPVOID)ischaracterunlocked_h, (LPVOID*)&ischaracterunlocked_o, "ischaracterunlocked" },
        { 0x63AB50, (LPVOID)setlockedoutfit_h, (LPVOID*)&setlockedoutfit_o, "setlockedoutfit" },
        { 0x639E90, (LPVOID)setlockedflashlight_h, (LPVOID*)&setlockedflashlight_o, "setlockedflashlight" },
        { 0x63B050, (LPVOID)setlockedpet_h, (LPVOID*)&setlockedpet_o, "setlockedpet" },
        { 0x639C10, (LPVOID)setlockedemote_h, (LPVOID*)&setlockedemote_o, "setlockedemote" },
        { 0x63AD60, (LPVOID)setlockedperks_h, (LPVOID*)&setlockedperks_o, "setlockedflashlight" },
        { 0x5F2990, (LPVOID)setplayerrank_h, (LPVOID*)&setplayerrank_o, "setplayerrank" },
        { 0x570560, (LPVOID)setrank_h, (LPVOID*)&setrank_o, "setrank" },
        { 0x5BAF20, (LPVOID)PlayerHasItem_h, (LPVOID*)&PlayerHasItem_o, "PlayerHasItem" },
        { 0x5B87A0, (LPVOID)UserHasItem_h, (LPVOID*)&UserHasItem_o, "UserHasItem" }
    };

    bool status = false;
    for (int attempts = 0; attempts < 10 && !status; attempts++) {
        status = true;

        for (const auto& hook : hooks) {
            LPVOID target = (LPVOID)(gameAssembly + hook.offset);

            MH_STATUS createStatus = MH_CreateHook(target, hook.hookFunc, hook.original);
            if (createStatus == MH_OK || createStatus == MH_ERROR_ALREADY_CREATED) {
                if (MH_EnableHook(target) == MH_OK) {
                    std::cout << "[+] Hook " << hook.name << " installed!" << std::endl;
                }
                else {
                    std::cout << "[-] Failed to enable hook " << hook.name << "." << std::endl;
                    status = false;
                }
            }
            else {
                std::cout << "[-] Failed to create hook " << hook.name << ". (Code: " << createStatus << ")" << std::endl;
                status = false;
            }
        }
        if (!status) {
            std::cout << "[~] Failed, trying again (" << attempts + 1 << "/10)...\n";
            Sleep(2500);
        }
    }

    if (status) {
        std::cout << "[+] Unlock all successfully activated." << std::endl;
    }
    else {
        std::cout << "[-] Failed to activate all hooks after 10 attempts." << std::endl;
    }
    
    return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hInst);
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}
