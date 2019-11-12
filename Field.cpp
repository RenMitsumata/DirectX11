#include "main.h"

#include "renderer.h"
#include "texture.h"
#include "GameObject.h"
#include <time.h>

#include "Field.h"

static XMMATRIX mtx;
static XMMATRIX offsetPos;
static XMFLOAT3** vertexPos;
static ID3D11Buffer* index;
static int g_Num;
VERTEX_3D* CField::vertex;
XMFLOAT3 AddXMFloat3(XMFLOAT3 a, XMFLOAT3 b);
XMFLOAT3 DecXMFloat3(XMFLOAT3 a, XMFLOAT3 b);
XMFLOAT3 ScalXMFloat3(XMFLOAT3 a, float b);
#define SQUARE_NUMBER (100)
#define SQUARE_SIZE (1.0f)

XMVECTOR faceQuadNormal[SQUARE_NUMBER * SQUARE_NUMBER];

CField::CField()
{
}


CField::~CField()
{
}

void CField::Init()
{
	//　頂点バッファ
	const int n = SQUARE_NUMBER;
	static const int vertexNum = 2 * (n + 1) * n + 2 * (n - 1);
	g_Num = vertexNum;
	m_Position = XMFLOAT3{ 0.0f,0.0f,0.0f };
	vertex = new VERTEX_3D[vertexNum];
	int count = 0;
	
	srand(time(NULL));


	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (2 * (n + 1)); j++,count++) {
			vertex[count].Position.x = m_Position.x - (n * 0.5f - (j % 2) - i) * SQUARE_SIZE;
			//float r = 0.05f * (i % 10);
			float r = sinf(i) * 0.5f + cosf(j) * 0.3f;
			//float r = 0.05f * ((rand() % 10) + 1);
			if ((i != 0) && !(count % 2)) {
				vertex[count].Position.y = vertex[count - (2 * (n + 1)) - 1].Position.y;
			}
			else {
				vertex[count].Position.y = m_Position.y + r;
			}
			vertex[count].Position.z = m_Position.z + (n * 0.5f - (j / 2)) * SQUARE_SIZE;
			
			
			vertex[count].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


			vertex[count].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[count].TexCoord = { (float)((j % 2) + i),float(j / 2)};
		}		
		if (i != (n - 1)) {
			vertex[count] = vertex[count - 1];
			count++;
			vertex[count] = vertex[count - 2 * (n + 1)];
			count++;
		}


	}

	/*
	// === 頂点の法線ベクトルを求める ===
	// まずはすべての面の法線ベクトルを求める
	const int faceNum = SQUARE_NUMBER * SQUARE_NUMBER * 2;
	XMVECTOR faceNormal[faceNum];
	XMVECTOR bufVector[2];
	int c = 0;
	for (int j = 0; j < SQUARE_NUMBER; j++) {
		for (int i = 0; i < 2 * SQUARE_NUMBER; i++) {
			bufVector[0] = (DirectX::XMLoadFloat3(&vertex[i + 1 + (j + 2) * 2].Position) - DirectX::XMLoadFloat3(&vertex[i + (j + 2) * 2].Position));
			bufVector[1] = (DirectX::XMLoadFloat3(&vertex[i + 2 + (j + 2) * 2].Position) - DirectX::XMLoadFloat3(&vertex[i + 1 + (j + 2) * 2].Position));
			bufVector[0] = XMVector3Normalize(bufVector[0]);
			bufVector[1] = XMVector3Normalize(bufVector[1]);
			faceNormal[c] = XMVector3Cross(bufVector[0], bufVector[1]);
			c++;
		}
	}

	// 次に、その点が接する全ての面の法線の平均を代入する
	
	for (int i = 0; i < SQUARE_NUMBER * SQUARE_NUMBER; i++) {
		XMVECTOR buf = faceNormal[i * 2] + faceNormal[i * 2 + 1];
		buf = XMVector3Normalize(buf);
		faceQuadNormal[i] = buf;
	}
	count = 0;
	// 1周目の偶数は左がない
	// 各周１、２個目は上がない->頂点の0、1番目
	// 各周最後１、２個目は下がない->頂点の2*NUM、2*NUM+1番目
	// 最後の周の奇数番目は右がない
	XMFLOAT3 initVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (2 * (n + 1)); j++, count++) {
			XMVECTOR calcVec = DirectX::XMLoadFloat3(&initVec);
			if ((i != 0) && (j > 1)) {
				// 左上のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2) - 1];
				}
				else {
					calcVec += faceQuadNormal[(i - 1) * SQUARE_NUMBER + (j / 2) - 1];
				}
			}
			if ((i != (n - 1)) && (j > 1)) {
				// 右上のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[(i + 1) * SQUARE_NUMBER + (j / 2) - 1];
				}
				else {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2) - 1];
				}
			}
			if ((i != 0) && (j < (2 * (n + 1)) - 2)) {
				// 左下のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2)];
				}
				else {
					calcVec += faceQuadNormal[(i - 1) * SQUARE_NUMBER + (j / 2)];
				}
			}
			if ((i != (n - 1)) && (j < (2 * (n + 1)) - 2)) {
				// 右下のポリゴンを足す
				if (count / 2) {
					calcVec += faceQuadNormal[(i + 1) * SQUARE_NUMBER + (j / 2)];
				}
				else {
					calcVec += faceQuadNormal[i * SQUARE_NUMBER + (j / 2)];
				}
			}
			calcVec = XMVector3Normalize(calcVec);
			DirectX::XMStoreFloat3(&vertex[count].Normal, calcVec);
		}
		if (i != (n - 1)) {
			vertex[count].Normal = vertex[count - 1].Normal;
			count++;
			vertex[count].Normal = vertex[count - 2 * (n + 1)].Normal;
			count++;
		}
	}
	*/
	


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//　頂点バッファ
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	/*
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertex, (sizeof(VERTEX_3D) * vertexNum));
	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	*/
	//　インデックスバッファ
	WORD index[vertexNum];
	for (int i = 0; i < vertexNum; i++) {
		index[i] = i;
	}
	D3D11_BUFFER_DESC bd_vb;
	ZeroMemory(&bd_vb, sizeof(bd_vb));
	bd_vb.Usage = D3D11_USAGE_DEFAULT;
	bd_vb.ByteWidth = sizeof(WORD) * vertexNum;
	bd_vb.BindFlags = D3D11_BIND_INDEX_BUFFER;	//　インデックスバッファ
	bd_vb.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd_vb;
	ZeroMemory(&sd_vb, sizeof(sd_vb));
	sd_vb.pSysMem = index;
	CRenderer::GetDevice()->CreateBuffer(&bd_vb, &sd_vb, &m_IndexBuffer);
	
	




	
	m_Texture = new CTexture;
	m_Texture->Load("asset/wall.tga");
	
	m_WallTexture = new CTexture;
	m_WallTexture->Load("asset/wall.tga");
	
	


}

