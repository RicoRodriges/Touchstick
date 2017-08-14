#pragma once
#include "PushButton.h"
#include "MoveButton.h"
#include "JoystickButton.h"
#include "CircularJoystickButton.h"

// Number of milliseconds and pixels to distinguish click from move
#define BUTTON_DELAY 100
#define DELTA_DISTANCE 6

// Virtual key code for enable/disable draw buttons
#define DRAW_BUTTONS_KEY VK_F1
#define DRAW_BUTTONS_AFTER_HOOK true

// Returns the handle of the game window
HWND getGameWindow();

// Function for create buttons
void InitButtons(std::list<IButton*> &listButtons, int windowWidth, int windowHeight, IDirect3DDevice9 *pDevice);