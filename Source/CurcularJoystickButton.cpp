#include "stdafx.h"
#include "CircularJoystickButton.h"

void CircularJoystickButton::Action(UINT code, WPARAM wParam, LPARAM lParam)
{
	if (code == WM_POINTERDOWN)
		isPressed = true;
	else if (code == WM_POINTERUP)
		isPressed = false;

	int newX = GET_X_LPARAM(lParam),
		newY = GET_Y_LPARAM(lParam);

	int r = (pos.right-pos.left)/2;
	int posX = newX-pos.left-r;
	int posY = newY-pos.top-r;
	double angle = atan((double(-posY))/posX);
	if (posX < 0)
		angle += PI;
	if (posY > 0 && posX > 0)
		angle += 2.0*PI;
	
	if ((posX*posX + posY*posY) > (r*r))
	{
		newX = (int)(cos(angle)*r);
		newY = (int)(-sin(angle)*r);
	} else {
		newX -= pos.left+r;
		newY -= pos.top+r;
	}

	if (code != WM_POINTERUPDATE || newX != prevX || newY != prevY)
	{
		if (textureStick)
			MoveStick(newX + pos.left + r, newY + pos.top + r);
		
		int persent = (int)(sqrt(newX*newX + newY*newY) / r * 100);

		if (!func)
		{
			int sector = (int)(((angle / (2.0*PI)) * 80.0 + 5) / 10);
			if (sector == 8)
				sector = 0;
			bool press = (persent >= deadzone);
			bool currentSector[4];
			currentSector[0] = (sector >= 1 && sector <= 3);
			currentSector[3] = (sector >= 3 && sector <= 5);
			currentSector[2] = (sector >= 5 && sector <= 7);
			currentSector[1] = (sector == 7 || sector <= 1);

			for (int i = 0; i < 4; i++)
			{
				if (pressed_keys[i] && (!press || !(currentSector[i])))
				{
					UnpressKey(vk_codes[i]);
					pressed_keys[i] = false;
				} else if (!pressed_keys[i] && press && currentSector[i]) {
					PressKey(vk_codes[i]);
					pressed_keys[i] = true;
				}
			}
		} else {
			POINTER_STATE state;
			switch(code)
			{
			case WM_POINTERDOWN:
				state = POINTER_STATE::POINTER_DOWN;
				break;
			case WM_POINTERUPDATE:
				state = POINTER_STATE::POINTER_MOVE;
				break;
			default:
				state = POINTER_STATE::POINTER_UP;
				break;
			}

			if (currentThread)
				((void(_cdecl*)(int,double,POINTER_STATE))func)(persent, angle, state);
			else
			{
				ThreadArg *arg = new ThreadArg;
				arg->persent = persent;
				arg->angle = angle;
				arg->state = state;
				arg->stickfunc = (void(_cdecl*)(int,double,POINTER_STATE))func;
				_beginthread(StartThread, 0, arg);
			}
		}
	}

	if (isPressed)
	{
		prevX = newX;
		prevY = newY;
	} else
		ResetStick();
}


void CircularJoystickButton::StartThread(void *args)
{
	ThreadArg *arg = (ThreadArg*) args;
	arg->stickfunc(arg->persent, arg->angle, arg->state);
	delete arg;
}