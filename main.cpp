#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "GameCore.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

VOID Render()
{
    GameCore::d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    if (SUCCEEDED(GameCore::d3d_device->BeginScene()))
    {
        GameCore::currentScene->setup_matrices();
        GameCore::currentScene->render();

        GameCore::d3d_device->EndScene();
    }

    GameCore::d3d_device->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        GameCore::cleanup();
        PostQuitMessage(0);
        return 0;
    case WM_GRAPHNOTIFY:
        GameCore::audioManager->HandleGraphEvent();
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "D3D Tutorial", NULL };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 06: Meshes",
        WS_OVERLAPPEDWINDOW, 0, 0, 1200, 700,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    HRESULT hr = CoInitialize(NULL);

     GameCore::audioManager->GetEventHandle(hWnd);

    if (SUCCEEDED(GameCore::InitD3D(hWnd)))
    {
        if (FAILED(GameCore::audioManager->InitDirectShow(hWnd)))
            return 0;

        if (SUCCEEDED(GameCore::currentScene->InitGeometry()))
        {
            GameCore::controller->InitDInput(hInst, hWnd);
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    GameCore::controller->HandleControls();
                    Render();
                }
            }
        }
    }

    UnregisterClass("D3D Tutorial", wc.hInstance);
    return 0;
}