#pragma once
#include "ISingleFingerButton.h"

class PushButton : public ISingleFingerButton {
private:
	char vk_code;

protected:
	void *func;

	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) override;

public:
	PushButton(int id, int x, int y, int width, int height, char vk_button);
	PushButton(int id, int x, int y, int width, int height, void(_cdecl *pushfunc)(POINTER_STATE));
};