
#ifndef _supporter_H_
#define _supporter_H_
#include "main.h"
D3DXVECTOR3 VectorToAngles(const D3DXVECTOR3& vector);
D3DXVECTOR3 AnglesToVector(const D3DXVECTOR3& angles);
float ComparisonAngle(const D3DXVECTOR3& Direction1, const D3DXVECTOR3& Direction2);
float GetDistance(D3DXVECTOR3 vec);
float GetAngleDifference(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2);
#endif
