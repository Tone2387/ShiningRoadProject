#include "Object.h"

D3DXVECTOR3 GetVec3Dir(const float Angle, const D3DXVECTOR3 vAxis)
{
	D3DXMATRIX mYaw;

	//回転.
	D3DXMatrixRotationY(&mYaw, Angle);//Y軸回転.

	D3DXVECTOR3 vDir;

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(&vDir, &vAxis, &mYaw);

	D3DXVec3Normalize(&vDir, &vDir);//現在向いている方向ベクトル.

	return vDir;
}

//ﾚｲとﾒｯｼｭの当たり判定.
bool clsObject::Intersect(
	const RAYSTATE RayState,
	const clsDX9Mesh* pTarget,//対象の物体.
	float* pfDistance, //(out)距離.
	D3DXVECTOR3* pvIntersect)//(out)交差座標.
{
	D3DXMATRIXA16 matRot;//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, m_Trans.fYaw);

	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxis = RayState.vAxis;
	//Z軸ﾍﾞｸﾄﾙそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(&vecAxis, &vecAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = RayState.vRayStart;
	//Attackerの座標に回転行列を合成する.
	vecEnd += vecAxis * 1.0f;

	//対象が動いてる物体でも、対象のworld行列の、逆並列を用いれば正しくﾚｲが当たる.
	D3DXMATRIX matWorld,mTrans,mRotate,mScale;
	D3DXMatrixTranslation(&mTrans,
		pTarget->m_Trans.vPos.x,
		pTarget->m_Trans.vPos.y,
		pTarget->m_Trans.vPos.z);

	D3DXMatrixRotationYawPitchRoll(&mRotate,
		pTarget->m_Trans.fYaw,
		pTarget->m_Trans.fPitch,
		pTarget->m_Trans.fRoll);

	D3DXMatrixScaling(&mScale,
		pTarget->m_Trans.vScale.x,
		pTarget->m_Trans.vScale.y,
		pTarget->m_Trans.vScale.z);

	matWorld = mTrans * mRotate * mScale;

	//逆並列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;

	DWORD dwIndex = 0;//ｲﾝﾃﾞｯｸｽ番号.
	D3DXVECTOR3 vVertex[3];//頂点座標.
	FLOAT U = 0, V = 0;//(out)重心ﾋｯﾄ座標.

	//ﾒｯｼｭとﾚｲの交差を調べる.
	D3DXIntersect(
		pTarget->m_pMesh,//対象ﾒｯｼｭ.
		&vecStart,//開始位置.
		&vecDistance,//ﾚｲの距離.
		&bHit,//(out)判定結果.
		&dwIndex,//(out)bHitがTrueの時、ﾚｲの始点に。最も近くの面のｲﾝﾃﾞｯｸｽ値へのﾎﾟｲﾝﾀ.
		&U, &V,//(out)重心ﾋｯﾄ座標.
		pfDistance,//ﾀｰｹﾞｯﾄとの距離.
		NULL, NULL);
	if (bHit)
	{
		//命中したとき.
		FindVecticesOnPoly(pTarget->m_pMeshForRay, dwIndex, vVertex);

		//中心座標から交差点を算出.
		//ﾛｰｶﾙ交点pは、v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		pvIntersect->x *= pTarget->m_Trans.vScale.x;
		pvIntersect->y *= pTarget->m_Trans.vScale.y;
		pvIntersect->z *= pTarget->m_Trans.vScale.z;

		return true;//命中している.
	}

	return false;//命中していない.
}

//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
//※頂点座標はﾛｰｶﾙ座標.
HRESULT clsObject::FindVecticesOnPoly(
	const LPD3DXMESH pTarget,
	const DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//頂点ごとのﾊﾞｲﾄ数を取得.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAm = pTarget->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをﾛｯｸ(読み込みﾓｰﾄﾞ)
	pTarget->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;//頂点(ﾊﾞｲﾄ数)
	FLOAT* pfVertices = NULL;//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;//頂点ﾊﾞｯﾌｧ.
	pTarget->GetVertexBuffer(&VB);//頂点情報の取得.

	//頂点ﾊﾞｯﾌｧのﾛｯｸ.
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		/*for (int i = 0; i < 3; i++)
		{
		//ﾎﾟﾘｺﾞﾝの頂点のiつ目を取得.
		pfVertices
		= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + i]];
		pVecVertices[i].x = pfVertices[0];
		pVecVertices[i].y = pfVertices[1];
		pVecVertices[i].z = pfVertices[2];
		}*/

		//ﾎﾟﾘｺﾞﾝの頂点の1つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//ﾎﾟﾘｺﾞﾝの頂点の2つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//ﾎﾟﾘｺﾞﾝの頂点の3つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		pTarget->UnlockIndexBuffer();//ﾛｯｸ解除.
		VB->Unlock();//ﾛｯｸ解除.
	}
	VB->Release();

	return S_OK;
}

