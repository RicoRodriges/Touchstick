#include "stdafx.h"
#include "MoveButton.h"

MoveButton::MoveButton(int id, int x, int y, int width, int height,
					   void(_cdecl *movefunc)(int, int, int, int, POINTER_STATE))
					   : ISingleFingerButton(id,x,y,width,height)
{
	type = BUTTON_TYPE::MOVE;
	prevX = -1;
	prevY = -1;
	func = movefunc;
}

void MoveButton::Action(UINT code, WPARAM wParam, LPARAM lParam)
{
	if (code == WM_POINTERDOWN)
		isPressed = true;
	else if (code == WM_POINTERUP)
		isPressed = false;

	int newX = GET_X_LPARAM(lParam),
		newY = GET_Y_LPARAM(lParam);

	if (func == nullptr)
	{
		if (code == WM_POINTERUPDATE && (newX != prevX || newY != prevY))
		{
			INPUT inp;
			ZeroMemory(&inp, sizeof(INPUT));
			inp.type = INPUT_MOUSE;
			inp.mi.dwFlags = MOUSEEVENTF_MOVE;
			inp.mi.dx = newX - prevX;
			inp.mi.dy = newY - prevY;
			SendInput(1, &inp, sizeof(INPUT));
		}
	} else {
		POINTER_STATE state;
		switch (code) {
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
			((void(_cdecl *)(int, int, int, int, POINTER_STATE)) func)(prevX, prevY, newX, newY, state);
		else {
			ThreadArg *arg = new ThreadArg;
			arg->p[0].x = prevX; arg->p[0].y = prevY;
			arg->p[1].x = newX; arg->p[1].y = newY;
			arg->state = state;
			arg->movefunc = (void(_cdecl *)(int, int, int, int, POINTER_STATE)) func;
			_beginthread(StartThread, 0, arg);
		}
	}

	prevX = newX;
	prevY = newY;
}

void __cdecl MoveButton::StartThread(void *args)
{
	ThreadArg *arg = (ThreadArg*)args;
	arg->movefunc(arg->p[0].x, arg->p[0].y, arg->p[1].x, arg->p[1].y, arg->state);
	delete arg;
}