#pragma once
#include <vector>
#include <Windows.h>

#include "Camera.h"
#include "Mesh.h"
#include "Skybox.h"

using namespace std;

class Scene
{
protected:
	vector<Mesh> meshes_;
	Skybox* skybox_ = nullptr;
	CXCamera* camera_;

public:
	Scene(CXCamera* camera);
	virtual ~Scene(); // cleanup

	HRESULT InitGeometry();
	VOID setup_matrices();
	virtual VOID setup_view_matrix();
	virtual VOID setup_projection_matrix();
	Mesh* get_intersected_mesh();
	VOID render();
	VOID release();
};

