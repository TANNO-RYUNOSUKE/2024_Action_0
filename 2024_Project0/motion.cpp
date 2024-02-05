//========================================================================================
//
// モーション
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "motion.h"
#include "debugproc.h"
#include "manager.h"
#include <stdio.h>

//=============================================
//コンストラクタ
//=============================================

CMotion::CMotion()
{
	m_bFinish = false;
	m_bLoop = false;
	m_nCounter = 0;
	m_nNumModel = 0;
	m_nkey = 0;
	m_nNumAll = 0;
	m_nType = 0;
	m_bBlend = false;
	for (int i = 0; i < NUM_KEY_MOTION; i++)
	{
		m_Prevkey[i] = {};
	}
	
	//for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	//{
	//	m_ppModel[nCnt] = DBG_NEW CModel*;
	//}
}
	



//=============================================
//デストラクタ
//=============================================
CMotion::~CMotion()
{
}
void CMotion::Uninit(void)
{
	/*for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			delete m_ppModel[nCnt];
			m_ppModel[nCnt] = NULL;
		}
	}*/
}
void CMotion::Update(void)
{
	CDebugProc * pDeb = CManager::GetInstance()->GetDeb();
	if (m_nType > -1 && m_nType <= m_nNumAll)
	{
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			KEY anim;

			if (m_Info[m_nType].nNumkey != 0)
			{
				if (m_bBlend)
				{
					anim.pos_x = 0.0f;
					anim.pos_y = 0.0f;
					anim.pos_z = 0.0f;
					anim.rot_x = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_x - m_Prevkey[nCnt].rot_x;
					anim.rot_y = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_y - m_Prevkey[nCnt].rot_y;
					anim.rot_z = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_z - m_Prevkey[nCnt].rot_z;
				}
				else
				{
					if (m_nkey >= m_Info[m_nType].nNumkey - 1 && !m_Info[m_nType].bLoop)
					{
						anim.pos_x = m_Info[0].aKeyInfo[0].aKey[nCnt].pos_x - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_x;
						anim.pos_y = m_Info[0].aKeyInfo[0].aKey[nCnt].pos_y - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_y;
						anim.pos_z = m_Info[0].aKeyInfo[0].aKey[nCnt].pos_z - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_z;
						anim.rot_x = m_Info[0].aKeyInfo[0].aKey[nCnt].rot_x - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_x;
						anim.rot_y = m_Info[0].aKeyInfo[0].aKey[nCnt].rot_y - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_y;
						anim.rot_z = m_Info[0].aKeyInfo[0].aKey[nCnt].rot_z - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_z;
					}
					else
					{
						anim.pos_x = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].pos_x - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_x;
						anim.pos_y = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].pos_y - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_y;
						anim.pos_z = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].pos_z - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_z;
						anim.rot_x = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_x - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_x;
						anim.rot_y = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_y - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_y;
						anim.rot_z = m_Info[m_nType].aKeyInfo[(m_nkey + 1) % m_Info[m_nType].nNumkey].aKey[nCnt].rot_z - m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_z;
					}
					
					
				}
			}

			if (anim.rot_x < -D3DX_PI)
			{
				anim.rot_x += D3DX_PI * 2;
			}
			else if (anim.rot_x > D3DX_PI)
			{
				anim.rot_x += -D3DX_PI * 2;
			}
			if (anim.rot_y < -D3DX_PI)
			{
				anim.rot_y += D3DX_PI * 2;
			}
			else if (anim.rot_y > D3DX_PI)
			{
				anim.rot_y += -D3DX_PI * 2;
			}
			if (anim.rot_z < -D3DX_PI)
			{
				anim.rot_z += D3DX_PI * 2;
			}
			else if (anim.rot_z > D3DX_PI)
			{
				anim.rot_z += -D3DX_PI * 2;
			}

			//パーツの位置、向きを設定
			D3DXVECTOR3 pos ;
			D3DXVECTOR3 rot;
			if (m_bBlend)
			{
				pos = D3DXVECTOR3(m_Prevkey[nCnt].pos_x,
					m_Prevkey[nCnt].pos_y,
					m_Prevkey[nCnt].pos_z) +
					D3DXVECTOR3(anim.pos_x, anim.pos_y, anim.pos_z) * ((float)m_nCounter / (float)m_Info[m_nType].aKeyInfo[m_nkey].nFrame);
				rot = D3DXVECTOR3(m_Prevkey[nCnt].rot_x,
					 m_Prevkey[nCnt].rot_y,
					 m_Prevkey[nCnt].rot_z) +
					D3DXVECTOR3(anim.rot_x, anim.rot_y, anim.rot_z) *  ((float)m_nCounter / (float)m_Info[m_nType].aKeyInfo[m_nkey].nFrame);
			}
			else
			{
				pos = D3DXVECTOR3(m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_x,
					m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_y,
					m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].pos_z) +
					D3DXVECTOR3(anim.pos_x, anim.pos_y, anim.pos_z) * ((float)m_nCounter / (float)m_Info[m_nType].aKeyInfo[m_nkey].nFrame);
				rot = D3DXVECTOR3(m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_x,
					m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_y,
					m_Info[m_nType].aKeyInfo[m_nkey].aKey[nCnt].rot_z) +
					D3DXVECTOR3(anim.rot_x, anim.rot_y, anim.rot_z) *  ((float)m_nCounter / (float)m_Info[m_nType].aKeyInfo[m_nkey].nFrame);
			}
			//pDeb->Print("モーション角度(x:%f,y:%f,z:%f)\n", rot.x, rot.y, rot.z);

			(*m_ppModel[nCnt])->SetPos((*m_ppModel[nCnt])->GetOffset() +pos);
			(*m_ppModel[nCnt])->SetRot(rot);
			
		}
		m_nCounter++;
		if (m_nCounter >= m_Info[m_nType].aKeyInfo[m_nkey].nFrame)
		{
			m_bBlend = false;
			m_nkey++;
			m_nCounter = 0;
		}
		if (m_nkey >= m_Info[m_nType].nNumkey)
		{
			if (m_Info[m_nType].bLoop == true)
			{
				m_nkey = 0;
			}
			else
			{
				m_nkey = 0;
				m_nType = 0;
			}
		}
	}
}

