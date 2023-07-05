#include "Room.h"

Room::Room(D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle) : Mesh(new char[]("room.x"), position, xAngle, yAngle, zAngle, false) {}