#pragma once


class CModel;
class CBullet
{
private:
	CModel* m_Model;
	CColSphere* m_Collision;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Velocity;
	CGameObject* owner;
	int lifetime;
	bool isKilled;
public:
	CBullet();
	CBullet(CGameObject* owner,XMFLOAT3 pos, XMFLOAT3 velocity);
	void Set(CGameObject * owner, XMFLOAT3 pos, XMFLOAT3 velocity);
	virtual ~CBullet();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool isDestroy(void) { return isKilled; }
};

