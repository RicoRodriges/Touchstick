#pragma once
#include "ISingleFingerButton.h"

class MoveButton : public ISingleFingerButton {
private:
	struct ThreadArg {
		POINT p[2];
		POINTER_STATE state;
		void(_cdecl *movefunc)(int,int,int,int, POINTER_STATE);
	};

	static void __cdecl StartThread(void*);

protected:
	void *func;
	int prevX, prevY;

	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) override;

public:
	MoveButton(int id, int x, int y, int width, int height,
		void(_cdecl *movefunc)(int prevX, int prevY, int newX, int newY, POINTER_STATE state) = 0);
};