void CField::Uninit()
{
	m_VertexBuffer->Release();
	m_WallTexture->Unload();
	m_Texture->Unload();
	delete m_Texture;
	delete[] vertex;
}

void CField::Update()
{
	
}

void CField::Draw()
{
	XMMATRIX worldMtx;
	worldMtx = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMtx *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	worldMtx *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	CRenderer::SetWorldMatrix(&worldMtx);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;


	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed((g_Num), 0,0);

	/*
	worldMtx *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f);
	worldMtx *= XMMatrixTranslation(0.0f, 0.0f, (SQUARE_SIZE * SQUARE_NUMBER) / 2);
	CRenderer::SetWorldMatrix(&worldMtx);
	CRenderer::SetTexture(m_WallTexture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw((g_Num), 0);
	for (int i = 0; i < 3; i++) {
		//worldMtx *= XMMatrixTranslation(0.0f, 0.0f, (SQUARE_SIZE * SQUARE_NUMBER) / 2);
		worldMtx *= XMMatrixRotationY(XMConvertToRadians(90.0f));		
		CRenderer::SetWorldMatrix(&worldMtx);
		CRenderer::SetTexture(m_WallTexture);
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CRenderer::GetDeviceContext()->Draw((g_Num), 0);
	}
	*/
}

float CField::GetHeight(XMFLOAT3 pos) {
	int x, z;
	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va, vb;
	float dp0n, dvn, dpn, t;
	v = XMFLOAT3(0.0f, -10.0f, 0.0f);

	x = (pos.x + (SQUARE_SIZE * SQUARE_NUMBER / 2)) / SQUARE_SIZE;
	z = (-pos.z + (SQUARE_SIZE * SQUARE_NUMBER / 2)) / SQUARE_SIZE;
	va.x = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 2)].Position.x - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.x;
	va.y = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 2)].Position.y - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.y;
	va.z = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 2)].Position.z - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.z;
	vb.x = pos.x - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.x;
	vb.y = pos.y - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.y;
	vb.z = pos.z - vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position.z;
	if (va.x * vb.z - vb.x * va.z >= 0.0f) {
		// 左上
		p0 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 0)].Position;
		p1 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position;
		p2 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 2)].Position;
	}
	else {
		// 右下
		p0 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 3)].Position;
		p1 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 2)].Position;
		p2 = vertex[x * (SQUARE_NUMBER * 2 + 4) + (2 * z + 1)].Position;
	}
	v01 = DecXMFloat3(p1 ,p0);
	v02 = DecXMFloat3(p2, p0);
	XMVECTOR V01 = XMLoadFloat3(&v01);
	XMVECTOR V02 = XMLoadFloat3(&v02);
	XMStoreFloat3(&n,XMVector3Cross(V01, V02));
	dvn = v.x*n.x + v.y*n.y + v.z*n.z;
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;
	dpn = pos.x*n.x + pos.y*n.y + pos.z*n.z;
	t = (dp0n - dpn) / dvn;
	hp = AddXMFloat3(pos,ScalXMFloat3(v,t));
	return hp.y;
}

