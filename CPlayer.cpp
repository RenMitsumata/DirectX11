
#include "main.h"
#include "renderer.h"
#include "manager.h"


#include "Scene.h"
#include "GameObject.h"
#include "CCollision.h"
#include "CColSphere.h"
#include "CBullet.h"
#include "camera.h"
#include "CModel.h"
#include "CModelAnimation.h"
#include "CShadow.h"
#include "Field.h"
#include "input.h"
#include "SkyDome.h"
#include <list>
#include "audio_clip.h"
#include "CPlayer.h"

static float myAngle;
int CurrentPose = 0;


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(void)
{
	m_Model = new CModelAnimation();
	
	m_Shadow = new CShadow(5.0f);
	m_Camera = CManager::GetScene()->AddGameObject<CCamera>(0);
	m_Position = { 0.0f,0.5f,0.0f };
	const XMFLOAT3 startFront = XMFLOAT3(0.0f, 0.0f, 1.0f);
	front = XMLoadFloat3(&startFront);
	const XMFLOAT3 startUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	up = XMLoadFloat3(&startUp);
	//m_Model->Init();
	m_Model->Load("asset/coaster.fbx");
	//m_ModelHuman->Load("asset/Human.fbx");
	for (int i = 0; i < 3; i++) {
		m_ModelHuman[i] = new CModelAnimation();
	}
	m_ModelHuman[0]->Load("asset/yakiu.fbx");
	m_ModelHuman[1]->Load("asset/Jumping.fbx");
	m_ModelHuman[2]->Load("asset/Walking.fbx");
	m_Shadow->Init(); 
	skydome = CManager::GetScene()->AddGameObject<SkyDome>(1);
	skydome->Init(50.0f);
	//playerUI = CManager::GetScene()->AddGameObject<CPolygon>(4);
	distance = 0.0f;
	m_SE_Shoot = new CAudioClip;
	m_SE_Shoot->Load("sound/shoot.wav");
	myAngle = 0.0f;
}

void CPlayer::Uninit(void)
{
	delete m_SE_Shoot;
	m_Shadow->Uninit();	
	delete m_Shadow;
	for (int i = 0; i < 3; i++) {
		m_ModelHuman[i]->UnLoad();
		delete m_ModelHuman[i];
	}	
	m_Model->UnLoad();
	//delete[] m_ModelHuman;
	delete m_Model;
}

