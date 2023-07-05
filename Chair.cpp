#include "Chair.h"

Chair::Chair(D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle) : Mesh(new char[]("chair.x"), position, xAngle, yAngle, zAngle) {}