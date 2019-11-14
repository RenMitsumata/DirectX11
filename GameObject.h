#pragma once

#include "Component.h"

#include <vector>


struct TRANSFORM {
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 front = { 0.0f,0.0f,1.0f };
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
};

class GameObject
{	
protected:
	TRANSFORM transform;	// �g�����X�t�H�[��(�K�v)
	XMVECTOR velocity;		// ���x
	std::vector<Component*> _ComponentList;	// �R���|�[�l���g�i�x�N�^�[�^�j
	bool isDestroy;
public:
	template<typename C>
	C* AddComponent() {
		C* comp = new C;
		comp->Init();
		_ComponentList.push_back(comp);
		return comp;
	}


	GameObject() : isDestroy(false) {}
	virtual ~GameObject() {}
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	void SetDestroy() { isDestroy = true; }
	bool GetDestroy() { return isDestroy; }
	TRANSFORM* GetTransform() { return &transform; }
	void AddVelocity(XMVECTOR force) { velocity += force; }
};

