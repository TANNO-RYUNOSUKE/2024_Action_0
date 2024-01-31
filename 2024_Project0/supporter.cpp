#include "Supporter.h"

// ベクトルを角度に変換する関数
D3DXVECTOR3 VectorToAngles(const D3DXVECTOR3& vector)
{
	D3DXVECTOR3 angles;

	// Yaw（ヨー）を計算
	angles.y = atan2f(vector.x, vector.z);

	// Pitch（ピッチ）を計算
	angles.x = atan2f(vector.y, sqrt(vector.x * vector.x + vector.z * vector.z));

	// Roll（ロール）は0度に設定
	angles.z = 0.0f;



	return angles;
}

// 角度をベクトルに変換する関数
D3DXVECTOR3 AnglesToVector(const D3DXVECTOR3& angles)
{
	D3DXVECTOR3 vector;

	// ラジアンを度に変換
	float yaw = (angles.y);
	float pitch = (angles.x);

	// ベクトルを計算
	vector.x = sinf(yaw) * cosf(pitch);
	vector.y = sinf(pitch);
	vector.z = cosf(yaw) * cosf(pitch);

	return vector;
}

float ComparisonAngle(const D3DXVECTOR3& Direction1, const D3DXVECTOR3& Direction2)
{
	// ベクトルの正規化
	D3DXVECTOR3 normalizedCameraDir, normalizedEnemyDir;
	D3DXVec3Normalize(&normalizedCameraDir, &Direction1);
	D3DXVec3Normalize(&normalizedEnemyDir, &Direction2);
	// ベクトルの内積を計算
	float dotProduct = D3DXVec3Dot(&normalizedCameraDir, &normalizedEnemyDir);
	float fAnswer = acosf(dotProduct) * (180.0f / D3DX_PI);
	if (fAnswer < 0.0f)
	{
		fAnswer *= -1;
	}
	// ラジアンから度数法に変換して返す
	return fAnswer;
}

//=============================================
//距離取得関数
//=============================================
float GetDistance(D3DXVECTOR3 vec)
{
	if (vec.x < 0.0f)
	{
		vec.x *= -1.0f;
	}
	if (vec.y < 0.0f)
	{
		vec.y *= -1.0f;
	}
	if (vec.z < 0.0f)
	{
		vec.z *= -1.0f;
	}
	float fDis = vec.x + vec.y + vec.z;
	return fDis;
}

float GetAngleDifference(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2)
{
	// ベクトルを正規化します。
	D3DXVECTOR3 v1, v2;
	D3DXVec3Normalize(&v1, &vec1);
	D3DXVec3Normalize(&v2, &vec2);

	// ドット積を計算して、角度を求めます。
	float dotProduct = D3DXVec3Dot(&v1, &v2);
	float angle = acosf(dotProduct);
	if (angle < 0.0f)
	{
		angle *= -1.0f;
	}
	// 角度の差を返します。
	return angle;
}