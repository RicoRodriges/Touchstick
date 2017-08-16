# Touchstick
Do you have a tablet based on Windows 8 or later without keyboard?

This project helps to draw controls in the game window as well as imitates pressing a keyboard buttons and moving a mouse.

### Currently supported:
- [ ] DirectDraw
- [ ] DirectX 8
- [x] DirectX 9
- [ ] DirectX 10
- [ ] DirectX 11

### Currently imitated:
- [x] Keyboard
- [x] Mouse
- [ ] DirectInput
- [ ] XInput

## Installation:
1. Edit `UserFunctions.cpp` file
1. Compile using Visual Studio or another compiler (you may also need DirectX Software Development Kit)
1. Inject DLL. For example you could use Cheat Engine

## Available classes:
- `PushButton` is a simple button like a keyboard key
- `MoveButton` is a rectangular zone which detect the movement of the finger like a touchpad
- `JoystickButton` is a rectangular joystick
- `CircularJoystickButton` is a round joystick

## Example
Worms Armageddon (Direct3D 9 mode):
![Worms Armageddon](https://github.com/RicoRodriges/Touchstick/raw/master/example/Worms%20Armageddon/screenshot.png)