#pragma once
#include <Windows.h>
#include <d3dx9.h>

class Mesh
{
protected:
	BOOL isSelectable;

	D3DXVECTOR3 position;
	float xAngle, yAngle, zAngle;

	char* filename;
	LPD3DXMESH mesh_ = NULL;
	D3DMATERIAL9* meshMaterials = NULL;
	LPDIRECT3DTEXTURE9* meshTextures = NULL;
	DWORD numMaterials = 0L;

public:
	Mesh(char filename[], D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle, BOOL isSelectable = true);
	HRESULT load_mesh();
	VOID render() const;
	virtual VOID setup_world_matrix();
	BOOL is_mesh_selected(D3DXVECTOR3* rayStartPos, D3DXVECTOR3* rayDirection) const;
	VOID move_in_direction(float fDist, D3DXVECTOR3* Dir);
	VOID rotate(float xAngleDiff, float yAngleDiff, float zAngleDiff);
	VOID release() const;
	BOOL is_selectable() const;
};

