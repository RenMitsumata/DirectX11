#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "CTitle.h"
#include "CResult.h"
#include "CFade.h"
#include "CGame.h"
#include "audio_clip.h"
#include "manager.h"
#include "Scene.h"

CScene* CManager::m_Scene = nullptr;
static CInput* g_Input;

void CManager::Init()
{

	CRenderer::Init();
	CAudioClip::Init();
	SetScene<CTitle>();
	g_Input = new CInput;
	g_Input->Init();
	CFade::Init();
}

void CManager::Uninit()
{
	CFade::Uninit();
	g_Input->Uninit();
	delete g_Input;
	m_Scene->Uninit();
	delete m_Scene;
	CAudioClip::Uninit();
	CRenderer::Uninit();
	
}

void CManager::Update()
{
	m_Scene->Update();
	g_Input->Update();
	
	CFade::Update();

}

void CManager::Draw()
{

	CRenderer::Begin();
	m_Scene->Draw();
	CFade::Draw();
	CRenderer::End();

}

CScene * CManager::GetScene()
{
	return m_Scene;
}
