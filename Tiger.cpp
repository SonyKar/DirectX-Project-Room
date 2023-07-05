#include "Tiger.h"

Tiger::Tiger(D3DXVECTOR3 position, float xAngle, float yAngle, float zAngle) : Mesh(new char[]("tiger.x"), position, xAngle, yAngle, zAngle) {}
