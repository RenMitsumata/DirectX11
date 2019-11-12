#pragma once

#include <map>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

// ëOï˚êÈåæ
struct aiScene;
struct aiNode;

class CModelAnimation
{
	


private:
	
	
	std::map<std::string, aiQuaternion> m_NodeRotation;
	std::map<std::string, aiVector3D> m_NodePosition;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	DX11_SUBSET*	m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;
	const aiScene* pScene;
	// const aiScene** pScenes;
	MESH* meshes;
	CTexture** pTextureArray;
	unsigned int animCnt;
	//void DrawChild(aiNode* pNode);
public:
	void CreateBone(aiNode * pNode);
	void UpdateBoneMatrix(aiNode * pNode, aiMatrix4x4* pMatrix);
	void Draw();
	void Draw(XMMATRIX matrix, float canonAngle, float canonUpAngle);
	void DrawChild(aiNode * pNode, float canonAngle, float canonUpAngle,XMMATRIX matrix);
	//void Draw(XMMATRIX matrix);
	void Load(const char* filename);
	void UnLoad();
	void Update(int Frame);
	//void Update(int Animation,int Frame);
	CModelAnimation();
	virtual ~CModelAnimation();
};

