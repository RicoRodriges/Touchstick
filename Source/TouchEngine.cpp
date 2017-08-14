#include "stdafx.h"
#include "TouchEngine.h"

TouchEngine::TouchEngine()
{
	ZeroMemory(&pointers, sizeof(pointers));
	sprite = nullptr;
	InitializeCriticalSection(&cs);
}

TouchEngine::~TouchEngine()
{
	if (sprite)
		sprite->Release();
	for (IButton *b : buttonList)
		delete b;
	DeleteCriticalSection(&cs);
}

void TouchEngine::NewTouchMessage(HWND hWnd, UINT code, WPARAM wParam, LPARAM lParam)
{
	EnterCriticalSection(&cs);
	switch (code)
	{
	case WM_POINTERDOWN:
		{
			INT32 p = GET_POINTERID_WPARAM(wParam);
			POINT point; point.x = GET_X_LPARAM(lParam); point.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &point);
			IButton *push = nullptr;
			int pushPriority = MININT32;
			IButton *move = nullptr;
			int movePriority = MININT32;
			for (IButton *b : buttonList)
			{
				if (b->enabled && !b->IsPressed() && b->IsHit(point))
				{
					if (b->type == BUTTON_TYPE::PUSH && pushPriority < b->priority) {
						push = b;
						pushPriority = b->priority;
					}
					else if (b->type == BUTTON_TYPE::MOVE && movePriority < b->priority) {
						move = b;
						movePriority = b->priority;
					}
				}
			}
			if (push || move)
			{
				if (push && move == nullptr)
				{
					push->AddPointer(p);
					push->Action(code, wParam, lParam);
				} else if (push == nullptr && move) {
					move->AddPointer(p);
					move->Action(code, wParam, lParam);
				} else {
					for (Pointer &pointer : pointers)
					{
						if (pointer.id == 0)
						{
							pointer.id = p;
							pointer.x = point.x;
							pointer.y = point.y;
							pointer.tick = GetTickCount64();
							pointer.push = (PushButton*)push;
							pointer.move = (MoveButton*)move;
							break;
						}
					}
				}
			}
		}
		break;
	case WM_POINTERUP:
		{
			INT32 p = GET_POINTERID_WPARAM(wParam);
			int i = 0;
			for (; i < MAX_POINTER; i++)
			{
				if (pointers[i].id == p)
				{
					pointers[i].id = 0;
					break;
				}
			}
			if (i == MAX_POINTER)
			{
				for (IButton *b : buttonList)
				{
					if (b->FoundPointer(p))
					{
						b->RemovePointer(GET_POINTERID_WPARAM(wParam));
						b->Action(code, wParam, lParam);
						break;
					}
				}
			}
		}
		break;
	case WM_POINTERUPDATE:
		{
			INT32 p = GET_POINTERID_WPARAM(wParam);
			int i = 0;
			for (; i < MAX_POINTER; i++)
			{
				if (pointers[i].id == p)
				{
					int dX = GET_X_LPARAM(lParam) - pointers[i].x,
						dY = GET_Y_LPARAM(lParam) - pointers[i].y;
					IButton *b = nullptr;
					if (dX*dX + dY*dY >= DELTA_DISTANCE*DELTA_DISTANCE)
						b = pointers[i].move;
					else if (GetTickCount64() - pointers[i].tick >= BUTTON_DELAY)
						b = pointers[i].push;
					if (b) {
						b->AddPointer(p);
						b->Action(WM_POINTERDOWN, wParam, lParam);
						pointers[i].id = 0;
					}
					break;
				}
			}
			if (i == MAX_POINTER)
			{
				for (IButton *b : buttonList)
				{
					if (b->FoundPointer(p))
					{
						b->Action(code, wParam, lParam);
						break;
					}
				}
			}
		}
		break;
	case WM_GESTURE: // Disable event
		CloseGestureInfoHandle((HGESTUREINFO)lParam);
		break;
	}
	LeaveCriticalSection(&cs);
}

void TouchEngine::InitGraphicAndButtons(IDirect3DDevice9 *pDevice)
{
	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	EnterCriticalSection(&cs);
	InitButtons(buttonList, viewport.Width, viewport.Height, pDevice);
	LeaveCriticalSection(&cs);

	D3DXCreateSprite(pDevice, &sprite);
}

void TouchEngine::DrawButtons(IDirect3DDevice9 *pDevice)
{
	EnterCriticalSection(&cs);

	for (IButton *b : buttonList)
		b->Draw(pDevice, sprite);

	LeaveCriticalSection(&cs);
}