#include "stdafx.h"
#include "ISingleFingerButton.h"

ISingleFingerButton::ISingleFingerButton(int id, int x, int y, int width, int height)
	: IButton(id, x, y, width, height)
{
	touchID = 0;
	isPressed = false;
	mainTexture = clickedTexture = nullptr;
}

ISingleFingerButton::~ISingleFingerButton() {
	if (mainTexture)
		mainTexture->Release();
	if (clickedTexture)
		clickedTexture->Release();
}

void ISingleFingerButton::setTexture(IDirect3DDevice9 *pDevice, TCHAR *mainTexturePath, TCHAR *clickedTexturePath)
{
	if (mainTexture)
		mainTexture->Release();
	if (clickedTexture)
		clickedTexture->Release();
	clickedTexture = nullptr;

	if (mainTexturePath) {
		D3DXCreateTextureFromFileEx(pDevice, mainTexturePath, pos.right - pos.left, pos.bottom - pos.top,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
			&mainTexture);
		if (clickedTexturePath)
			D3DXCreateTextureFromFileEx(pDevice, clickedTexturePath, pos.right - pos.left, pos.bottom - pos.top,
				1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, nullptr, nullptr,
				&clickedTexture);

		isDrawable = true;
	}
}

void ISingleFingerButton::Draw(IDirect3DDevice9* pDevice, ID3DXSprite *sprite)
{
	if (isDrawable)
	{
		D3DXVECTOR3 drawPos;
		drawPos.x = (float)pos.left;
		drawPos.y = (float)pos.top;
		drawPos.z = 1.0f;

		RECT drawArea;
		drawArea.left = drawArea.top = 0;
		drawArea.right = pos.right - pos.left;
		drawArea.bottom = pos.bottom - pos.top;

		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		if (!clickedTexture)
			sprite->Draw(mainTexture, &drawArea, NULL, &drawPos, 0xFFFFFFFF);
		else
			sprite->Draw(isPressed ? clickedTexture : mainTexture, &drawArea, NULL, &drawPos, 0xFFFFFFFF);
		sprite->End();
	}
}

void ISingleFingerButton::AddPointer(INT32 p) { touchID = p; }

bool ISingleFingerButton::FoundPointer(INT32 p) { return touchID == p; }

void ISingleFingerButton::RemovePointer(INT32 p) { touchID = 0; }

bool ISingleFingerButton::IsPressed() { return isPressed; }

void ISingleFingerButton::setDrawable(bool isDrawable)
{
	if (!mainTexture)
		this->isDrawable = false;
	else
		this->isDrawable = isDrawable;
}