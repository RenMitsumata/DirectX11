#include "main.h"
#include "renderer.h"
#include "texture.h"
#include <vector>
#include <string>
#include "GameObject.h"
#include "Cource.h"
#pragma comment(lib,"d3d11.lib")
using namespace std;

#define FILENAME ("cource/test.txt")
#define STARTPOS (XMFLOAT3(0.0f,0.0f,0.0f))
#define COURCEWIDTH (10.0f)
#define DRAWLENGTH (500.0f)	//　描画距離（前後○ｍずつ）

typedef struct CourceVtx_Tag {
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 texcoord;
}CourceVtx;

static CTexture* courceImg;


Cource::Cource()
{
}


Cource::~Cource()
{
}

//　コースデータの読み込み及び、その他初期設定
void Cource::Init(void) {
	//　コース画像読み込み
	courceImg = new CTexture;
	courceImg->Load("Assets/rainbow.png");
	//　コースファイルの読み込み
	//　コースデータはdistance,yaw,pitch,roll,radius,insline
	FILE* fp;
	fp = fopen(FILENAME, "rt");
	char buffer[64];
	//　ヘッダ部分を読み飛ばす
	for (int i = 0; i < 4; i++) {
		fgets(buffer, 64, fp);
	}
	//	Cource Length:を読み飛ばす
	fgets(buffer, 64, fp);

	//　コースの長さ（実数）を引き出す
	fscanf(fp, "%d", &courceLength);
	fscanf(fp, "\n\n", NULL);

	
	//　引き出したコースの長さ分、CourceDataを動的確保
	courceData = new CourceData*[courceLength * 8];
	for (int i = 0; i < (courceLength * 8); i++) {
		courceData[i] = new CourceData;
	}

	//　コースデータから、中心座標及び四隅の頂点の位置を計算
	XMFLOAT3 courceFront = { 0.0f,0.0f,1.0f };
	XMFLOAT3 courceRight = { 1.0f,0.0f,0.0f };
	XMFLOAT3 courceUp = { 0.0f,1.0f,0.0f };
	XMMATRIX rotateMtx;
	float deltaYaw = 0.0f;			//　ΔYaw（単位距離当たりどれくらい傾ける状態なのか（カーブ中など））
	float deltaPitch = 0.0f;		//　deltaYawのPitch版
	float deltaRoll = 0.0f;			//	deltaYawのRoll版
	float radius = 0.0f;			//　Ｙ軸回転時の、回転半径。
	float incline = 0.0f;			//　Ｘ軸回転時の、回転半径。

	vector<int> distancelist;
	vector<float> curveStatus[3];

	int disBuf;
	int k = 0;
	while (fscanf(fp,"%d ",&disBuf) != EOF) {
		//　関数？を読み解く
		float curveData[3];
		
		// curveDataの中身：[0]Roll回転量、[1]曲線半径(Y軸)、[2]曲線半径(Right軸)
		fscanf(fp,"%[^:]:",buffer);
		string moji = buffer;
		if (moji == "Curve") {
			fscanf(fp, "%f,", &curveData[0]);
			fscanf(fp, "%f,", &curveData[1]);
			fscanf(fp, "%f,", &curveData[2]);
		}
		else if(moji == "CurveEnd"){
			curveData[0] = 0.0f;
			curveData[1] = 0.0f;
			curveData[2] = 0.0f;
		}



		fscanf(fp, "\n", NULL);
		curveStatus[0].push_back(curveData[0]);
		curveStatus[1].push_back(curveData[1]);
		curveStatus[2].push_back(curveData[2]);
		distancelist.push_back(disBuf);
		k++;
	}
	fclose(fp);


	//　コースデータの生成
	for (int i = 0; i < (courceLength * 8); i++) {
		courceData[i]->distance = i * 0.125f;
		if (i == 0) {
			courceData[i]->centerpos = STARTPOS;
		}
		else {
			int Num = -1;
			bool bChange = false;
			for (int j = 0; j < k; j++) {
				if (courceData[i]->distance == distancelist[j]) {
					Num = j;
					bChange = true;
					break;
				}
			}
			// deltaYaw,deltaPitch,deltaRollの更新
			if (bChange == true) {
				radius = curveStatus[1][Num];			//　Ｙ軸回転時の、回転半径。
				incline = curveStatus[2][Num];		//　Ｘ軸回転時の、回転半径。
				if (radius == 0.0f) {
					deltaYaw = 0.0f;
				}
				else {
					deltaYaw = 0.125f / radius;
				}
				if (incline == 0.0f) {
					deltaPitch = 0.0f;
				}
				else {
					deltaPitch = 0.125f / incline;
				}
				deltaRoll = 0;
			}


			// Yaw,Pitch,Rollの更新
			yaw += deltaYaw;
			pitch += deltaPitch;
			roll += deltaRoll;

			rotateMtx = XMMatrixIdentity();
			rotateMtx *= XMMatrixRotationRollPitchYaw(deltaPitch, deltaYaw, deltaRoll);
			XMStoreFloat3(&courceFront , XMVector3TransformNormal(XMLoadFloat3(&courceFront), rotateMtx));
			XMStoreFloat3(&courceRight, XMVector3TransformNormal(XMLoadFloat3(&courceRight), rotateMtx));
			XMStoreFloat3(&courceUp, XMVector3TransformNormal(XMLoadFloat3(&courceUp), rotateMtx));
			courceData[i]->centerpos = courceData[i - 1]->centerpos + courceFront * 0.125f;
			courceData[i]->vecFront = courceFront;
		}

		//	MFの各頂点座標	　　		  =　　　中心ポジション     +/-　　　　　コース幅　　　　　　 +/-　　　　前後幅
		courceData[i]->vertex[0].position = courceData[i]->centerpos - (courceRight / 2) * COURCEWIDTH + (courceFront / 2) * 0.125f;
		courceData[i]->vertex[0].normal = courceUp;
		courceData[i]->vertex[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		courceData[i]->vertex[0].texcoord = XMFLOAT2(0.0f, 0.0f);
		courceData[i]->vertex[1].position = courceData[i]->centerpos + (courceRight / 2) * COURCEWIDTH + (courceFront / 2) * 0.125f;
		courceData[i]->vertex[1].normal = courceUp;
		courceData[i]->vertex[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		courceData[i]->vertex[1].texcoord = XMFLOAT2(1.0f, 0.0f);
		courceData[i]->vertex[2].position = courceData[i]->centerpos - (courceRight / 2) * COURCEWIDTH - (courceFront / 2) * 0.125f;
		courceData[i]->vertex[2].normal = courceUp;
		courceData[i]->vertex[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		courceData[i]->vertex[2].texcoord = XMFLOAT2(0.0f, 1.0f);
		courceData[i]->vertex[3].position = courceData[i]->centerpos + (courceRight / 2) * COURCEWIDTH - (courceFront / 2) * 0.125f;
		courceData[i]->vertex[3].normal = courceUp;
		courceData[i]->vertex[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		courceData[i]->vertex[3].texcoord = XMFLOAT2(1.0f, 1.0f);
		courceData[i]->pitchyawroll.x = pitch;
		courceData[i]->pitchyawroll.y = yaw;
		courceData[i]->pitchyawroll.z = roll;
	}
	VERTEX* vertexdata = new VERTEX[(courceLength * 8) * 4];
	for (int i = 0; i < (courceLength * 8); i++) {
		vertexdata[4* i ] = courceData[i]->vertex[0];
		vertexdata[4*i+1] = courceData[i]->vertex[1];
		vertexdata[4*i+2] = courceData[i]->vertex[2];
		vertexdata[4*i+3] = courceData[i]->vertex[3];
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX) * (courceLength * 8) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//　頂点バッファ
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertexdata;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer);
	delete[] vertexdata;
	WORD* index = new WORD[courceLength * 8 * 4];
	for (int i = 0; i < courceLength * 8 * 4; i++) {
		index[i] = i;
	}
	D3D11_BUFFER_DESC bd_vb;
	ZeroMemory(&bd_vb, sizeof(bd_vb));
	bd_vb.Usage = D3D11_USAGE_DEFAULT;
	bd_vb.ByteWidth = sizeof(WORD) * courceLength * 8 * 4;
	bd_vb.BindFlags = D3D11_BIND_INDEX_BUFFER;	//　インデックスバッファ
	bd_vb.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd_vb;
	ZeroMemory(&sd_vb, sizeof(sd_vb));
	sd_vb.pSysMem = index;
	CRenderer::GetDevice()->CreateBuffer(&bd_vb, &sd_vb, &indexBuffer);
	delete[] index;
	
}

void Cource::Draw(void) {
	//　メッシュフィールドの描画
	//int maxnum = max(0, currentPos * 8 - DRAWLENGTH);
	//int minnum = min(courceLength * 8, currentPos * 8 + DRAWLENGTH);
	
	XMMATRIX worldMtx;
	worldMtx = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMtx *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	//worldMtx *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	CRenderer::SetWorldMatrix(&worldMtx);
	CRenderer::SetDepthEnable(true);
	
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	CRenderer::GetDeviceContext()->IASetIndexBuffer(&indexBuffer[0], DXGI_FORMAT_R16_UINT, 0);
	CRenderer::SetTexture(courceImg);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed((courceLength * 8 * 4), 0, 0);
	CRenderer::SetDepthEnable(false);
	
}

void Cource::Uninit(void) {
	//　コースファイルの削除（リリース）
	indexBuffer->Release();
	vertexBuffer->Release();
	for (int i = 0; i < courceLength * 8; i++) {
		delete courceData[i];
	}
	delete courceData;
	delete courceImg;
}

XMFLOAT3 Cource::GetCource(float distance) {
	float bufDis = distance;
	bufDis *= 8;
	int bufInt = bufDis;
	if (bufInt - bufDis == 0) {
		return courceData[bufInt]->centerpos;
	}
	else {
		float a = distance * 8 - bufInt;
		float b = (bufInt + 1) - distance * 8;
		XMFLOAT3 currentPos;
		currentPos = ((a / (a + b)) * courceData[bufInt]->centerpos + (b / (a + b)) * courceData[bufInt + 1]->centerpos);
		return currentPos;
	}

}

XMVECTOR Cource::GetTild(float distance) {
	float bufDis = distance;
	bufDis *= 8;
	int bufInt = bufDis;
	if (bufInt - bufDis == 0) {
		XMFLOAT3 frontBuf = courceData[bufInt]->vecFront;
			return XMLoadFloat3(&frontBuf);
	}
	else {
		float a = distance - (bufDis / 8);
		float b = ((bufDis + 1) / 8) - distance;
		/*
		XMFLOAT3 currentFront;
		currentFront = ((a / (a + b)) * courceData[bufInt]->vecFront + (b / (a + b)) * courceData[bufInt + 1]->vecFront);
		XMStoreFloat3(&currentFront , XMVector3Normalize(XMLoadFloat3(&currentFront)));
		*/
		XMVECTOR currentFront;
		XMFLOAT3 frontBuf = ((a / (a + b)) * courceData[bufInt]->vecFront + (b / (a + b)) * courceData[bufInt + 1]->vecFront);
		currentFront = XMLoadFloat3(&frontBuf);

		return currentFront;
	}
}

XMFLOAT3 Cource::GetPitchYawRoll(float distance)
{
	float bufDis = distance;
	bufDis *= 8;
	int bufInt = bufDis;
	if (bufInt - bufDis == 0) {
		XMFLOAT3 ret = courceData[bufInt]->pitchyawroll;
		return ret;
	}
	else {
		float a = distance - (bufDis / 8);
		float b = ((bufDis + 1) / 8) - distance;
		/*
		XMFLOAT3 currentFront;
		currentFront = ((a / (a + b)) * courceData[bufInt]->vecFront + (b / (a + b)) * courceData[bufInt + 1]->vecFront);
		XMStoreFloat3(&currentFront , XMVector3Normalize(XMLoadFloat3(&currentFront)));
		*/
		
		XMFLOAT3 ret = ((a / (a + b)) * courceData[bufInt]->pitchyawroll + (b / (a + b)) * courceData[bufInt + 1]->pitchyawroll);
		
		return ret;
	}
}

