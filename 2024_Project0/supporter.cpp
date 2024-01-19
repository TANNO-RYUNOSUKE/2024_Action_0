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