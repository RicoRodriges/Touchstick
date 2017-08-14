#pragma once
#include "MoveButton.h"

class JoystickButton : public MoveButton {
private:
	struct ThreadArg {
		char persentX;
		char persentY;
		POINTER_STATE state;
		void (_cdecl *stickfunc)(int,int,POINTER_STATE);
	};
	static void __cdecl StartThread(void*);
	ISingleFingerButton::setTexture;

protected:
	IDirect3DTexture9 *textureStick, *clickedStick;
	RECT stick;
	int deadzone; // %
	char vk_codes[4]; // UP, RIGHT, DOWN, LEFT
	bool pressed_keys[4];

	void InitStick(int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight);
	void ResetStick();
	void MoveStick(int x, int y);

	virtual void Draw(IDirect3DDevice9* pDevice, ID3DXSprite *sprite) override;
	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) override;

public:
	JoystickButton(int id, int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight, UINT8 deadzone,
		char keyUp, char keyRight, char keyDown, char keyLeft);
	JoystickButton(int id, int x, int y, int boxWidth, int boxHeight, int stickWidth, int stickHeight,
		void(_cdecl *stickfunc)(int persentX, int persentY, POINTER_STATE state));

	~JoystickButton();

	void setBoxTexture(IDirect3DDevice9 *pDevice, TCHAR *textureBoxPath, TCHAR *textureClickedBoxPath = 0);
	void setStickTexture(IDirect3DDevice9 *pDevice, TCHAR *stickTexturePath, TCHAR *textureClickedStickPath = 0);
};