#pragma once
#include <cmath>
#include "JoystickButton.h"

#define PI 3.14159265358979323846l

class CircularJoystickButton : public JoystickButton
{
private:
	struct ThreadArg {
		char persent;
		double angle;
		POINTER_STATE state;
		void(_cdecl *stickfunc)(int,double,POINTER_STATE);
	};
	static void __cdecl StartThread(void*);

protected:
	virtual void Action(UINT code, WPARAM wParam, LPARAM lParam) override;

public:
	CircularJoystickButton(int id, int x, int y, int boxRadius, int stickRadius, UINT8 deadzone,
		char keyUp, char keyRight, char keyDown, char keyLeft)
		: JoystickButton(id, x, y, boxRadius * 2, boxRadius * 2, stickRadius * 2, stickRadius * 2, deadzone,
			keyUp, keyRight, keyDown, keyLeft) {};
	CircularJoystickButton(int id, int x, int y, int boxRadius, int stickRadius,
		void(_cdecl *stickfunc)(int persent, double angle, POINTER_STATE pressed))
		: JoystickButton(id, x, y, boxRadius * 2, boxRadius * 2, stickRadius * 2, stickRadius * 2,
		(void(_cdecl*)(int, int, POINTER_STATE))stickfunc) {};
};