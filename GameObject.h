#pragma once


class CGameObject
{	
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	bool m_Destroy = false;
public:
	CGameObject() {};
	virtual ~CGameObject() {};
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void SetDestroy(void) { m_Destroy = true; }
	bool Destroy(void);
	XMFLOAT3 GetPos(void) { return m_Position; }
};

