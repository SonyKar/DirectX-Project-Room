#include "Test.h"

#include "GameCore.h"
#include "Tiger.h"

Test::Test(CXCamera* camera) : Scene(camera)
{
	meshes_.push_back(Tiger(D3DXVECTOR3(0, 0, 0), 0, 0, 0));
}

void Test::setup_view_matrix()
{
    const float distance = 0.01f * direction;
    
    currentStep = currentStep + direction;
    if (currentStep > maximum_step_direction)
    {
        direction = -1;
    }
    if (currentStep < 0)
        direction = 1;
    GameCore::camera->MoveForward(distance);

    GameCore::camera->Update();
}
