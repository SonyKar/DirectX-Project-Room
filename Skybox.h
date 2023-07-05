#pragma once
#include "Mesh.h"

class Skybox : public Mesh
{
public:
	Skybox(D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle);
	void setup_world_matrix() override;
};

