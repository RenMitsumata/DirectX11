#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "CModel.h"

#include "CShadow.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "audio_clip.h"

#include <stdlib.h>
#include "Enemy.h"

int CEnemy::Cnt = 0;

CEnemy::CEnemy()
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	

	m_Model = new CModel;
	m_Model->Init("Assets/miku_01.obj",transform.position);
	m_Model->SetScale(XMFLOAT3(5.0f, 5.0f, 5.0f));
	m_Shadow = new CShadow(5.0f);
	transform.position = { -125 + ((rand() % 500) * 0.5f),0.5f,(rand() % 500) * 0.5f };
	m_Col = CManager::GetScene()->AddCollision<CColSphere>();
	m_Col->Init(&transform.position);
	m_Col->Attach(this);
	m_Shadow->Init();
	m_SE_Bomb = new CAudioClip;
	m_SE_Bomb->Load("sound/bomb.wav");
	Cnt++;
}

void CEnemy::Uninit(void)
{
	delete m_Shadow;
	delete m_Model;
	m_SE_Bomb->Play(false);
}

void CEnemy::Update(void)
{
}

void CEnemy::Draw(void)
{
	if (CManager::GetScene()->GetGameObject<CCamera>(0)->GetVisiblity(transform.position)) {
		m_Shadow->Draw(transform.position);

		m_Model->Draw(transform.position);

	}
}
