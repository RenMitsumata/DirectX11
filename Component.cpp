////////////////////////////////////////////////////////////
//
//�@Windows DirectX11�v���O���~���O �Q�[������
//�@[Component.cpp]
//                                    Author:Ren Mitsumata
//==========================================================
//�@GameObject�ɐݒ肷��A�R���|�[�l���g�̊��N���X
////////////////////////////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "Component.h"

// �ÓI�����o�̍Đ錾
std::list<Component*> Component::_ComponentList[UPDATEPRIORITY_MAX];


Component::Component()
{
	AddList(this);
}

Component::Component(GameObject* owner)
{
	this->owner = owner;
	AddList(this);
}

Component::~Component()
{
}

void Component::SetOwner(GameObject* owner)
{
	this->owner = owner;
}

void Component::AddList(Component* component, UpdatePriority priority) 
{
	_ComponentList[priority].push_back(component);
}

void Component::FixedUpdate() 
{
	for (Component* component : _ComponentList[FIXED_UPDATE]) {
		component->Update();
	}
}

void Component::UpdateAll() 
{
	for (Component* component : _ComponentList[NORMAL_UPDATE]) {
		component->Update();
	}
}

void Component::LateUpdate()
{
	for (Component* component : _ComponentList[LATE_UPDATE]) {
		component->Update();
	}
}