#pragma once

#include <dinput.h>
#include "Camera.h"

class Controller
{
	CXCamera* camera_;
	LPDIRECTINPUT8			g_pDin;	
	LPDIRECTINPUTDEVICE8	g_pDinKeyboard;	
	BYTE					g_KeyState[256];

	LPDIRECTINPUTDEVICE8	g_pDinMouse;
	DIMOUSESTATE			g_pMouseState;

	float alphaUp = 0, alphaRight = 0;

	float speed_ = 0.05f;
	float angleSpeed = 0.001f;

	BOOL isSelectButtonPressed = false;

	void handle_movement() const;
	void handle_camera();
	void handle_object_selection();
	void handle_object_rotation() const;
public:
	Controller(CXCamera* camera);
	~Controller();
	void HandleControls();
	HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd);
};

