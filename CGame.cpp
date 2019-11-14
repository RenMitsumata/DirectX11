#include "manager.h"
#include "CFade.h"
#include "Scene.h"
#include "input.h"
#include "CResult.h"
#include "audio_clip.h"
#include "CGame.h"


CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::Init(void)
{

	
}

void CGame::Uninit(void)
{
	CScene::Uninit();
	
}

void CGame::Update(void)
{
	CScene::Update();
	
}

