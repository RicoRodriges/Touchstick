#include "stdafx.h"
#include "JoystickButton.h"

JoystickButton::JoystickButton(int id, int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight, UINT8 deadzone,
							   char keyUp, char keyRight, char keyDown, char keyLeft)
							   : MoveButton(id,x,y,boxWidth,boxHeight)
{
	InitStick(x, y, boxWidth, boxHeight, stickWidth, stickHeight);
	func = nullptr;
	this->deadzone = deadzone;
	ZeroMemory(&pressed_keys, sizeof(pressed_keys));
	vk_codes[0] = keyUp;
	vk_codes[1] = keyRight;
	vk_codes[2] = keyDown;
	vk_codes[3] = keyLeft;
	priority = 10;
}

JoystickButton::JoystickButton(int id, int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight, void(_cdecl *stickfunc)(int,int,POINTER_STATE))
	: MoveButton(id,x,y,boxWidth,boxHeight)
{
	InitStick(x, y, boxWidth, boxHeight, stickWidth, stickHeight);
	func = stickfunc;
	deadzone = 0;
	ZeroMemory(&pressed_keys, sizeof(pressed_keys));
	ZeroMemory(&vk_codes, sizeof(vk_codes));
	priority = 10;
}

JoystickButton::~JoystickButton() {
	if (textureStick)
		textureStick->Release();
	if (clickedStick)
		clickedStick->Release();
}

void JoystickButton::setBoxTexture(IDirect3DDevice9 *pDevice, TCHAR *textureBoxPath, TCHAR *textureClickedBoxPath)
{
	if (mainTexture)
		mainTexture->Release();
	if (clickedTexture)
		clickedTexture->Release();

	if (textureBoxPath) {
		D3DXCreateTextureFromFileEx(pDevice, textureBoxPath, pos.right - pos.left, pos.bottom - pos.top,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
			&mainTexture);
		if (clickedTexture)
			D3DXCreateTextureFromFileEx(pDevice, textureClickedBoxPath, pos.right - pos.left, pos.bottom - pos.top,
				1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
				&clickedTexture);
		isDrawable = true;
	}
}

void JoystickButton::setStickTexture(IDirect3DDevice9 *pDevice, TCHAR *stickTexturePath, TCHAR *textureClickedStickPath)
{
	if (textureStick)
		textureStick->Release();
	if (clickedStick)
		clickedStick->Release();

	if (stickTexturePath) {
		D3DXCreateTextureFromFileEx(pDevice, stickTexturePath, stick.right - stick.left, stick.bottom - stick.top,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
			&textureStick);
		if (textureClickedStickPath)
			D3DXCreateTextureFromFileEx(pDevice, textureClickedStickPath, stick.right - stick.left, stick.bottom - stick.top,
				1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
				&clickedStick);

		isDrawable = true;
	}
}

void JoystickButton::InitStick(int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight) {
	stick.left = boxWidth/2 - stickWidth/2 + x;
	stick.right = boxWidth/2 + stickWidth/2 + x;
	stick.top = boxHeight/2 - stickHeight/2 + y;
	stick.bottom = boxHeight/2 + stickHeight/2 + y;
	prevX = (stick.right - stick.left)/2 + x;
	prevY = (stick.bottom - stick.top)/2 + y;
	textureStick = clickedStick = nullptr;
}

void JoystickButton::ResetStick()
{
	int boxWidth = pos.right - pos.left,
		boxHeight = pos.bottom - pos.top,
		stickWidth = stick.right - stick.left,
		stickHeight = stick.bottom - stick.top;
	stick.left = boxWidth/2 - stickWidth/2 + pos.left;
	stick.right = boxWidth/2 + stickWidth/2 + pos.left;
	stick.top = boxHeight/2 - stickHeight/2 + pos.top;
	stick.bottom = boxHeight/2 + stickHeight/2 + pos.top;
	prevX = (stick.right - stick.left)/2 + stick.left;
	prevY = (stick.bottom - stick.top)/2 + stick.top;
	for (int i = 0; i < 4; i++)
	{
		if (pressed_keys[i])
		{
			UnpressKey(vk_codes[i]);
			pressed_keys[i] = false;
		}
	}
}

