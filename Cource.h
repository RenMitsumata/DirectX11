#pragma once

#ifndef COURCE_H
#define COURCE_H
//-----------------------------------

#define COURCE_LENGTH


//　CourceData：コース（メッシュフィールド）の座標および、プレイヤーの特定の時間における座標を表すもの。
typedef struct VERTEX_Tag {
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
	XMFLOAT2 texcoord;
}VERTEX;
typedef struct CourceData_Tag {
	float distance;			//　スタート地点（０）からの距離。メッシュフィールド用。
	XMFLOAT3 centerpos;	//　単位距離当たりのメッシュフィールドの中心座標
	VERTEX vertex[4];	//　コース（メッシュフィールド）用に計算された頂点座標を収納する。
	XMFLOAT3 vecFront;	//　コースの進行方向。計算に用いる
	XMFLOAT3 pitchyawroll; //　どれくらい傾いているか
}CourceData;

class Cource : public CGameObject
{
private:
	int courceLength;		//　コースの長さ
	ID3D11Buffer* vertexBuffer = nullptr;	// 頂点バッファ
	ID3D11Buffer* indexBuffer = nullptr; // インデックスバッファ
	CourceData** courceData;
	float yaw;				//　ニュートラル（playerのfrontがＺ＋方向を向いた状態）から、どれだけＹ軸回転をしているかを表す値。
	float pitch;			//　ニュートラルから、どれだけＸ軸回転をしているかを表す値。
	float roll;				//　ニュートラルから、どれだけＺ軸回転をしているかを表す値。
public:
	Cource();
	virtual ~Cource();
	void Init(void);
	void Draw(void);
	void Update(void) {}
	void Uninit(void);
	XMFLOAT3 GetCource(float distance);
	XMVECTOR GetTild(float distance);
	XMFLOAT3 GetPitchYawRoll(float distance);

};
//-----------------------------------
#endif	// COURCE_H



