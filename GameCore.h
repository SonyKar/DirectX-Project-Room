#pragma once
#include <d3d9.h>

#include "AudioManager.h"
#include "scene.h"
#include "Camera.h"
#include "Controller.h"

class GameCore
{
public:
	static LPDIRECT3D9 d_3d;
	static LPDIRECT3DDEVICE9 d3d_device;
	static Scene* currentScene;
	static CXCamera* camera;
	static Controller* controller;
	static Mesh* selectedObject;
	static AudioManager* audioManager;

	static HRESULT InitD3D(HWND hWnd);
	static VOID cleanup();

private:
	static VOID initiate_camera();
};