void JoystickButton::Draw(IDirect3DDevice9* pDevice, ID3DXSprite *sprite)
{
	if (isDrawable)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		if (mainTexture)
		{
			D3DXVECTOR3 drawPosBox;
			drawPosBox.x = (float)pos.left;
			drawPosBox.y = (float)pos.top;
			drawPosBox.z = 1.0f;

			RECT drawAreaBox;
			drawAreaBox.left = drawAreaBox.top = 0;
			drawAreaBox.right = pos.right - pos.left;
			drawAreaBox.bottom = pos.bottom - pos.top;

			sprite->Draw((isPressed && clickedTexture) ? clickedTexture : mainTexture, &drawAreaBox, NULL, &drawPosBox, 0xFFFFFFFF);
		}
		if (textureStick)
		{
			D3DXVECTOR3 drawPosStick;
			drawPosStick.x = (float)stick.left;
			drawPosStick.y = (float)stick.top;
			drawPosStick.z = 1.0f;

			RECT drawAreaStick;
			drawAreaStick.left = drawAreaStick.top = 0;
			drawAreaStick.right = stick.right - stick.left;
			drawAreaStick.bottom = stick.bottom - stick.top;

			sprite->Draw((isPressed && clickedStick) ? clickedStick : textureStick, &drawAreaStick, NULL, &drawPosStick, 0xFFFFFFFF);
		}
		sprite->End();
	}
}

void JoystickButton::MoveStick(int newX, int newY)
{
	int stickWidth = stick.right - stick.left,
		stickHeight = stick.bottom - stick.top;

	stick.left = newX - stickWidth/2;
	stick.right = newX + stickWidth/2;
	stick.top = newY - stickHeight/2;
	stick.bottom = newY + stickHeight/2;
}

void JoystickButton::Action(UINT code, WPARAM wParam, LPARAM lParam)
{
	if (code == WM_POINTERDOWN)
		isPressed = true;
	else if (code == WM_POINTERUP)
		isPressed = false;

	int newX = GET_X_LPARAM(lParam),
		newY = GET_Y_LPARAM(lParam);

	if (newX < pos.left)
		newX = pos.left;
	else if (newX > pos.right)
		newX = pos.right;

	if (newY < pos.top)
		newY = pos.top;
	else if (newY > pos.bottom)
		newY = pos.bottom;

	if (code != WM_POINTERUPDATE || newX != prevX || newY != prevY)
	{
		if (textureStick)
			MoveStick(newX, newY);

		int boxWidth = pos.right - pos.left,
			boxHeight = pos.bottom - pos.top;
		int centerX = boxWidth/2 + pos.left,
			centerY = boxHeight/2 + pos.top,
			posX = newX - centerX,
			posY = newY - centerY;
		int persentX = 100 * posX / (boxWidth/2),
			persentY = 100 * posY / (boxHeight/2);

		if (!func)
		{
			const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;
			bool press[4] = { 0 };

			int ind = DOWN;
			if (persentY < 0) {
				persentY = -persentY;
				ind = UP;
			}
			if (persentY >= deadzone)
				press[ind] = true;
			
			ind = RIGHT;
			if (persentX < 0) {
				persentX = -persentX;
				ind = LEFT;
			}
			if (persentX >= deadzone)
				press[ind] = true;

			for (int i = 0; i < 4; i++) {
				if (press[i] != pressed_keys[i]) {
					if (press[i]) {
						PressKey(vk_codes[i]);
						pressed_keys[i] = true;
					}
					else {
						UnpressKey(vk_codes[i]);
						pressed_keys[i] = false;
					}
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
				((void(_cdecl*)(int,int,POINTER_STATE))func)(persentX, persentY, state);
			else
			{
				ThreadArg *arg = new ThreadArg;
				arg->persentX = persentX;
				arg->persentY = persentY;
				arg->state = state;
				arg->stickfunc = (void(_cdecl*)(int,int,POINTER_STATE))func;
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

void JoystickButton::StartThread(void *args)
{
	ThreadArg *arg = (ThreadArg*) args;
	arg->stickfunc(arg->persentX, arg->persentY, arg->state);
	delete arg;
}