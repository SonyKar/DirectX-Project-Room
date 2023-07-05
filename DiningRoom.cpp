#include "DiningRoom.h"

#include "Chair.h"
#include "Room.h"
#include "Skybox.h"

DiningRoom::DiningRoom(CXCamera* camera) : Scene(camera)
{
	meshes_.push_back(Chair(D3DXVECTOR3(0, 0, 0), 0, 0, 0));
	meshes_.push_back(Chair(D3DXVECTOR3(-1, 0, 6), 0, 90, 0));
	meshes_.push_back(Chair(D3DXVECTOR3(-8, 0, 6), 0, -90, 0));
	meshes_.push_back(Room(D3DXVECTOR3(-5, 0, -5), 0, 0, 0));
	skybox_ = new Skybox(D3DXVECTOR3(0, 0, 0), 0, 0, 0);
}
