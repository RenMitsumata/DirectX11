#pragma once

// 子クラスの前方宣言
class CGameObject;
class CColSphere;
class CColBox;

class CCollision
{
protected:
	int colType;
	bool m_Destroy;
	XMFLOAT3* pos;
	CGameObject* owner;
public:
	CCollision();
	virtual ~CCollision();
	void Attach(CGameObject* owner) { this->owner = owner; }
	virtual void Init(XMFLOAT3* ownerPos) {
		pos = ownerPos;
		m_Destroy = false;
	};
	bool Destroy(void);
	void SetDestroy(void) { m_Destroy = true; }
	virtual void Dispatch(CCollision* other) = 0;
	virtual bool isCollision(CColSphere* other) = 0;
	virtual bool isCollision(CColBox* other) = 0;
};


