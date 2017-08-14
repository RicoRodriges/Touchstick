#include "stdafx.h"
#include "Hook.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return _beginthread(InitialThread, 0, hModule) != -1;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