//回転値調整.
void ObjRollOverGuard(float* fRot)
{
	if (*fRot > D3DX_PI * 2.0f)
	{
		//1周以上している.
		*fRot -= D3DX_PI * 2.0f;//2π(360°)分を引く.

		//再帰関数.
		if (*fRot > D3DX_PI*2.0f)
		{
			ObjRollOverGuard(fRot);
		}
	}
}

bool clsObject::WallSetAxis(const clsDX9Mesh* pWall, float* fResultDis, const D3DXVECTOR3 vRayDir)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fMoveSpeed;
	RAYSTATE rs;
	rs.vAxis = vRayDir;
	rs.vRayStart = m_Trans.vPos;

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	//前が壁に接近.
	if (fDis < fRaySpece && fDis > g_fGroundSpece)
	{
		bResult = true;
	}

	return bResult;
}
bool clsObject::WallForward(const clsDX9Mesh* pWall, const bool bSlip)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis, fYaw;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fMoveSpeed;
	RAYSTATE rs;
	rs.vAxis = g_vDirForward;
	rs.vRayStart = m_Trans.vPos;
	//ﾚｲの向きによる当たる壁までの距離を求める.

	fYaw = fabs(m_Trans.fYaw);//fabs:絶対値(float版)
	ObjRollOverGuard(&fYaw);//0～2πの間に収める.

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;
	
	//前が壁に接近.
	if (fDis < fRaySpece && fDis > g_fGroundSpece)
	{
		bResult = true;

		if (bSlip)
		{
			if (m_Trans.fYaw < 0.0f)
			{
				//時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}
			}

			else
			{
				//反時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}
			}
		}
	}

	return bResult;
}

bool clsObject::WallBack(const clsDX9Mesh* pWall, const bool bSlip)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis, fYaw;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fMoveSpeed;
	RAYSTATE rs;
	rs.vAxis = g_vDirBack;
	rs.vRayStart = m_Trans.vPos;
	//ﾚｲの向きによる当たる壁までの距離を求める.

	fYaw = fabs(m_Trans.fYaw);//fabs:絶対値(float版)
	ObjRollOverGuard(&fYaw);//0～2πの間に収める.

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	if (fDis < fRaySpece && fDis > g_fGroundSpece)
	{
		bResult = true;

		if (bSlip)
		{
			if (m_Trans.fYaw < 0.0f)
			{
				//時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}
			}

			else
			{
				//反時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}
			}
		}
	}

	return bResult;
}

bool clsObject::WallLeft(const clsDX9Mesh* pWall, const bool bSlip)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis, fYaw;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fMoveSpeed;
	RAYSTATE rs;
	rs.vAxis = g_vDirLeft;
	rs.vRayStart = m_Trans.vPos;
	//ﾚｲの向きによる当たる壁までの距離を求める.

	fYaw = fabs(m_Trans.fYaw);//fabs:絶対値(float版)
	ObjRollOverGuard(&fYaw);//0～2πの間に収める.

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	if (fDis < fRaySpece && fDis > g_fGroundSpece)
	{
		bResult = true;

		if (bSlip)
		{
			if (m_Trans.fYaw < 0.0f)
			{
				//時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}
			}

			else
			{
				//反時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

			}
		}
	}

	return bResult;
}

bool clsObject::WallRight(const clsDX9Mesh* pWall, const bool bSlip)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis, fYaw;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fMoveSpeed;
	RAYSTATE rs;
	rs.vAxis = g_vDirRight;
	rs.vRayStart = m_Trans.vPos;
	//ﾚｲの向きによる当たる壁までの距離を求める.

	fYaw = fabs(m_Trans.fYaw);//fabs:絶対値(float版)
	ObjRollOverGuard(&fYaw);//0～2πの間に収める.

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	if (fDis < fRaySpece && fDis > g_fGroundSpece)
	{
		bResult = true;

		if (bSlip)
		{
			if (m_Trans.fYaw < 0.0f)
			{
				//時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}
			}

			else
			{
				//反時計回り
				if (fYaw >= 0.785f && fYaw < 2.355f)//右から.
				{
					m_Trans.vPos.z += fRaySpece - fDis;
				}

				else if (fYaw >= 2.355f && fYaw < 3.925f)//前から.
				{
					m_Trans.vPos.x += fRaySpece - fDis;
				}

				else if (fYaw >= 3.925f && fYaw < 5.495f)//左から
				{
					m_Trans.vPos.z -= fRaySpece - fDis;
				}

				else//奥から.
				{
					m_Trans.vPos.x -= fRaySpece - fDis;
				}
			}
		}
	}

	return bResult;
}

