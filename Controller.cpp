#include "Controller.h"
#include "GameCore.h"

Controller::Controller(CXCamera* camera) : camera_(camera), g_pDin(nullptr), g_pDinKeyboard(nullptr), g_KeyState{},
                                           g_pDinMouse(nullptr),
                                           g_pMouseState()
{
}

HRESULT Controller::InitDInput(HINSTANCE hInstance, HWND hWnd)
{
    DirectInput8Create(hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&g_pDin,
        NULL);

    g_pDin->CreateDevice(GUID_SysKeyboard,
        &g_pDinKeyboard,
        NULL);
    g_pDin->CreateDevice(GUID_SysMouse,
        &g_pDinMouse,
        NULL);
    
    g_pDinKeyboard->SetDataFormat(&c_dfDIKeyboard);
    
    g_pDinMouse->SetDataFormat(&c_dfDIMouse);
    
    g_pDinKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    
    g_pDinMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);


    return S_OK;
}

void Controller::handle_movement() const
{
    BOOL playsound = false;
    const auto movingDirection = new D3DXVECTOR3(0, 0, 0);
    if (g_KeyState[DIK_W] || g_KeyState[DIK_UPARROW])
    {
        playsound = true;
        movingDirection->x += camera_->GetLookAt()->x;
        movingDirection->z += camera_->GetLookAt()->z;
    }
    if (g_KeyState[DIK_S] || g_KeyState[DIK_DOWNARROW])
    {
        playsound = true;
        movingDirection->x -= camera_->GetLookAt()->x;
        movingDirection->z -= camera_->GetLookAt()->z;
    }
    if (g_KeyState[DIK_D] || g_KeyState[DIK_RIGHTARROW])
    {
        playsound = true;
        movingDirection->x += camera_->GetRight()->x;
        movingDirection->z += camera_->GetRight()->z;
    }
    if (g_KeyState[DIK_A] || g_KeyState[DIK_LEFTARROW])
    {
        playsound = true;
        movingDirection->x -= camera_->GetRight()->x;
        movingDirection->z -= camera_->GetRight()->z;
    }

    if (GameCore::selectedObject != nullptr)
    {
        if (playsound) GameCore::audioManager->PlaySound();
        GameCore::selectedObject->move_in_direction(speed_, movingDirection);
    }
    else camera_->MoveInDirection(speed_, movingDirection);
}

void Controller::handle_camera()
{
    alphaRight -= (float)g_pMouseState.lY * angleSpeed;
    alphaUp += (float)g_pMouseState.lX * angleSpeed;

    if (alphaRight > 90) alphaRight = 90;
    else if (alphaRight < -90) alphaRight = -90;

    D3DXVECTOR3* position = camera_->GetPosition();
    D3DXVECTOR3* lookAt = new D3DXVECTOR3(
        sin(alphaUp) * cos(alphaRight),
        sin(alphaRight),
        cos(alphaUp) * cos(alphaRight)
    );
    camera_->LookAtPos(position, new D3DXVECTOR3(position->x + lookAt->x, position->y + lookAt->y, position->z + lookAt->z), new D3DXVECTOR3(0, 1, 0));
}

void Controller::handle_object_selection()
{
    if (g_KeyState[DIK_F] && !isSelectButtonPressed)
    {
        isSelectButtonPressed = true;

        if (GameCore::selectedObject != nullptr)
        {
            GameCore::selectedObject = nullptr;
            return;
        }

        Mesh* tmp = GameCore::currentScene->get_intersected_mesh();
        if (tmp != nullptr && tmp->is_selectable())
        {
            GameCore::selectedObject = tmp;
            MessageBox(NULL, "Chair was intersected", "Meshes.exe", MB_OK);
        }
    }
    if (!g_KeyState[DIK_F]) isSelectButtonPressed = false;
}

void Controller::handle_object_rotation() const
{
    if (GameCore::selectedObject == nullptr) return;

    float xAngleDiff = 0, yAngleDiff = 0, zAngleDiff = 0;
    if (g_KeyState[DIK_Y])
    {
        xAngleDiff += angleSpeed;
    }
    if (g_KeyState[DIK_H])
    {
        xAngleDiff -= angleSpeed;
    }
    if (g_KeyState[DIK_U])
    {
        yAngleDiff += angleSpeed;
    }
    if (g_KeyState[DIK_J])
    {
        yAngleDiff -= angleSpeed;
    }
    if (g_KeyState[DIK_I])
    {
        zAngleDiff += angleSpeed;
    }
    if (g_KeyState[DIK_K])
    {
        zAngleDiff -= angleSpeed;
    }

    GameCore::selectedObject->rotate(xAngleDiff, yAngleDiff, zAngleDiff);
}

void Controller::HandleControls() 
{
    g_pDinKeyboard->Acquire();
    g_pDinMouse->Acquire();

    g_pDinKeyboard->GetDeviceState(256, (LPVOID)g_KeyState);
    g_pDinMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&g_pMouseState);

    handle_movement();
    handle_camera();
    handle_object_selection();
    handle_object_rotation();
}

Controller::~Controller()
{
    g_pDinKeyboard->Unacquire();
    g_pDinMouse->Unacquire();
    g_pDin->Release(); 
}