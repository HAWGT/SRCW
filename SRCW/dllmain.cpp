#include "Features.h"

#include "CppSDK/SDK/Engine_classes.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

inline HMODULE CurrentModule = nullptr;

void Init()
{
	std::ifstream config(ConfigFileName);

	auto CreateConsole = [](const char* name) {
		FILE* ConsoleIO;
		if (!AllocConsole())
			return;

		freopen_s(&ConsoleIO, "CONIN$", "r", stdin);
		freopen_s(&ConsoleIO, "CONOUT$", "w", stderr);
		freopen_s(&ConsoleIO, "CONOUT$", "w", stdout);

		SetConsoleTitleA(name);
		};

	if (config.is_open())
	{
		std::string param;
		unsigned int value = 0;

		while (config >> param >> value)
		{
			if (!param.compare("Console"))
			{
				if (value == 1) CreateConsole("SRCW");
				std::cout << "[*] Sonic Racing Crossworlds" << "\n";
				std::cout << "[+] Settings Loaded: " << "\n";
				std::cout << param << " " << value << "\n";
			}
		}

		config.close();
	}
	else
	{
		std::ofstream ConfigFile;

		ConfigFile.open(ConfigFileName, std::ofstream::out | std::ofstream::trunc);

		ConfigFile << "Console 0" << std::endl;

		ConfigFile.close();
	}

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HookGame, CurrentModule, 0, nullptr);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		CurrentModule = hModule;
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Init, CurrentModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

