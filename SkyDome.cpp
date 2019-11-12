
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "texture.h"
#include "GameObject.h"
#include "SkyDome.h"



SkyDome::SkyDome()
{
	
}


SkyDome::~SkyDome()
{
	texture->Unload();
	delete texture;
	indexBuffer->Release();
	vertexBuffer->Release();
}

void SkyDome::Init(float radius)
{
	//---------------------------------------------------------
	//	Caution!
	//　スカイドームの目の細かさを変更したい場合、
	//　tおよびnの条件を変更できるようにする（引数などを追加）
	//　〈現在は、10度ごとになっている〉
	//---------------------------------------------------------

	VERTEX_3D vertex[1 + 17 * 36 + 1];
	// 頂点バッファを作る
	for (int t = 0; t <= 18; t++) {
		if (t == 0) {
			vertex[0].Position = XMFLOAT3(0.0f, radius, 0.0f);
			vertex[0].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[0].TexCoord = XMFLOAT2(0.5f, 1.0f);
		}
		else if (t == 18) {
			vertex[613].Position = XMFLOAT3(0.0f, -radius, 0.0f);
			vertex[613].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[613].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[613].TexCoord = XMFLOAT2(0.5f, 0.0f);
			break;
		}
		else {
			for (int n = 0; n < 36; n++) {
				vertex[36 * (t - 1) + n].Position = radius * XMFLOAT3(cosf(XMConvertToRadians(10 * n)), cosf(XMConvertToRadians(10 * t)), sinf(XMConvertToRadians(10 * n)));
				vertex[36 * (t - 1) + n].Normal = -1 * vertex[36 * t + n].Position / radius;
				vertex[36 * (t - 1) + n].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex[36 * (t - 1) + n].TexCoord = XMFLOAT2((float)n / 36, (float)t / 18);

			}
		}
	}
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_3D) * (1 + 17 * 36 + 1);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = vertex;
	vbData.SysMemPitch = 0;
	vbData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vbData, &vertexBuffer);


	// インデックスバッファを作る
	unsigned short index[(36 + 72 * 16 + 36) * 3];
	indexSize = (36 + 72 * 16 + 36) * 3;
	unsigned int Cnt = 0;
	for (int t = 0; t < 18; t++) {
		for (int n = 0; n < 36; n++) {
			if (t == 0) {
				index[Cnt] = 0;
				index[Cnt + 1] = n + 1;
				if (n == 35) {
					index[Cnt + 2] = 1;
				}
				else index[Cnt + 2] = n + 2;
				Cnt += 3;
			}
			else if (t == 17) {
				index[Cnt] = 1 + 16 * 36 + n;
				if (n == 35) {
					index[Cnt + 1] = 1 + 16 * 36;
				}
				else index[Cnt + 1] = 1 + 16 * 36 + n + 1;
				index[Cnt + 2] = 613;
				Cnt += 3;
			}
			else {
				index[Cnt] = 36 * t + n + 1;

				index[Cnt + 2] = 36 * (t + 1) + n + 1;

				if (n == 35) {
					index[Cnt + 1] = 36 * t + 1;
				}
				else index[Cnt + 1] = 36 * t + n + 2;


				index[Cnt + 3] = 36 * (t + 1) + n + 1;

				if (n == 35) {
					index[Cnt + 5] = 36 * t + 1;
				}
				else index[Cnt + 5] = 36 * t + n + 2;

				if (n == 35) {
					index[Cnt + 4] = 36 * (t + 1) + 1;
				}
				else index[Cnt + 4] = 36 * (t + 1) + n + 2;
				Cnt += 6;
			}
		}
	}
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ((36 + 72 * 16 + 36) * 3);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = index;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&indexBufferDesc, &ibData, &indexBuffer);

	// 画像をロードする
	texture = new CTexture;
	texture->Load("asset/space.png");
}

void SkyDome::Draw(void) {
	XMFLOAT3 playerPos = CManager::GetScene()->GetGameObject<CPlayer>(2)->GetPos();
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix *= XMMatrixTranslation(playerPos.x, playerPos.y, playerPos.z);
	CRenderer::SetWorldMatrix(&worldMatrix);
	// あらかじめワールドマトリクスを設定しておいて
	CRenderer::SetDepthEnable(false);
	CRenderer::SetVertexBuffers(vertexBuffer);
	CRenderer::SetIndexBuffer(indexBuffer);
	CRenderer::SetTexture(texture);
	CRenderer::DrawIndexed(indexSize, 0, 0);
	CRenderer::SetDepthEnable(true);
}
