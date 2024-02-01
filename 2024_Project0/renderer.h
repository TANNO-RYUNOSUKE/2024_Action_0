//==================
//�����_���[�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _RENDERER_H_//��d�C���N���[�h�h�~�̃}�N��
#define _RENDERER_H_
//�C���N���[�h
#include "main.h"
class CZTexture;	//Z�e�N�X�`��
class CDepthShadow; //�[�x�V���h�E
//�N���X��`
class CRenderer //�����_���[�N���X
{
public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void){return m_pD3DDevice;}
	CZTexture * GetZShader(void) { return m_pZTex; }
	CDepthShadow * GetDepthShader(void) { return m_pDepthShadow; }
private:
	ID3DXSprite * m_pSprite;	 //�V���h�E�p�X�v���C�g
	CZTexture * m_pZTex;		//Z�e�N�X�`�������p
	CDepthShadow * m_pDepthShadow;					//�[�x�e
	LPDIRECT3D9 m_pD3D;						//direct3D�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//direct3D�f�o�C�X�ւ̃|�C���^
	IDirect3DDevice9 * m_pDev; //�`��f�o�C�X

	D3DXMATRIX CameraView, CameraProj;	// �J�����r���[�ϊ��E�ˉe�ϊ�
	D3DXMATRIX LightView, LightProj;	// ���C�g�r���[�ϊ��E�ˉe�ϊ�

	IDirect3DTexture9 * m_pZTexture;				//Z�e�N�X�`���{��
};

#endif // ! _RENDERER_H_
