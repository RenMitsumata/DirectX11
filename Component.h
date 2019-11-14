////////////////////////////////////////////////////////////
//
//�@Windows DirectX11�v���O���~���O �Q�[������
//�@[Component.h]
//                                    Author:Ren Mitsumata
//==========================================================
//�@GameObject�ɐݒ肷��A�R���|�[�l���g�̊��N���X
////////////////////////////////////////////////////////////

#ifndef _COMPONENT_H
#define _COMPONENT_H
// --- �C���N���[�h�K�[�h --- //

#include <list>

// �O���錾
class GameObject;

class Component
{
protected:
	enum UpdatePriority {
		FIXED_UPDATE,
		NORMAL_UPDATE,
		LATE_UPDATE,
		UPDATEPRIORITY_MAX
	};

private:
	static std::list<Component*> _ComponentList[UPDATEPRIORITY_MAX];
protected:
	static void AddList(Component* component,UpdatePriority priority = NORMAL_UPDATE);
	GameObject* owner;
public:
	Component();
	Component(GameObject* owner);
	void SetOwner(GameObject* owner);
	virtual ~Component();
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	static void FixedUpdate();
	static void UpdateAll();
	static void LateUpdate();
};


// --- �C���N���[�h�K�[�h --- //
#endif