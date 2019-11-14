#include "main.h"
#include "renderer.h"

#include "texture.h"

#include "CShadow.h"

VERTEX_3D vertex[4];

CShadow::CShadow()
{
	size = 1.0f;
}

CShadow::CShadow(float radius)
{
	size = radius;
}


CShadow::~CShadow()
{
}

void CShadow::Init(void)
{
	m_Texture = new CTexture;
	m_Texture->Load("Assets/circle_shadow.tga");
	for (int i = 0; i < 4; i++) {
		vertex[i].Position.x = -(size / 2) + (i % 2) * size;
		vertex[i].Position.y = 0.1f;
		vertex[i].Position.z = (size / 2) - (i / 2) * size;
		vertex[i].TexCoord.x = (i % 2);
		vertex[i].TexCoord.y = (i / 2);
		vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_Texture = new CTexture;
	m_Texture->Load("Assets/circle_shadow.tga");

}


void CShadow::Uninit(void)
{
	m_Texture->Unload();
	delete m_Texture;
}


void CShadow::Draw(void)
{

}

void CShadow::Draw(XMFLOAT3 pos)
{
	/*
	XMMATRIX worldMtx;
	worldMtx = XMMatrixIdentity();
	worldMtx *= XMMatrixTranslation(pos.x, 0.1f, pos.z);
	CRenderer::SetWorldMatrix(&worldMtx);
	*/



	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);

}