XMFLOAT4 CField::GetNormal(XMFLOAT3 * pos)
{
	// 得られたposから、何枚目のポリゴンにいるかを求める。
	int number = 0;
	if ((abs((long)pos->x) >= ((SQUARE_NUMBER * SQUARE_SIZE) / 2)) || (abs((long)pos->z) >= ((SQUARE_NUMBER * SQUARE_SIZE) / 2))) {
		// 禁じられた場所にいる
		assert(false);
	}

	XMFLOAT3 checkpos = vertex[0].Position;
	checkpos.x += SQUARE_SIZE;
	checkpos.z -= SQUARE_SIZE;
	while (pos->x > checkpos.x) {
		checkpos.x += SQUARE_SIZE;
		number += SQUARE_NUMBER;
	}
	while (pos->z < checkpos.z) {
		checkpos.z -= SQUARE_SIZE;
		number++;
	}
	// これで、どのQUADにいるかわかるので、そのポリゴンの斜線の頂点番号を求める。
	int l = number / SQUARE_NUMBER;
	int r = number % SQUARE_NUMBER;
	int verANum = l * (2 * (SQUARE_NUMBER + 2)) + 2 * r + 1;
	int verBNum = verANum + 1;

	// y入れちゃいけない？
	XMVECTOR vecDev = XMLoadFloat3(&vertex[verBNum].Position) - XMLoadFloat3(&vertex[verANum].Position);
	XMVECTOR vecDir = XMLoadFloat3(pos) - XMLoadFloat3(&vertex[verANum].Position);
	XMVECTOR vecRet = XMVector3Cross(vecDir, vecDev);
	
	vecRet = XMVector3Normalize(vecRet);
	XMFLOAT4 ret;
	XMStoreFloat4(&ret,vecRet);

	
	// いまいるべきY座標をretのwにぶち込む
	XMFLOAT3 VecA;	// こいつが斜線
	VecA.x = vertex[verBNum].Position.x - vertex[verANum].Position.x;
	VecA.y = vertex[verBNum].Position.y - vertex[verANum].Position.y;
	VecA.z = vertex[verBNum].Position.z - vertex[verANum].Position.z;
	XMFLOAT3 VecB;
	VecB.x = pos->x - vertex[verANum].Position.x;
	VecB.y = pos->y - vertex[verANum].Position.y;
	VecB.z = pos->z - vertex[verANum].Position.z;
	XMVECTOR VecC;
	XMVECTOR bufA = XMLoadFloat3(&VecA);
	XMVECTOR bufB = XMLoadFloat3(&VecB);
	VecC = XMVector3Cross(bufA, bufB);
	XMFLOAT3 judgeVec;
	XMStoreFloat3(&judgeVec, VecC);
	if (judgeVec.y > 0) {
		// 左上
	}
	else {
		// 右上
	}

	
	XMFLOAT3 vecC;
	if (ret.z >= 0) {
		vecC.x = pos->x - vertex[verANum - 1].Position.x;
		vecC.y = vertex[verANum - 1].Position.y;
		vecC.z = pos->z - vertex[verANum - 1].Position.z;
	}
	else {
		vecC.x = pos->x - vertex[verBNum + 1].Position.x;
		vecC.y = vertex[verBNum + 1].Position.y;
		vecC.z = pos->z - vertex[verBNum + 1].Position.z;
	}
	float A = sqrt(VecA.x * VecA.x + VecA.z * VecA.z);
	float B = sqrt(VecB.x * VecB.x + VecB.z * VecB.z);
	float C = sqrt(vecC.x * vecC.x + vecC.z * vecC.z);
	
	ret.w = ((B + C) / (A + B + C)) * VecA.y + ((A + C) / (A + B + C)) * VecB.y + ((B + A) / (A + B + C)) * vecC.y;

	return ret;
}

XMFLOAT3 AddXMFloat3(XMFLOAT3 a, XMFLOAT3 b) {
	XMFLOAT3 ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}
XMFLOAT3 DecXMFloat3(XMFLOAT3 a, XMFLOAT3 b) {
	XMFLOAT3 ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

XMFLOAT3 ScalXMFloat3(XMFLOAT3 a, float b) {
	XMFLOAT3 ret;
	ret.x = a.x * b;
	ret.y = a.y * b;
	ret.z = a.z * b;
	return ret;
}