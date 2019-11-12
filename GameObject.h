#pragma once


struct TRANSFORM {
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 front = { 0.0f,0.0f,1.0f };
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
};

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

