//========================================================================================
//
// 3Dオブジェクト
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "objectX.h"
#include "manager.h"
#include "texture.h"
#include "xfile.h"
#include "Xmanager.h"
#include<vector>


//=============================================
//コンストラクタ
//=============================================

CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{

	CXManager * pManger = CManager::GetInstance()->GetXManager();
	pManger->Regist(this);
	m_pVtxBuff = NULL;
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_bLight = true;
}



//=============================================
//デストラクタ
//=============================================
CObjectX::~CObjectX()
{
}


//=============================================
//ポリゴン初期化関数
//=============================================
HRESULT CObjectX::Init()
{
	int nNumVtx = 0; //頂点数
	DWORD dwSizeFVF; //頂点フォーマットのサイズ
	BYTE*pVtxBuff = NULL; //頂点バッファへのポインタ
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();


	CTexture * pTex = CManager::GetInstance()->GetTexture();
	CXFile * pXFile = CManager::GetInstance()->GetXFiles();
	m_nIdxXFile = pXFile->Regist(m_pModelName);


	//頂点数を取得
	nNumVtx = pXFile->GetAddress(m_nIdxXFile)->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pXFile->GetAddress(m_nIdxXFile)->GetFVF());

	//頂点バッファをロック
	pXFile->GetAddress(m_nIdxXFile)->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		if (m_vtxMinModel.x > vtx.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		if (m_vtxMinModel.y > vtx.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		if (m_vtxMinModel.z > vtx.z)
		{
			m_vtxMinModel.z = vtx.z;
		}
		if (m_vtxMaxModel.x < vtx.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		if (m_vtxMaxModel.y < vtx.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		if (m_vtxMaxModel.z < vtx.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}
	//頂点バッファをアンロック
	pXFile->GetAddress(m_nIdxXFile)->UnlockVertexBuffer();

	//テクスチャの読み込み

	D3DXMATERIAL *pMat; //マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)pXFile->GetMaterial(m_nIdxXFile)->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)pXFile->GetNumMaterial(m_nIdxXFile); nCntMat++)
	{
	
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する時
			m_nIdxTex[nCntMat] = pTex->Regist(pMat[nCntMat].pTextureFilename);
		}
	}
	return S_OK;;
}
//=============================================
//生成関数
//=============================================
CObjectX * CObjectX::Create(char * pName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CObjectX * pObject3D = NULL;

	pObject3D = DBG_NEW  CObjectX(nPriority);
	pObject3D->m_pModelName = pName;
	pObject3D->SetPos(pos);
	pObject3D->SetRot(rot);
	pObject3D->Init();
	return pObject3D;
}



//=============================================
//ポリゴンの終了処理
//=============================================
void CObjectX::Uninit(void)
{
	CXManager * pManger = CManager::GetInstance()->GetXManager();
	if (m_nID >= 0)
	{
		pManger->Release(m_nID);
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================
//ポリゴンの更新処理
//=============================================
void CObjectX::Update(void)
{
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
}

//=============================================
//ポリゴンの描画処理
//=============================================
void CObjectX::Draw(void)
{
	CXFile * pXFile = CManager::GetInstance()->GetXFiles();
	CTexture * pTex = CManager::GetInstance()->GetTexture();
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
	D3DMATERIAL9 matDef; //現在のマテリアル保存用
	D3DXMATERIAL *pMat = NULL; //マテリアルデータへのポインタ

	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = GetRot();
	pos = GetPos();
	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	

	
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//ライティング
	pDevice->SetRenderState(D3DRS_LIGHTING, m_bLight);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	if (pXFile->GetMaterial(m_nIdxXFile) != NULL)
	{
		pMat = (D3DXMATERIAL*)pXFile->GetMaterial(m_nIdxXFile)->GetBufferPointer();

	}

	for (int nCntMat = 0; nCntMat < (int)pXFile->GetNumMaterial(m_nIdxXFile); nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTex->Getaddress((m_nIdxTex[nCntMat])));

		//モデル(パーツ)の描写
		pXFile->GetAddress(m_nIdxXFile)->DrawSubset(nCntMat);
		//m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//=============================================
//当たり判定
//=============================================
bool CObjectX::Collision(D3DXVECTOR3 prevPosition, D3DXVECTOR3* pCurrentPosition, D3DXVECTOR3* pMovement)
{
	
	
			if (pCurrentPosition->x > GetPos().x + m_vtxMinModel.x  && pCurrentPosition->x < GetPos().x + m_vtxMaxModel.x  && pCurrentPosition->z > GetPos().z + m_vtxMinModel.z  && pCurrentPosition->z < GetPos().z + m_vtxMaxModel.z && pCurrentPosition->y > GetPos().y + m_vtxMinModel.y  && pCurrentPosition->y < GetPos().y + m_vtxMaxModel.y)
			{
			

				if (prevPosition.y >= GetPos().y + m_vtxMaxModel.y && pCurrentPosition->y < GetPos().y + m_vtxMaxModel.y)
				{
					pCurrentPosition->y = GetPos().y + +m_vtxMaxModel.y;
					pMovement->y = 0.0f;
				}
				else if (prevPosition.z <= (GetPos().z + m_vtxMinModel.z) && pCurrentPosition->z >(GetPos().z + m_vtxMinModel.z))
				{
					pCurrentPosition->z = GetPos().z + m_vtxMinModel.z;
					pMovement->z = 0.0f;
				}

				else if (prevPosition.x <= (GetPos().x + m_vtxMinModel.x) && pCurrentPosition->x > (GetPos().x + m_vtxMinModel.x))
				{
					pCurrentPosition->x = GetPos().x + m_vtxMinModel.x;
					pMovement->x = 0.0f;
				}
				else if (prevPosition.x >= GetPos().x + m_vtxMaxModel.x && pCurrentPosition->x < GetPos().x + m_vtxMaxModel.x)
				{
					pCurrentPosition->x = GetPos().x + +m_vtxMaxModel.x;
					pMovement->x = 0.0f;
				}
				else if (prevPosition.z >= GetPos().z + m_vtxMaxModel.z && pCurrentPosition->z < GetPos().z + m_vtxMaxModel.z)
				{
					pCurrentPosition->z = GetPos().z + +m_vtxMaxModel.z;
					pMovement->z = 0.0f;
				}
				return true;
			}
		
	
	return false;
}
//=============================================
//レイ
//=============================================
bool CObjectX::IntersectTriangle(const D3DXVECTOR3& prevPosition, D3DXVECTOR3* pCurrentPosition, D3DXVECTOR3* pMovement, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3)
{
	// 三角形の法線ベクトルを計算
	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal, &(v2 - v1), &(v3 - v1));
	D3DXVec3Normalize(&normal, &normal);

	// 三角形の面方程式を計算 (Ax + By + Cz + D = 0)
	float A = normal.x;
	float B = normal.y;
	float C = normal.z;
	float D = -D3DXVec3Dot(&normal, &v1);

	// 移動ベクトルを取得
	D3DXVECTOR3 moveVector = *pCurrentPosition - prevPosition;

	// 移動ベクトルが三角形の面と交差するかチェック
	float dotProduct = A * moveVector.x + B * moveVector.y + C * moveVector.z;
	if (dotProduct != 0.0f)
	{
		float t = (-D - A * prevPosition.x - B * prevPosition.y - C * prevPosition.z) / dotProduct;

		// tが0から1の間にある場合、交差点を計算
		if (t >= 0.0f && t <= 1.0f)
		{
			D3DXVECTOR3 intersection = prevPosition + t * moveVector;

			// 交差点が三角形の内部にあるかどうかチェック
			D3DXVECTOR3 edge1 = v2 - v1;
			D3DXVECTOR3 edge2 = v3 - v2;
			D3DXVECTOR3 edge3 = v1 - v3;

			D3DXVECTOR3 c1 = D3DXVECTOR3(0, 0, 0);
			D3DXVECTOR3 c2 = v2 - v1;
			D3DXVECTOR3 c3 = v3 - v1;

			D3DXVECTOR3 p1 = intersection - v1;
			D3DXVECTOR3 p2 = intersection - v2;
			D3DXVECTOR3 p3 = intersection - v3;

			D3DXVECTOR3 test1, test2, test3;

			D3DXVec3Cross(&test1, &c1, &p1);
			D3DXVec3Cross(&test2, &c2, &p2);
			D3DXVec3Cross(&test3, &c3, &p3);

			if (D3DXVec3Dot(&test1, &normal) >= 0 && D3DXVec3Dot(&test2, &normal) >= 0 && D3DXVec3Dot(&test3, &normal) >= 0)
			{
				// 交差点が三角形の内部にある場合、座標を補正
				*pCurrentPosition = intersection;

				// 移動ベクトルを補正
				*pMovement = *pMovement - (intersection - prevPosition);

				return true;
			}
		}
	}

	return false;
}

D3DXVECTOR3  CObjectX::CalculateNormal(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3)
{
	D3DXVECTOR3 edge1 = v2 - v1;
	D3DXVECTOR3 edge2 = v3 - v1;

	// 外積を計算して法線ベクトルを得る
	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal, &edge1, &edge2);
	D3DXVec3Normalize(&normal, &normal);

	return normal;
}
bool CObjectX::Ray(D3DXVECTOR3 start, D3DXVECTOR3 end,D3DXVECTOR3 * pPos)
{

	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 max = m_vtxMaxModel;
	D3DXVECTOR3 min = m_vtxMinModel;

	D3DXVECTOR3 v1, v2, v3, v4, v5, v6, v7, v8;
	v1 = D3DXVECTOR3(min.x, max.y, max.z);
	v2 = D3DXVECTOR3(max.x, max.y, max.z);
	v3 = D3DXVECTOR3(max.x, max.y, min.z);
	v4 = D3DXVECTOR3(min.x, max.y, min.z);
	v5 = D3DXVECTOR3(min.x, min.y, max.z);
	v6 = D3DXVECTOR3(max.x, min.y, max.z);
	v7 = D3DXVECTOR3(max.x, min.y, min.z);
	v8 = D3DXVECTOR3(min.x, min.y, min.z);

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&v1, &v1, &m_mtxWorld);
	D3DXVec3TransformCoord(&v2, &v2, &m_mtxWorld);
	D3DXVec3TransformCoord(&v3, &v3, &m_mtxWorld);
	D3DXVec3TransformCoord(&v4, &v4, &m_mtxWorld);
	D3DXVec3TransformCoord(&v5, &v5, &m_mtxWorld);
	D3DXVec3TransformCoord(&v6, &v6, &m_mtxWorld);
	D3DXVec3TransformCoord(&v7, &v7, &m_mtxWorld);
	D3DXVec3TransformCoord(&v8, &v8, &m_mtxWorld);

	

	if (CManager::GetInstance()->RayTri(start, end, v3, v7, v4, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v4,v7, v8, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v3, v6, v7, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v3, v2, v6, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v2, v1, v6, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v6, v1, v5, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v1, v8, v5, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v1, v4, v8, pPos))
	{
		return true;
	}

	if (CManager::GetInstance()->RayTri(start, end, v1, v2, v3, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v3, v4, v1, pPos))
	{
		return true;
	}

	if (CManager::GetInstance()->RayTri(start, end, v5, v6, v7, pPos))
	{
		return true;
	}
	if (CManager::GetInstance()->RayTri(start, end, v7, v8, v5, pPos))
	{
		return true;
	}
	return false;
}
CDebri::CDebri(int nPriority) : CObjectX(nPriority)
{
	CXManager * pManger = CManager::GetInstance()->GetXManager();
	pManger->Release(m_nID);
}

CDebri::~CDebri()
{
}
HRESULT CDebri::Init()
{
	CObjectX::Init();
	return S_OK;
}
void CDebri::Uninit()
{
	CObjectX::Uninit();
}
void CDebri::Update()
{
	CObjectX::Update();
	SetPos(GetPos() + move);
	SetRot(GetRot() + rotation);
	m_nLife--;
	if (m_nLife <= 0)
	{

		Release();
	}
}
void CDebri::Draw()
{
	CObjectX::Draw();
}
CDebri * CDebri::Create(std::string pName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotation, int nLife, int nPriority )
{
	CDebri * pObject3D = NULL;

	pObject3D = DBG_NEW  CDebri(nPriority);
	pObject3D->SetName(pName);
	pObject3D->SetPos(pos);
	pObject3D->SetRot(rot);
	pObject3D->move = move;
	pObject3D->rotation = rotation;
	pObject3D->m_nLife = nLife;
	pObject3D->Init();
	return pObject3D;
}