bool clsObject::WallUp(const clsDX9Mesh* pWall)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis;//距離と回転.
	float fRaySpece = g_fRaySpace + m_fFollPower;
	RAYSTATE rs;
	rs.vAxis = g_vDirUp;
	rs.vRayStart = m_Trans.vPos;

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	if (fDis < fRaySpece + m_fFollPower && fDis > g_fGroundSpece)
	{
		bResult = true;

		if (m_fFollPower > 0.0f)
		{
			m_fFollPower = 0.0f;
			m_Trans.vPos.y = vIntersect.y - fRaySpece;
		}
	}

	return bResult;
}

bool clsObject::WallUnder(const clsDX9Mesh* pWall)
{
	FLOAT fDistance;//距離.
	D3DXVECTOR3 vIntersect;//交点座標.
	float fDis;//距離と回転.
	float fRaySpece = g_fRaySpace - m_fFollPower;
	RAYSTATE rs;
	rs.vAxis = g_vDirDown;
	rs.vRayStart = m_Trans.vPos;

	Intersect(rs, pWall, &fDis, &vIntersect);

	bool bResult = false;

	if (fDis < fRaySpece - m_fFollPower && fDis > g_fGroundSpece)
	{
		bResult = true;

		m_Trans.vPos.y = vIntersect.y + (fRaySpece - g_fGroundSpece);

		m_fFollPower = 0.0f;
	}

	return bResult;
}

bool clsObject::Collision(SPHERE pAttacker, SPHERE pTarget)
{
	//2つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength = *pTarget.vCenter - *pAttacker.vCenter;
	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//2物体間の距離が、2物体の半径を足したもの.
	//小さいということは、ｽﾌｨｱ同士が重なっている.
	//(衝突している)ということ.
	if (Length <=
		pAttacker.fRadius + pTarget.fRadius)
	{
		return true;//衝突.
	}
	return false;//衝突していない.
}

bool clsObject::ObjectCollision(std::vector<SPHERE> pTarget, const int iNumMax)
{
	for (int i = 0; i < m_iColSpheresMax; i++)
	{
		for (int j = 0; j < iNumMax; j++)
		{
			if (Collision(m_v_pSpheres[i], pTarget[j]))
			{
				return true;
			}
		}
	}

	return false;
}

void clsObject::FreeFoll()
{
	if (!m_bGround)
	{
		m_fFollPower -= g_fGravity;
		m_Trans.vPos.y += m_fFollPower;
	}
}

bool clsObject::WallJudge(clsStage* const pStage)
{
	if (!pStage)return false;

	std::vector<clsDX9Mesh*> vvpMeshTmp;
	vvpMeshTmp = pStage->GetStageMeshArray();

	m_bGround = false;

	bool bResult = false;

	for (int i = 0; i < vvpMeshTmp.size(); i++)
	{
		clsDX9Mesh* pObjMesh = vvpMeshTmp[i];

		if (!pObjMesh)continue;

		bool bHit = false;

		//StageObjectとの当たり判定.
		if (WallForward(pObjMesh))if (!bHit)bHit = true;
		if (WallBack(pObjMesh))if (!bHit)bHit = true;
		if (WallLeft(pObjMesh))if (!bHit)bHit = true;
		if (WallRight(pObjMesh))if (!bHit)bHit = true;
		if (WallUp(pObjMesh))if (!bHit)bHit = true;

		if (WallUnder(pObjMesh))
		{
			m_bGround = true;
			if (!bHit)bHit = true;

			D3DXVECTOR3 vMovePos = GetPosition();
			D3DXVECTOR3 vMoveDir = { 0.0f, 0.0f, 0.0f };

			vMovePos += vMoveDir;
			SetPosition(vMovePos);
		}

		if (!bResult)
		{
			bResult = bHit;
		}
	}

	FreeFoll();

	return bResult;
}