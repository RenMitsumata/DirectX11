#include "main.h"


#include "renderer.h"
#include "texture.h"
#include "CFade.h"

static XMMATRIX mtx;
static XMMATRIX offsetPos;
#define DELTA (0.015625f)

ID3D11Buffer* CFade::m_VertexBuffer = NULL;
CTexture* CFade::texture = NULL;
bool CFade::isFade; // フェード中？
bool CFade::isIn;	// フェードイン？フェードアウト？(トグルスイッチ)
float CFade::degree;
CFade::CFade()
{
}


CFade::~CFade()
{
}

void CFade::Init()
{
	degree = 1.0f;
	isIn = false;
	VERTEX_3D vertex[4];
	for (int i = 0; i < 4; i++) {
		vertex[i].Position = XMFLOAT3((i % 2)*SCREEN_WIDTH, (i / 2)*SCREEN_HEIGHT, 0);
		vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[i].Diffuse = XMFLOAT4(degree, degree, degree, 0);
		vertex[i].TexCoord = XMFLOAT2(i % 2, i / 2);
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//　頂点バッファ
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	texture = new CTexture;
	texture->Load("asset/WhiteBack.png");
	mtx = XMMatrixIdentity();



}

void CFade::Uninit()
{
	delete texture;
	m_VertexBuffer->Release();
}

void CFade::Update()
{
	if (isFade) {
		if (isIn) {
			degree += DELTA;
			if (degree == 1.0f) {
				isFade = false;
				isIn = false;
			}
		}
		else {
			degree -= DELTA;
			if (degree == 0.0f) {
				isFade = false;
				isIn = true;
			}
		}
		
		VERTEX_3D vertex[4];
		for (int i = 0; i < 4; i++) {
			vertex[i].Position = XMFLOAT3((i % 2)*SCREEN_WIDTH, (i / 2)*SCREEN_HEIGHT, 0);
			vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(degree,degree,degree,1-degree);
			vertex[i].TexCoord = XMFLOAT2(i % 2, i / 2);
		}
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, vertex, sizeof(VERTEX_3D) * 4);
		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
		
	}
	
}

void CFade::Draw()
{
	CRenderer::SetDepthEnable(false);
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;



	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	CRenderer::SetWorldViewProjection2D(&mtx);
	CRenderer::SetTexture(texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);
	//CRenderer::SetDepthEnable(true);
}

void CFade::Set(void)
{
	if (isFade) {
		return;
	}
	isFade = true;	
}

bool CFade::Get(void)
{
	return isFade;
}


