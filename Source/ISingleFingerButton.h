#pragma once
#include "IButton.h"

class ISingleFingerButton : public IButton {
protected:
	bool isPressed;
	INT32 touchID;
	IDirect3DTexture9 *mainTexture;
	IDirect3DTexture9 *clickedTexture;

	virtual void Draw(IDirect3DDevice9* pDevice, ID3DXSprite *sprite) override;
	virtual void AddPointer(INT32 p) override;
	virtual bool FoundPointer(INT32 p) override;
	virtual void RemovePointer(INT32 p) override;
	virtual bool IsPressed() override;

	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) override = 0;

public:
	ISingleFingerButton(int id, int x, int y, int width, int height);
	~ISingleFingerButton();

	virtual void setDrawable(bool isDrawable) override;

	void setTexture(IDirect3DDevice9 *pDevice, TCHAR *mainTexturePath, TCHAR *clickedTexturePath = 0);
};