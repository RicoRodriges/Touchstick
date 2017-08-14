#pragma once
#include "stdafx.h"
#include "TouchEngine.h"

#define DX9_ENDSCENE 42

#ifndef _WIN64
typedef INT32 PTR;
#else
typedef INT64 PTR;
#endif

typedef HRESULT (WINAPI *ENDSCENE9)(IDirect3DDevice9*);

LRESULT WINAPI WndProc(HWND hWnd, UINT code, WPARAM wParam, LPARAM lParam);
HRESULT WINAPI EndScene9(IDirect3DDevice9* pDevice);

bool HookDX9();
// Unsafe function
void UnhookDX9();

bool HookWindowProc(HWND hWnd);
bool UnhookWindowProc(HWND hWnd);

void _cdecl InitialThread(void *arg);