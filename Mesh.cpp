#include "Mesh.h"

#include "GameCore.h"

Mesh::Mesh(char filename[], D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle, BOOL isSelectable)
{
    this->position = position;
    this->xAngle = xAngle;
    this->yAngle = yAngle;
    this->zAngle = zAngle;
	this->filename = filename;
    this->isSelectable = isSelectable;
}

HRESULT Mesh::load_mesh()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,
        GameCore::d3d_device, NULL,
        &pD3DXMtrlBuffer, NULL, &numMaterials,
        &mesh_)))
    {
        char prefix[] = "..\\";
        strcat_s(prefix, filename);
        if (FAILED(D3DXLoadMeshFromX(prefix, D3DXMESH_SYSTEMMEM,
            GameCore::d3d_device, NULL,
            &pD3DXMtrlBuffer, NULL, &numMaterials,
            &mesh_)))
        {
            char error[] = "Could not find ";
            strcat_s(error, filename);

            MessageBox(NULL, error, "Meshes.exe", MB_OK);
            return E_FAIL;
        }
    }

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    meshMaterials = new D3DMATERIAL9[numMaterials];
    meshTextures = new LPDIRECT3DTEXTURE9[numMaterials];

    for (DWORD i = 0; i < numMaterials; i++)
    {
        meshMaterials[i] = d3dxMaterials[i].MatD3D;
        meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
        meshTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
        {
            if (FAILED(D3DXCreateTextureFromFile(GameCore::d3d_device,
                d3dxMaterials[i].pTextureFilename,
                &meshTextures[i])))
            {
                const TCHAR* strPrefix = TEXT("..\\");
                const int lenPrefix = lstrlen(strPrefix);
                TCHAR strTexture[MAX_PATH];
                lstrcpyn(strTexture, strPrefix, MAX_PATH);
                lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
                if (FAILED(D3DXCreateTextureFromFile(GameCore::d3d_device,
                    strTexture,
                    &meshTextures[i])))
                {
                    MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
                }
            }
        }
    }

    pD3DXMtrlBuffer->Release();
    return S_OK;
}

void Mesh::render() const
{
    for (DWORD i = 0; i < numMaterials; i++)
    {
        GameCore::d3d_device->SetMaterial(&meshMaterials[i]);
        GameCore::d3d_device->SetTexture(0, meshTextures[i]);

        mesh_->DrawSubset(i);
    }
}

void Mesh::setup_world_matrix()
{
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity(&matWorld);

    D3DXMATRIX positionMatrix;
    D3DXMatrixIdentity(&positionMatrix);
    D3DXMatrixTranslation(&positionMatrix, 0, 0, 0);
    D3DXMatrixMultiply(&matWorld, &matWorld, &positionMatrix);
    
    D3DXMATRIX rotationMatrixX;
    D3DXMatrixIdentity(&rotationMatrixX);
    D3DXMatrixRotationX(&rotationMatrixX, xAngle);
    D3DXMatrixMultiply(&matWorld, &matWorld, &rotationMatrixX);
    
    D3DXMATRIX rotationMatrixY;
    D3DXMatrixIdentity(&rotationMatrixY);
    D3DXMatrixRotationY(&rotationMatrixY, yAngle);
    D3DXMatrixMultiply(&matWorld, &matWorld, &rotationMatrixY);
    
    D3DXMATRIX rotationMatrixZ;
    D3DXMatrixIdentity(&rotationMatrixZ);
    D3DXMatrixRotationZ(&rotationMatrixZ, zAngle);
    D3DXMatrixMultiply(&matWorld, &matWorld, &rotationMatrixZ);
    //
    D3DXMatrixIdentity(&positionMatrix);
    D3DXMatrixTranslation(&positionMatrix, position.x, position.y, position.z);
    D3DXMatrixMultiply(&matWorld, &matWorld, &positionMatrix);

    GameCore::d3d_device->SetTransform(D3DTS_WORLD, &matWorld);
}

BOOL Mesh::is_mesh_selected(D3DXVECTOR3* rayStartPos, D3DXVECTOR3* rayDirection) const
{
    BOOL* isHit = new BOOL(false);
    const auto rayDistance = new FLOAT(100.0f);
    D3DXIntersect(
        mesh_,
        rayStartPos,
        rayDirection,
        isHit,
        NULL,
        NULL,
        NULL,
        rayDistance,
        NULL,
        NULL
    );
    // if (*isHit)
    // {
    //     MessageBox(NULL, "Object was intersected", "Meshes.exe", MB_OK);
    // }
    // else MessageBox(NULL, "Object was not intersected", "Meshes.exe", MB_OK);

    return *isHit;
}

void Mesh::move_in_direction(float fDist, D3DXVECTOR3* Dir)
{
    D3DXVECTOR3 DirToMove(0, 0, 0);
    D3DXVec3Normalize(&DirToMove, Dir);
    position += fDist * DirToMove;
}

void Mesh::rotate(float xAngleDiff, float yAngleDiff, float zAngleDiff)
{
    xAngle += xAngleDiff;
    yAngle += yAngleDiff;
    zAngle += zAngleDiff;
}

void Mesh::release() const
{
	delete[] meshMaterials;

    if (meshTextures)
    {
        for (DWORD i = 0; i < numMaterials; i++)
        {
            if (meshTextures[i])
                meshTextures[i]->Release();
        }
        delete[] meshTextures;
    }

    delete filename;

    if (mesh_ != NULL)
        mesh_->Release();
}

BOOL Mesh::is_selectable() const
{
    return isSelectable;
}
