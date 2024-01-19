#include "Supporter.h"

// �x�N�g�����p�x�ɕϊ�����֐�
D3DXVECTOR3 VectorToAngles(const D3DXVECTOR3& vector)
{
	D3DXVECTOR3 angles;

	// Yaw�i���[�j���v�Z
	angles.y = atan2(vector.x, vector.z);

	// Pitch�i�s�b�`�j���v�Z
	angles.x = atan2(vector.y, sqrt(vector.x * vector.x + vector.z * vector.z));

	// Roll�i���[���j��0�x�ɐݒ�
	angles.z = 0.0f;



	return angles;
}

// �p�x���x�N�g���ɕϊ�����֐�
D3DXVECTOR3 AnglesToVector(const D3DXVECTOR3& angles)
{
	D3DXVECTOR3 vector;

	// ���W�A����x�ɕϊ�
	float yaw = (angles.y);
	float pitch = (angles.x);

	// �x�N�g�����v�Z
	vector.x = sin(yaw) * cos(pitch);
	vector.y = sin(pitch);
	vector.z = cos(yaw) * cos(pitch);

	return vector;
}