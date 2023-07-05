#include "GameCore.h"

#include "DiningRoom.h"

LPDIRECT3D9 GameCore::d_3d;
LPDIRECT3DDEVICE9 GameCore::d3d_device;
Scene* GameCore::currentScene;
CXCamera* GameCore::camera;
Controller* GameCore::controller;
Mesh* GameCore::selectedObject;
AudioManager* GameCore::audioManager = new AudioManager();

HRESULT GameCore::InitD3D(HWND hWnd)
{
    if (NULL == (d_3d = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if (FAILED(d_3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &d3d_device)))
    {
        if (FAILED(d_3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp, &d3d_device)))
            return E_FAIL;
    }

    d3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);

    d3d_device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    initiate_camera();

    currentScene = new DiningRoom(camera);
    controller = new Controller(camera);
    return S_OK;
}

void GameCore::cleanup()
{
    delete audioManager;

    if (controller != NULL)
    {
        delete controller;
    }

    if (currentScene != NULL)
    {
        delete currentScene;
    }

    if (d3d_device != NULL)
    {
        d3d_device->Release();
    }

	if (d_3d != NULL)
	{
		d_3d->Release();
	}
}

void GameCore::initiate_camera()
{
    camera = new CXCamera(d3d_device);

    D3DXVECTOR3 vEyePt(0.0f,10.0f, -20.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 5.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

    camera->LookAtPos(&vEyePt, &vLookatPt, &vUpVec);
}
