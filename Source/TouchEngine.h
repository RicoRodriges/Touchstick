#pragma once
#include "stdafx.h"
#include "IButton.h"
#include "PushButton.h"
#include "MoveButton.h"
#include "UserFunctions.h"

// The maximum number of new touch points at a time
#define MAX_POINTER 5

struct Pointer {
	INT32 id;
	LONG x,y;
	ULONGLONG tick;
	PushButton *push;
	MoveButton *move;
};

class TouchEngine
{
	// It is used when the touch point hits several buttons
	Pointer pointers[MAX_POINTER];
	ID3DXSprite *sprite;
	CRITICAL_SECTION cs;
public:
	std::list<IButton*> buttonList;

	TouchEngine();
	~TouchEngine();
	void NewTouchMessage(HWND hWnd, UINT code, WPARAM wParam, LPARAM lParam);
	void InitGraphicAndButtons(IDirect3DDevice9 *pDevice);
	void DrawButtons(IDirect3DDevice9 *pDevice);
};