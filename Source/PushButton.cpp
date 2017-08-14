#include "stdafx.h"
#include "PushButton.h"

PushButton::PushButton(int id, int x, int y, int width, int height, char vk_button)
	: ISingleFingerButton(id,x,y,width,height)
{
	type = BUTTON_TYPE::PUSH;
	vk_code = vk_button;
	func = nullptr;
}

PushButton::PushButton(int id, int x, int y, int width, int height, void(_cdecl *pushfunc)(POINTER_STATE))
	: ISingleFingerButton(id,x,y,width,height)
{
	type = BUTTON_TYPE::PUSH;
	vk_code = 0;
	func = pushfunc;
}

void PushButton::Action(UINT code, WPARAM wParam, LPARAM lParam)
{
	if (code != WM_POINTERUPDATE)
	{
		isPressed = code == WM_POINTERDOWN;
		if (func == nullptr)
		{
			if (isPressed)
				PressKey(vk_code);
			else
				UnpressKey(vk_code);
		} else {
			POINTER_STATE state = isPressed ? POINTER_STATE::POINTER_DOWN : POINTER_STATE::POINTER_UP;
			if (currentThread)
				 ((void(_cdecl *)(POINTER_STATE)) func)(state);
			else
				_beginthread((void(_cdecl*)(void*))func, 0, (void*)state);
		}
	}
}