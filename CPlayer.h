#pragma once


class CModelAnimation;
class CShadow;
class CCamera;
class CBullet;
class Cource;
class SkyDome;
class CPolygon;
class CAudioClip;


class CPlayer :	public CGameObject
{
private:
	CModelAnimation* m_Model = NULL;
	CModelAnimation* m_ModelHuman[3];
	CShadow* m_Shadow = NULL;
	CCamera* m_Camera = NULL;
	Cource* pCource = NULL;
	SkyDome* skydome = NULL;
	CPolygon* playerUI = NULL;
	XMVECTOR front;
	XMVECTOR up;
	float distance;
	CAudioClip* m_SE_Shoot;
	std::list<CBullet*> _bulletList;
public:
	CPlayer();
	virtual ~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

