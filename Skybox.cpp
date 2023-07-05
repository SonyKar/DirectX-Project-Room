#include "Skybox.h"

#include "GameCore.h"

Skybox::Skybox(D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle) : Mesh(new char[]("skybox2.x"), position, xAngle, yAngle, zAngle)
{}

void Skybox::setup_world_matrix()
{
	position = *GameCore::camera->GetPosition();
	Mesh::setup_world_matrix();
}
