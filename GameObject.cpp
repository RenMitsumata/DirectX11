#include "main.h"
#include "renderer.h"

#include "GameObject.h"

void GameObject::Init()
{
	transform.position = XMFLOAT3(0.0f, 0.0f, 5.0f);
}

void GameObject::Uninit()
{
}

void GameObject::Update()
{
	

}

void GameObject::Draw()
{
	/*
	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	CRenderer::SetWorldMatrix(&world);
	*/
	for (Component* comp : _ComponentList) {
		comp->Draw();
	}
}
