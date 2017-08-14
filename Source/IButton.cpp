#include "stdafx.h"
#include "IButton.h"

bool IButton::IsHit(POINT &p) {
	return (p.x >= pos.left && p.x <= pos.right &&
		p.y >= pos.top && p.y <= pos.bottom) ? true : false;
}

IButton::IButton(int id, int x, int y, int width, int height)
	: id(this->id) {
	pos.left = x;
	pos.right = x + width;
	pos.top = y;
	pos.bottom = y + height;
	type = BUTTON_TYPE::UNKNOWN;
	isDrawable = false;
	enabled = true;
	currentThread = true;
	priority = 0;
}

int IButton::getID() { return id; }

void PressKey(char vk_code)
{
	INPUT inp;
	ZeroMemory(&inp, sizeof(INPUT));
	if (vk_code == VK_LBUTTON || vk_code == VK_RBUTTON)
	{
		inp.type = INPUT_MOUSE;
		if (vk_code == VK_LBUTTON)
			inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		else
			inp.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	} else {
		inp.type = INPUT_KEYBOARD;
		inp.ki.wScan = MapVirtualKey(vk_code, MAPVK_VK_TO_VSC);
		inp.ki.wVk = vk_code;
		inp.ki.dwFlags = KEYEVENTF_SCANCODE;
	}
	SendInput(1, &inp, sizeof(INPUT));
}

void UnpressKey(char vk_code)
{
	INPUT inp;
	ZeroMemory(&inp, sizeof(INPUT));
	if (vk_code == VK_LBUTTON || vk_code == VK_RBUTTON)
	{
		inp.type = INPUT_MOUSE;
		if (vk_code == VK_LBUTTON)
			inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		else
			inp.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	} else {
		inp.type = INPUT_KEYBOARD;
		inp.ki.wScan = MapVirtualKey(vk_code, MAPVK_VK_TO_VSC);
		inp.ki.wVk = vk_code;
		inp.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	}
	SendInput(1, &inp, sizeof(INPUT));
}