
#include <Windows.h>
#include <iostream>
#include "memory.h"
#include "interfaces.h"
#include "sdk/schema.h"
#pragma warning (disable : 4996)


HMODULE inj_mod;


DWORD WINAPI MainThread(LPVOID lpParam) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	HWND targetwindow = FindWindow(NULL, L"Deadlock");

	if (!targetwindow) {
		std::cout << "deadlock window not found";
		goto exit;
	}

	GetWindowThreadProcessId(targetwindow, &memory::processID);

	if (!memory::processID) {
		std::cout << "proccess id is null";
		goto exit;
	}

	if (!interfaces::core::Setup()) {
		std::cout << "error creating interfaces";
		goto exit;
	}
	
	if (!schema::Dump("client.dll"))
		std::cout << "error while dumping client.dll";
	

exit:
	FreeConsole();
	FreeLibraryAndExitThread(inj_mod, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		inj_mod = hModule;
		DisableThreadLibraryCalls(hModule);
		//CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)UI::Render, nullptr, NULL, nullptr));
		CloseHandle(CreateThread(0, NULL, MainThread, (LPVOID)L"Dumper", NULL, NULL));

	}
	return TRUE;
}