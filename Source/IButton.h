#pragma once
#include "stdafx.h"

void PressKey(char vk_code);

void UnpressKey(char vk_code);

enum POINTER_STATE
{
	POINTER_DOWN,
	POINTER_MOVE,
	POINTER_UP
};

enum BUTTON_TYPE {
	UNKNOWN,
	PUSH,
	MOVE
};

class IButton {
	friend class TouchEngine;
protected:
	const int id;
	RECT pos;
	BUTTON_TYPE type;
	void *func;
	bool isDrawable;

	virtual void Draw(IDirect3DDevice9* pDevice, ID3DXSprite *sprite) = 0;
	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) = 0;
	virtual void AddPointer(INT32) = 0;
	virtual bool FoundPointer(INT32) = 0;
	virtual void RemovePointer(INT32) = 0;
	virtual bool IsPressed() = 0;

	bool IsHit(POINT &p);
	IButton(int id, int x, int y, int width, int height);
public:
	bool enabled;
	bool currentThread;
	INT32 priority;

	virtual void setDrawable(bool isDrawable) = 0;
	virtual ~IButton() {};

	int getID();
};