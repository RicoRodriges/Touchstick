#include "stdafx.h"
#include "UserFunctions.h"

IButton *buttons[12];
void SelectDelay(POINTER_STATE state);
void PauseMenu(POINTER_STATE state);
void OpenWeaponMenu(POINTER_STATE state);
void CloseWeaponMenu(POINTER_STATE state);
void FireFunc(POINTER_STATE state);

HWND getGameWindow()
{
	HWND hWnd = FindWindow(TEXT("ArmageddonWindowed"), nullptr);
	hWnd = FindWindowEx(hWnd, 0, TEXT("Worms2D"), nullptr);
	return hWnd;
}

void InitButtons(std::list<IButton*> &listButtons, int windowWidth, int windowHeight, IDirect3DDevice9 *pDevice)
{
	PushButton *push;

	// Select weapon delay
	// Press 1, 2, 3, 4 or 5
	push = new PushButton(1, 360, windowHeight-35-150, 130, 150, SelectDelay);
	push->setTexture(pDevice, TEXT("touchstick\\delay_up.png"), TEXT("touchstick\\delay_down.png"));
	listButtons.push_front(push);
	buttons[0] = push;

	// Open weapons selection menu
	// Press right mouse button
	push = new PushButton(2, windowWidth-205-130, windowHeight-55-150, 130, 150, OpenWeaponMenu);
	push->setTexture(pDevice, TEXT("touchstick\\weapon_up.png"), TEXT("touchstick\\weapon_down.png"));
	listButtons.push_front(push);
	buttons[1] = push;

	// Fire
	// Press Space and left mouse click
	push = new PushButton(3, windowWidth-205-130, windowHeight-230-150, 130, 150, FireFunc);
	push->setTexture(pDevice, TEXT("touchstick\\fire_up.png"), TEXT("touchstick\\fire_down.png"));
	push->currentThread = false;
	listButtons.push_front(push);
	buttons[2] = push;

	// Jump
	// Press Enter
	push = new PushButton(4, windowWidth-30-130, windowHeight-230-150, 130, 150, VK_RETURN);
	push->setTexture(pDevice, TEXT("touchstick\\jumpup_up.png"), TEXT("touchstick\\jumpup_down.png"));
	listButtons.push_front(push);
	buttons[3] = push;

	// Jump
	// Press backspace
	push = new PushButton(5, windowWidth-25-130, windowHeight-55-150, 140, 150, VK_BACK);
	push->setTexture(pDevice, TEXT("touchstick\\jumpdown_up.png"), TEXT("touchstick\\jumpdown_down.png"));
	listButtons.push_front(push);
	buttons[4] = push;

	MoveButton *move;

	// Mouse controller
	move = new MoveButton(6, 0, 0, windowWidth, windowHeight-400);
	listButtons.push_front(move);
	buttons[5] = move;
	move = new MoveButton(7, 440, windowHeight-400, windowWidth-440-330, 400);
	listButtons.push_front(move);
	buttons[6] = move;

	CircularJoystickButton *joystick;
	joystick = new CircularJoystickButton(8, 40, windowHeight-25-285, 142, 142, 35/*%*/, VK_UP, VK_RIGHT, VK_DOWN, VK_LEFT);
	joystick->setBoxTexture(pDevice, TEXT("touchstick\\joystick_static.png"));
	joystick->setStickTexture(pDevice, TEXT("touchstick\\joystick_dynamic.png"));
	listButtons.push_front(joystick);
	buttons[7] = joystick;

	// Pause
	// Press ESC
	push = new PushButton(9, windowWidth-20-88, 20, 88, 97, PauseMenu);
	push->setTexture(pDevice, TEXT("touchstick\\pause.png"));
	listButtons.push_front(push);
	buttons[8] = push;
	
	// Mouse for weapon menu
	move = new MoveButton(10, 0, 0, windowWidth, windowHeight);
	move->enabled = false;
	listButtons.push_front(move);
	buttons[9] = move;

	// Close weapons selection menu
	// Press right mouse button
	push = new PushButton(11, 0, 0, windowWidth, windowHeight, CloseWeaponMenu);
	push->enabled = false;
	listButtons.push_front(push);
	buttons[10] = push;

	// Left mouse click for pause menu
	push = new PushButton(12, 0, 0, windowWidth, windowHeight, VK_LBUTTON);
	push->enabled = false;
	listButtons.push_back(push);
	buttons[11] = push;

}

void SelectDelay(POINTER_STATE state)
{
	static int delay = 3;
	if (state == POINTER_STATE::POINTER_DOWN)
	{
		delay++;
		if (delay == 6)
			delay = 1;
		PressKey('0' + delay); // Press 1, 2, 3, 4 or 5
	} else
		UnpressKey('0' + delay);
}

void PauseMenu(POINTER_STATE state)
{
	static bool isPause = false;
	if (state == POINTER_STATE::POINTER_DOWN)
	{
		PressKey(VK_ESCAPE);
	} else {
		for (int i = 0; i < 12; i++)
		{
			if (i == 9 || i == 11)
			{
				buttons[i]->enabled = !isPause;
			} else if (i != 8) {
				buttons[i]->enabled = isPause;
				buttons[i]->setDrawable(isPause);
			}				
		}
		isPause = !isPause;
		UnpressKey(VK_ESCAPE);
	}
}

void OpenWeaponMenu(POINTER_STATE state)
{
	if (state == POINTER_STATE::POINTER_DOWN)
	{
		PressKey(VK_RBUTTON);
	} else {
		for (int i = 0; i < 12; i++)
		{
			if (i == 9 || i == 10)
			{
				buttons[i]->enabled = true;
			} else {
				buttons[i]->enabled = false;
				buttons[i]->setDrawable(false);
			}
		}
		UnpressKey(VK_RBUTTON);
	}
}

void CloseWeaponMenu(POINTER_STATE state)
{
	if (state == POINTER_STATE::POINTER_DOWN)
	{
		PressKey(VK_LBUTTON);
	} else {
		for (int i = 0; i < 12; i++)
		{
			if (i < 9)
			{
				buttons[i]->enabled = true;
				buttons[i]->setDrawable(true);
			} else
				buttons[i]->enabled = false;
		}
		UnpressKey(VK_LBUTTON);
	}
}

void FireFunc(POINTER_STATE state)
{
	if (state == POINTER_STATE::POINTER_DOWN)
	{
		PressKey(VK_SPACE);
	} else {
		UnpressKey(VK_SPACE);
		PressKey(VK_LBUTTON);
		Sleep(100);
		UnpressKey(VK_LBUTTON);
	}
}