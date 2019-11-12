#pragma once

#include <map>
#include <unordered_map>

#include <assimp\vector3.h>
#include <assimp\quaternion.h>
#include <assimp\matrix4x4.h>



// 前方宣言
struct aiNode;
struct aiScene;
struct aiFace;
struct aiMesh;

struct DEFORM_VERTEX {
	aiVector3D position;
	aiVector3D deformPosition;
	aiVector3D normal;
	aiVector3D deformNormal;
	int boneNum;
	// int boneIndex[4];	←早いほう
	std::string boneName[4];
	float boneWeight[4];

};

struct BONE {
	// std::string name;
	aiMatrix4x4 matrix;
	aiMatrix4x4 animationMatrix;
	aiMatrix4x4 offsetMatrix;
};

struct FACE {
	aiFace* pFace;
};

struct MESH {
	aiMesh*			pMesh;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	//unsigned short	indexCount;
	//unsigned short	startIndex;
	FACE*			pFaces;
	unsigned short baseIndexNum;
};
class Model : public Component
{



private:

	// 共通（アニメーション有無）
	ID3D11Buffer* vertexBuffer = NULL;
	ID3D11Buffer* indexBuffer = NULL;
	DX11_SUBSET* m_SubsetArray = NULL;
	const aiScene* pScene;
	// const aiScene** pScenes;
	std::vector<DEFORM_VERTEX>* pDeformVertexs;
	std::unordered_map<std::string, BONE> Bones;

	// アニメーション時使用
	std::map<std::string, aiQuaternion> m_NodeRotation;	// 各ノードの回転を保存
	std::map<std::string, aiVector3D> m_NodePosition;	// 各ノードの位置を保存

	// 不明
	unsigned short	m_SubsetNum;


	MESH* meshes;
	CTexture** pTextureArray;
	unsigned int animCnt;
	//void DrawChild(aiNode* pNode);
public:
	void Init();
	void Update();
	void Uninit();
	void CreateBone(aiNode * pNode);
	void UpdateBoneMatrix(aiNode * pNode, aiMatrix4x4* pMatrix);
	void Draw();
	void Draw(XMMATRIX matrix, float canonAngle, float canonUpAngle);
	void DrawChild(aiNode * pNode, float canonAngle, float canonUpAngle, XMMATRIX matrix);
	//void Draw(XMMATRIX matrix);
	void Load(const char* filename);
	void UnLoad();
	void Update(int Frame);
	//void Update(int Animation,int Frame);
	Model();
	virtual ~Model();
};
