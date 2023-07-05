#include "GameCore.h"
#include "scene.h"

Scene::Scene(CXCamera* camera)
{
	camera_ = camera;
}

Scene::~Scene()
{
	release();
}

HRESULT Scene::InitGeometry()
{
	for (auto it = meshes_.begin(); it != meshes_.end(); ++it)
	{
		if (FAILED(it->load_mesh()))
			return E_FAIL;
	}

	if (FAILED(skybox_->load_mesh()))
		return E_FAIL;
	return S_OK;
}

void Scene::setup_matrices()
{
	setup_view_matrix();
	setup_projection_matrix();
}

void Scene::setup_view_matrix()
{
	GameCore::camera->Update();
}

void Scene::setup_projection_matrix()
{
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 3, 1.0f, 1.0f, 1000.0f);
	GameCore::d3d_device->SetTransform(D3DTS_PROJECTION, &matProj);
}

Mesh* Scene::get_intersected_mesh()
{
	for (auto it = meshes_.begin(); it != meshes_.end(); ++it)
	{
		if (it->is_mesh_selected(camera_->GetPosition(), camera_->GetLookAt())) return &*it;
	}
	return nullptr;
}

void Scene::render()
{
	GameCore::d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	skybox_->setup_world_matrix();
	skybox_->render();

	for (auto it = meshes_.begin(); it != meshes_.end(); ++it)
	{
		it->setup_world_matrix();
		it->render();
	}
}

void Scene::release()
{
	for (auto it = meshes_.begin(); it != meshes_.end(); ++it)
	{
		it->release();
	}
	skybox_->release();
	delete skybox_;
}
