#include "stdafx.h"
#include "UserFunctions.h"

IButton *buttons[3];

HWND getGameWindow()
{
	HWND hWnd = FindWindow(...);
	return hWnd;
}

void InitButtons(std::list<IButton*> &listButtons, int windowWidth, int windowHeight, IDirect3DDevice9 *pDevice)
{
	PushButton *push = new PushButton(1, 0, 0, 100, 100, '1'); // press 1
	listButtons.push_front(push);
	buttons[0] = push;

	MoveButton *move = new MoveButton(2, 0, 0, windowWidth, windowHeight);
	listButtons.push_front(move);
	buttons[1] = move;
	
	CircularJoystickButton *joystick = new CircularJoystickButton(3, 0, 0, 200, 200, 33/*%*/, VK_UP, VK_RIGHT, VK_DOWN, VK_LEFT);
	listButtons.push_front(joystick);
	buttons[2] = joystick;
}