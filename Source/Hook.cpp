#include "stdafx.h"
#include "Hook.h"

// Original functions
WNDPROC WndProcOriginal = nullptr;
ENDSCENE9 EndSceneOriginal = nullptr;

BYTE original_code[1 + sizeof(PTR)];			// original EndScene code
BYTE jmp_code[1 + sizeof(PTR)];					// jump to EndScene9
bool isDrawButtons = DRAW_BUTTONS_AFTER_HOOK;

TouchEngine *engine = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT code, WPARAM wParam, LPARAM lParam)
{
	switch (code)
	{
	case WM_KEYUP:
		if (wParam == DRAW_BUTTONS_KEY)
			isDrawButtons = !isDrawButtons;
		break;
	case WM_POINTERDOWN:
	case WM_POINTERUP:
	case WM_POINTERUPDATE:
	case WM_GESTURE:
		engine->NewTouchMessage(hWnd, code, wParam, lParam);
		return 0;
	}
	return CallWindowProc(WndProcOriginal, hWnd, code, wParam, lParam);
}

HRESULT WINAPI EndScene9(IDirect3DDevice9* pDevice)
{
	static bool isFirst = true;
	if (isFirst)
	{
		engine->InitGraphicAndButtons(pDevice);
		isFirst = false;
	}

	if (isDrawButtons)
		engine->DrawButtons(pDevice);

	memcpy_s(EndSceneOriginal, 1+sizeof(PTR), original_code, 1+sizeof(PTR));
	HRESULT res = EndSceneOriginal(pDevice);
	memcpy_s(EndSceneOriginal, 1+sizeof(PTR), jmp_code, 1+sizeof(PTR));
	return res;
}

bool HookDX9()
{
	HWND hWnd = CreateWindowExA(0, "STATIC","DXHook", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	HMODULE lib = GetModuleHandleA("d3d9.dll");
	if (lib == nullptr)
	{
		DestroyWindow(hWnd);
		return false;
	}

	// Create Direct3D device
	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;
	IDirect3DDevice9 *d3dDevice;
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE::D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);

	// Search for IDirect3DDevice9::EndScene address in virtual table
	PTR *vtable = *((PTR**)d3dDevice);
	EndSceneOriginal = (ENDSCENE9)vtable[DX9_ENDSCENE];

	d3dDevice->Release();
	d3d9->Release();
	DestroyWindow(hWnd);

	// EndSceneOriginal:
	// jmp EndScene9;
	jmp_code[0] = 0xE9;
	PTR addr = (PTR)EndScene9 - (PTR)EndSceneOriginal - sizeof(jmp_code);
	*((PTR*)(jmp_code + 1)) = addr;

	DWORD temp;
	VirtualProtect(EndSceneOriginal, 1+sizeof(PTR), PAGE_EXECUTE_READWRITE, &temp);
	memcpy_s(original_code, 1+sizeof(PTR), EndSceneOriginal, 1+sizeof(PTR));

	memcpy_s(EndSceneOriginal, 1+sizeof(PTR), jmp_code, 1+sizeof(PTR));
	return true;
}

void UnhookDX9()
{
	if (EndSceneOriginal)
		memcpy_s(EndSceneOriginal, 1+sizeof(PTR), original_code, 1+sizeof(PTR));
	EndSceneOriginal = nullptr;
}

bool HookWindowProc(HWND hWnd)
{
	WndProcOriginal = (WNDPROC) SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG) WndProc);
	return WndProcOriginal != nullptr;
}

bool UnhookWindowProc(HWND hWnd)
{
	return SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG) WndProcOriginal);
}

void _cdecl InitialThread(void *arg)
{
	HWND hWnd = getGameWindow();
	if (hWnd == 0)
	{
		MessageBox(0, TEXT("The game window wasn't found."), 0, MB_ICONERROR);
		FreeLibraryAndExitThread((HMODULE)arg, 0);
	}

	engine = new TouchEngine();

	if (!HookDX9())
	{
		MessageBox(0, TEXT("d3d9.dll not found.\nProbably the game doesn't use DirectX 9."), 0, MB_ICONERROR);
		FreeLibraryAndExitThread((HMODULE)arg, 0);
	}

	if (!HookWindowProc(hWnd))
	{
		UnhookDX9();
		delete engine;
		MessageBox(0, TEXT("The replacement of the main window function is failed."), 0, MB_ICONERROR);
		FreeLibraryAndExitThread((HMODULE)arg, 0);
	}
}