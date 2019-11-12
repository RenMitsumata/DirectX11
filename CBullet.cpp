#include "main.h"
#include "renderer.h"
#include "manager.h"

#include "Scene.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "CPlayer.h"
#include "model.h"
#include "CBullet.h"


CBullet::CBullet() : isKilled(false)
{
}

CBullet::CBullet(CGameObject* owner,XMFLOAT3 pos, XMFLOAT3 velocity)
{
	this->owner = owner;
	m_Velocity = velocity;
	m_Position = pos;
}

void CBullet::Set(CGameObject* owner, XMFLOAT3 pos, XMFLOAT3 velocity)
{
	this->owner = owner;
	m_Velocity = velocity;
	m_Position = pos;
	m_Model = new CModel;
	m_Model->Init("asset/bullet.obj", m_Position);
	m_Model->SetScale(XMFLOAT3(0.4f, 0.4f, 0.4f));
}


CBullet::~CBullet()
{
}

void CBullet::Init(void)
{
	
	
	//m_Position = { 0.0f,0.0f,0.0f };

	m_Collision = CManager::GetScene()->AddCollision<CColSphere>();
	m_Collision->Init(&m_Position);
	m_Collision->SetRadius(0.8f);
	lifetime = 100;
}

void CBullet::Uninit(void)
{
	delete m_Model;
}

void CBullet::Update(void)
{
	if (isKilled) {
		delete this;
	}
	XMFLOAT3 delta;
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	m_Model->Update(m_Velocity);
	lifetime--;
	if (lifetime <= 0 || m_Collision == nullptr) {
		isKilled = true;
	}
	
}

void CBullet::Draw(void)
{
	m_Model->Draw();
}