void CMotion::SetInfo(INFO Info)
{
	if (m_nNumAll < NUM_INFO)
	{
		m_Info[m_nNumAll] = Info;
		m_nNumAll++;
	}
}




//====================================================================
//モーションのロード処理
//====================================================================
void CMotion::Load(char * cFile)
{
	FILE *pFile; //ファイルポインタを宣言

				 //ファイルを開く
	//pFile = fopen("", "r");

	//ファイルを開く
	pFile = fopen(cFile, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		int nNum = 0;//モデルの最大数
		int nNumMotion = 0;
		int nNumParts = 0;

		char aString[256] = {};				//ゴミ箱
		char aStartMessage[256] = {};		//スタートメッセージ
		char aModelMessage[256] = {};		//モデル用メッセージ
		char aCharaSetMessage[NUM_MODEL][128] = {};	//キャラセットメッセージ
		char aPartsSetMessage[128] = {};	//パーツセットメッセージ
		char aMotionSetMessage[128] = {};	//モーションセットメッセージ
		char KeySetMessage[128] = {};		//キーセットメッセージ
		char KeyStartMessage[128] = {};		//キースタートメッセージ
		char aNameMessage[128] = {};		//変数名のメッセージ
		char aPartsEndMessage[128] = {};	//パーツエンドメッセージ
		char aCharaEndMessage[128] = {};	//キャラエンドメッセージ
		char aEndMessage[128] = {};			//終了メッセージ
		char aType[128] = {};				//終了メッセージ
		char aBool[128] = {};				//bool変換用メッセージ


		CMotion::INFO info;
		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aStartMessage[0]);
			if (strcmp(&aStartMessage[0], "SCRIPT") == 0)
			{
				// モデル数読み込み-----------------------------------------------------
				while (1)
				{//「NUM_MODEL」を探す
					fscanf(pFile, "%s", &aModelMessage[0]);
					if (strcmp(&aModelMessage[0], "NUM_MODEL") == 0)
					{

						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &nNum);		//モデル数の設定
						SetNumModel(nNum);
						

					}
					if (strcmp(&aModelMessage[0], "NUM_MOTION") == 0)
					{

						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &nNumMotion);		//モーション数の定設
						break;
					}
				}
				
				for (int nCnt = 0; nCnt < nNum; )
				{
					fscanf(pFile, "%s", &aModelMessage[0]);
					if (strcmp(&aModelMessage[0], "MODEL_FILENAME") == 0)
					{

						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &aCharaSetMessage[nCnt][0]);		//モデル数の設定
						
					
						nCnt++;
						if (nCnt >= NUM_MODEL -1 )
						{
							break;
						}
					}
				}
				while (1)
				{
					fscanf(pFile, "%s", &aModelMessage[0]);
					if (strcmp(&aModelMessage[0], "NUM_PARTS") == 0)
					{

						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &nNumParts);		//モデル数の設定
						break;
					}
				}
				//キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す
					fscanf(pFile, "%s", &aPartsSetMessage[0]);
					if (strcmp(&aPartsSetMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aNameMessage[0]);
							if (strcmp(&aNameMessage[0], "INDEX") == 0)
							{
								int nIDX = 0;
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nIDX);	//インデックスを設定
								(*m_ppModel[nCntModel]) = CModel::Create(&aCharaSetMessage[nIDX][0]);
							}
							if (strcmp(&aNameMessage[0], "PARENT") == 0)
							{
								int nParent;
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nParent);	//親モデルのインデックスを設定
								if (nParent > -1)
								{
									(*m_ppModel[nCntModel])->SetParent(*m_ppModel[nParent]);
								}
								else
								{
									(*m_ppModel[nCntModel])->SetParent(NULL);
								}
							
							}
							if (strcmp(&aNameMessage[0], "POS") == 0)
							{
								D3DXVECTOR3 pos;
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &pos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &pos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &pos.z);				//位置(オフセット)の初期設定
								(*m_ppModel[nCntModel])->SetPos(pos);
								(*m_ppModel[nCntModel])->SetOffset(pos);
							}
							if (strcmp(&aNameMessage[0], "ROT") == 0)
							{
								D3DXVECTOR3 rot;
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &rot.x);				////向きの初期設定
								fscanf(pFile, "%f", &rot.y);				////向きの初期設定
								fscanf(pFile, "%f", &rot.z);				////向きの初期設定
								(*m_ppModel[nCntModel])->SetRot(rot);
							}
							if (strcmp(&aNameMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= nNumParts)
						{
							break;
						}
					}
				}

				// モーション情報読み込み-----------------------------------------------------
				while (1)
				{//「MOTIONSET」を探す
					fscanf(pFile, "%s", &aMotionSetMessage[0]);
					if (strcmp(&aMotionSetMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aNameMessage[0]);
							if (strcmp(&aNameMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定
								info.bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方
							}
							if (strcmp(&aNameMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &info.nNumkey);	//キーの総数を設定
								break;
							}
						}

						// キー情報読み込み-----------------------------------------------------
						while (1)
						{//「KEYSET」を探す
							fscanf(pFile, "%s", &KeySetMessage[0]);
							if (strcmp(&KeySetMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//「FRAME」を探す
									fscanf(pFile, "%s", &aNameMessage[0]);
									if (strcmp(&aNameMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &info.aKeyInfo[nCntKeySet].nFrame);	//キーフレームを設定
										break;
									}
								}

								while (1)
								{//「KEY」を探す
									fscanf(pFile, "%s", &KeyStartMessage[0]);
									if (strcmp(&KeyStartMessage[0], "KEY") == 0)
									{
										while (1)
										{//各種変数を探す
											fscanf(pFile, "%s", &aNameMessage[0]);
											if (strcmp(&aNameMessage[0], "POS") == 0)
											{

												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].pos_x);	//位置を設定
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].pos_y);	//位置を設定
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].pos_z);	//位置を設定
											}
											if (strcmp(&aNameMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].rot_x);	//位置を設定
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].rot_y);	//位置を設定
												fscanf(pFile, "%f", &info.aKeyInfo[nCntKeySet].aKey[nCntKey].rot_z);	//位置を設定
												break;
											}
										}
									}
									if (strcmp(&KeyStartMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= nNum)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&KeySetMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= info.nNumkey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMotionSetMessage[0], "END_MOTIONSET") == 0)
					{
						SetInfo(info);
						nCntKeySet = 0;
						nCntMotion++;
						if (nCntMotion >= nNumMotion)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aStartMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		//if (m_ppModel != NULL)
		//{
		//	delete[] m_ppModel;
		//	m_ppModel = NULL;
		//}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}