void CPlayer::Update(void)
{
	//m_Model->Update();
	/*
	for (int i = 0; i < 3; i++) {
		m_ModelHuman[i]->Update(distance);
	}
	*/
	m_ModelHuman[CurrentPose]->Update(distance);
	XMFLOAT3 frontPos;
	XMFLOAT3 upPos;
	XMStoreFloat3(&frontPos, front);
	XMStoreFloat3(&upPos, up);
	frontPos = frontPos * (-6.0f) + upPos * 2.0f;
	frontPos = m_Position + frontPos;
	
	XMVECTOR curFront = pCource->GetTild(distance);
	
	if (distance > 0.25f) {
		
		XMFLOAT3 YPR = pCource->GetPitchYawRoll(distance);


		XMVECTOR curUp = { 0.0f,1.0f,0.0f };
		XMMATRIX tild = XMMatrixIdentity();
		tild *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
		XMVector3TransformNormal(curUp, tild);
		XMStoreFloat3(&frontPos, curFront * (-6.0f) + curUp * 2.0f);
		frontPos = m_Position + frontPos;
		m_Camera->Update(frontPos, curFront, curUp);
	}

	else m_Camera->Update(frontPos, front, up);


	if (CInput::GetKeyPress('W') && m_Rotation.y < 0.3f) {
		m_Rotation.y += 0.05f;
	}

	if (CInput::GetKeyPress('S') && m_Rotation.y > -0.3f) {
		m_Rotation.y -= 0.05f;
	}	
	
	if (CInput::GetKeyPress('A') && m_Rotation.x > -0.95f) {
		m_Rotation.x -= 0.05f;
	}
	
	if (CInput::GetKeyPress('D') && m_Rotation.x < 0.95f) {
		m_Rotation.x += 0.05f;
	}
	
	if (CInput::GetKeyPress('P')) {
		CurrentPose = 0;
	}

	if (CInput::GetKeyPress('L')) {
		CurrentPose = 1;
	}

	if (CInput::GetKeyPress('O')) {
		CurrentPose = 2;
	}

	
	distance += 0.25f;
	pCource = CManager::GetScene()->GetGameObject<Cource>(1);
	m_Position = pCource->GetCource(distance);
	XMFLOAT3 upFor;
	XMStoreFloat3(&upFor,up);
	m_Position.x += upFor.x;
	m_Position.y += upFor.y;
	m_Position.z += upFor.z;
	
	
	XMMATRIX curMat = XMMatrixIdentity();
	curMat = XMMatrixRotationAxis(up,m_Rotation.x);
	XMVECTOR right = XMVector3Cross(front,up);
	curMat *= XMMatrixRotationAxis(right, m_Rotation.y);

	curFront = XMVector3TransformNormal(curFront, curMat);
	front = curFront;
	
	for (CBullet* bullet : _bulletList) {
		bullet->Update();
	}


	if (CInput::GetKeyTrigger(VK_SPACE)) {
		XMFLOAT3 velocity;
		XMStoreFloat3(&velocity, front);
		// ‹@‘Ì‚Ì³–Ê‚ðŽ‚Á‚Ä‚«‚½‚¢
		XMVECTOR vectorFront = { 0.0f,0.0f,1.0f };
		XMMATRIX matFront = XMMatrixIdentity();
		XMFLOAT3 YPR = pCource->GetPitchYawRoll(distance);
		matFront *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
		XMVector3TransformNormal(vectorFront, matFront);


		XMFLOAT3 calcPos;
		XMStoreFloat3(&calcPos, vectorFront + front * 2.5f + up * 1.3f);
		CBullet* bullet = new CBullet;
		bullet->Init();
		bullet->Set(this, m_Position + calcPos, velocity * 2.0f);
		_bulletList.push_back(bullet);
		//CBullet* bullet = new CBullet(this, m_Position + calcPos, velocity * 2.0f);	// m_Position‚É‘å–C‚Ìƒtƒƒ“ƒg‚ð‘«‚·
		m_SE_Shoot->Play(false);

	}
	_bulletList.remove_if([](CBullet* bullet) {
		return bullet->isDestroy();
	});

	//front = pCource->GetTild(distance);
	//float newPosY = CField::GetHeight(m_Position);
	//XMFLOAT4 newPosY = CField::GetNormal(&m_Position);
	//m_Position.y += 0.5f;
	
	myAngle += 0.05f;
}

void CPlayer::Draw(void)
{
	m_Shadow->Draw(m_Position);
	
	
	if (pCource == NULL) {
		pCource = CManager::GetScene()->GetGameObject<Cource>(1);
	}
	/*
	XMFLOAT3 buffer;
	XMStoreFloat3(&buffer,front);
	*/
	
	for (CBullet* bullet : _bulletList) {
		bullet->Draw();
	}


	XMFLOAT3 curRotate;
	XMStoreFloat3(&curRotate, front);

	XMMATRIX transform = XMMatrixIdentity();	
	transform *= XMMatrixRotationNormal(up, m_Rotation.x);
	XMFLOAT3 YPR = pCource->GetPitchYawRoll(distance);
	
	
	XMMATRIX mat = XMMatrixIdentity();
	mat *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
	mat *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	mat *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMMATRIX humMat = XMMatrixIdentity();
	//humMat *= XMMatrixRotationRollPitchYaw(YPR.x, YPR.y, YPR.z);
	//humMat *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	humMat *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);


	//CRenderer::SetWorldMatrix(&mat);

	//m_ModelHuman[CurrentPose]->Draw(humMat, 0.0f, 0.0f);
	
	//m_Model->Draw(mat, m_Rotation.x,m_Rotation.y);
	
	
	
}

