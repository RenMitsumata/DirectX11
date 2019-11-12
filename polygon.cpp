#include "main.h"


#include "renderer.h"
#include "texture.h"
#include "GameObject.h"
#include "polygon.h"

static XMMATRIX mtx;
static XMMATRIX offsetPos;

CPolygon::CPolygon()
{
}


CPolygon::~CPolygon()
{
}

void CPolygon::Init()
{
	VERTEX_3D vertex[4];
	for (int i = 0; i < 4; i++) {
		vertex[i].Position = XMFLOAT3((i % 2)*SCREEN_WIDTH, (i / 2)*SCREEN_HEIGHT, 0);
		vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i].TexCoord = XMFLOAT2(i % 2, i / 2);
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//　頂点バッファ
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	
	
	mtx = XMMatrixIdentity();
	
	

}

void CPolygon::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Unload();
	delete m_Texture;
}

void CPolygon::Update()
{
	//XMVECTOR axis = (0.0f, 0.0f, 1.0f);
	//mtx *= XMMatrixRotationZ(0.03f);
}

void CPolygon::Draw()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	CRenderer::SetTexture(m_Texture);
	CRenderer::SetWorldViewProjection2D(&mtx);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);

}

void CPolygon::Set(const char * filename)
{
	if (m_Texture != NULL) {
		m_Texture->Unload();
		delete m_Texture;
	}
	m_Texture = new CTexture;
	m_Texture->Load(filename);
}
