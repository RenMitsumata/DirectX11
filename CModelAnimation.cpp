#define NOMINMAX
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "model.h"
#include <string>
#include <vector>

#include <unordered_map>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#pragma comment (lib,"assimp.lib")



struct DEFORM_VERTEX {
	aiVector3D position;
	aiVector3D deformPosition;
	aiVector3D normal;
	aiVector3D deformNormal;
	int boneNum;
	// int boneIndex[4];	�������ق�
	std::string boneName[4];
	float boneWeight[4];

};

struct BONE {
	// std::string name;
	aiMatrix4x4 matrix;
	aiMatrix4x4 animationMatrix;
	aiMatrix4x4 offsetMatrix;
};

#include "CModelAnimation.h"

std::vector<DEFORM_VERTEX>* pDeformVertexs;
std::unordered_map<std::string, BONE> Bones;


void CModelAnimation::CreateBone(aiNode * pNode)
{
	BONE bone;
	Bones[pNode->mName.C_Str()] = bone;	// �m�[�h�̖��O���{�[���������ɂ��� = �{�[����
	for (int i = 0; i < pNode->mNumChildren; i++) {
		CreateBone(pNode->mChildren[i]);
	}
}

void CModelAnimation::UpdateBoneMatrix(aiNode * pNode, aiMatrix4x4* pMatrix)
{
	BONE* pBone = &Bones[pNode->mName.C_Str()];
	aiMatrix4x4 worldMatrix;
	worldMatrix = *pMatrix;
	worldMatrix *= pBone->animationMatrix;
	pBone->matrix = worldMatrix;
	pBone->matrix *= pBone->offsetMatrix;
	for (int n = 0; n < pNode->mNumChildren; n++) {
		UpdateBoneMatrix(pNode->mChildren[n], &worldMatrix);
	}
}

void CModelAnimation::Draw()
{
	aiNode* pRootNode = pScene->mRootNode;
	XMMATRIX matrix = XMMatrixIdentity();

	matrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	
	DrawChild(pRootNode,0,0,matrix);
}

void CModelAnimation::Draw(XMMATRIX matrix, float canonAngle, float canonUpAngle)
{
	
	//CRenderer::SetWorldMatrix(&matrix);
	
	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(vertexBuffer);
	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(indexBuffer);
	

	aiNode* pCurrentNode = pScene->mRootNode; // ���̃V�[���́H

	DrawChild(pCurrentNode,canonAngle,canonUpAngle,matrix);


	
}

void CModelAnimation::DrawChild(aiNode * pNode, float canonAngle, float canonUpAngle, XMMATRIX matrix)
{
	XMMATRIX local = XMMatrixIdentity();

	if (!pScene->HasAnimations()) {
		if (strcmp(pNode->mName.data, "Body") == 0) {
			local *= XMMatrixRotationZ(AI_MATH_PI);

		}



		

		//aiQuaternion aiQuat = m_NodeRotation[pNode->mName.C_Str()];
		//XMVECTOR quat = XMLoadFloat4(&XMFLOAT4(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w));
		//aiVector3D aiPos = m_NodePosition[pNode->mName.C_Str()];

		

		if (strcmp(pNode->mName.data, "Canon") == 0) {

			local *= XMMatrixRotationZ(canonAngle);


		}

		if (strcmp(pNode->mName.data, "CanonBody") == 0) {

			local *= XMMatrixRotationX(canonUpAngle);
		}


		// �܂��̓��[�J�����W�����߂āA�X�^�b�N�ɉ�������
		
		aiMatrix4x4 localMat = pNode->mTransformation;
		aiTransposeMatrix4(&localMat);
		XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
			localMat.b1, localMat.b2, localMat.b3, localMat.b4,
			localMat.c1, localMat.c2, localMat.c3, localMat.c4,
			localMat.d1, localMat.d2, localMat.d3, localMat.d4
		);

		local = local * pushMatrix * matrix;
		

		//XMMATRIX world = XMMatrixRotationQuaternion(quat);

		//world *= XMMatrixTranslation(aiPos.x * 0.01f, aiPos.y * 0.01f, aiPos.z * 0.01f);
		//world *= pushMatrix;



		//world *= matrix;


		CRenderer::SetWorldMatrix(&local);

		// �܂��A������`�悷��
		for (int m = 0; m < pNode->mNumMeshes; m++) {
			// �}�e���A���ݒ�
			CRenderer::SetMaterial(m_SubsetArray[pNode->mMeshes[m]].Material.Material);


			// �|���S���`��
			CRenderer::DrawIndexed(m_SubsetArray[pNode->mMeshes[m]].IndexNum, m_SubsetArray[pNode->mMeshes[m]].StartIndex, meshes[pNode->mMeshes[m]].baseIndexNum);

		}
		// ���̌�A�q��������Ύq����`�悷��
		for (int i = 0; i < pNode->mNumChildren; i++) {
			
			DrawChild(pNode->mChildren[i], canonAngle, canonUpAngle, local);
			
		}
	}

	// �A�j���[�V��������
	else {
		
		
		/*
		// �܂��̓��[�J�����W�����߂āA�X�^�b�N�ɉ�������
		aiMatrix4x4 localMat = pNode->mTransformation;
		aiTransposeMatrix4(&localMat);
		XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
			localMat.b1, localMat.b2, localMat.b3, localMat.b4,
			localMat.c1, localMat.c2, localMat.c3, localMat.c4,
			localMat.d1, localMat.d2, localMat.d3, localMat.d4
		);
		*/

		XMMATRIX world;

		aiQuaternion aiQuat = m_NodeRotation[pNode->mName.C_Str()];
		XMVECTOR auat = XMLoadFloat4(&XMFLOAT4(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w));
		world = XMMatrixRotationQuaternion(auat);

		aiVector3D aiVector = m_NodePosition[pNode->mName.C_Str()];
		XMVECTOR  pos = XMLoadFloat3(&XMFLOAT3(aiVector.x, aiVector.y, aiVector.z));
		world *= XMMatrixTranslationFromVector(pos);
		
		
		

		world *= matrix;
		
		
		CRenderer::SetWorldMatrix(&world);

		// �܂��A������`�悷��
		for (int m = 0; m < pNode->mNumMeshes; m++) {
			// �}�e���A���ݒ�
			CRenderer::SetMaterial(m_SubsetArray[pNode->mMeshes[m]].Material.Material);


			// �|���S���`��
			CRenderer::DrawIndexed(m_SubsetArray[pNode->mMeshes[m]].IndexNum, m_SubsetArray[pNode->mMeshes[m]].StartIndex, meshes[pNode->mMeshes[m]].baseIndexNum);

		}
		 
		for (int i = 0; i < pNode->mNumChildren; i++) {
			DrawChild(pNode->mChildren[i], canonAngle, canonUpAngle, world);
		}
		
	}
	
}

void CModelAnimation::Load(const char * filename)
{
	
	animCnt = 0;
	pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (pScene == nullptr) {
		char filestring[256];
		lstrcpy(filestring, filename);
		char exp[32] = { "���ǂݍ��߂܂���" };
		lstrcat(filestring, exp);
		MessageBox(NULL, filestring, "Assimp", MB_OK | MB_ICONHAND);
	}
	
	CreateBone(pScene->mRootNode);
	pDeformVertexs = new std::vector<DEFORM_VERTEX>[pScene->mNumMeshes];
	for (auto m = 0; m < pScene->mNumMeshes; m++) {
		aiMesh* pMesh = pScene->mMeshes[m];
		for (auto v = 0; v < pMesh->mNumVertices; v++) {
			DEFORM_VERTEX defVertex;
			defVertex.position = pMesh->mVertices[v];
			defVertex.deformPosition = pMesh->mVertices[v];
			defVertex.normal = pMesh->mNormals[v];
			defVertex.deformNormal = pMesh->mNormals[v];
			defVertex.boneNum = 0;
			for (auto b = 0; b < 4; b++) {
				// defVertex.boneIndex = 0;
				defVertex.boneName[b] = {};
				defVertex.boneWeight[b] = 0.0f;
			}
			pDeformVertexs[m].push_back(defVertex);
		}
		for (auto b = 0; b < pMesh->mNumBones; b++) {
			aiBone* pBone = pMesh->mBones[b];
			Bones[pBone->mName.C_Str()].offsetMatrix = pBone->mOffsetMatrix;
			for (auto w = 0; w < pBone->mNumWeights; w++) {
				aiVertexWeight weight = pBone->mWeights[w];
				pDeformVertexs[m][weight.mVertexId].boneWeight[pDeformVertexs[m][weight.mVertexId].boneNum] = weight.mWeight;
				pDeformVertexs[m][weight.mVertexId].boneName[pDeformVertexs[m][weight.mVertexId].boneNum] = pBone->mName.C_Str();
				pDeformVertexs[m][weight.mVertexId].boneNum++;
				if (pDeformVertexs[m][weight.mVertexId].boneNum > 4) {
					MessageBox(nullptr, "���̃��f���f�[�^�͐������\������Ȃ��\��������܂�", "�x��", MB_OK);
				}
			}
		}

	}





	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̒P�ꉻ�̂��߂̃J�E���^
	unsigned int indexCnt = 0;
	unsigned int vertexCount = 0;
	meshes = new MESH[pScene->mNumMeshes];

	std::vector<VERTEX_3D> _VertexList;
	std::vector<unsigned short> _IndexList;


	m_SubsetArray = new DX11_SUBSET[pScene->mNumMeshes];



	// �摜�t�@�C���̓ǂݍ���
	pTextureArray = new CTexture*[pScene->mNumMaterials];
	for (int p = 0; p < pScene->mNumMaterials; p++) {
		aiString path;

		if (pScene->mMaterials[p]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			// �}�e���A���ɉ摜������
			// �摜�͓����t�@�C���H�O���t�@�C���H
			if (path.data[0] == '*') {
				/*
				// FBX�����ɉ摜�t�@�C��������i�o�[�W�����ɂ���ĈقȂ�̂Œ��Ӂj

				int id = atoi(&path.data[1]);
				TextureSamba = LoadTextureFromMemory((const unsigned char*)g_pScene->mTextures[id]->pcData, g_pScene->mTextures[id]->mWidth);
				*/
			}
			else {
				std::string texPath = path.data;
				size_t pos = texPath.find_last_of("\\/");
				std::string headerPath = texPath.substr(0, pos + 1);
				headerPath += path.data;
				texPath.c_str();	// string�̐擪�A�h���X���擾�ł���
				pTextureArray[p]->Load(headerPath.c_str());
			}
		}
	}
	







	for (int i = 0; i < pScene->mNumMeshes; i++) {
		// �e�N�X�`���̓ǂݍ���
		if (pScene->mTextures == '\0'){
			int a = 0;
		}


		meshes[i].pMesh = pScene->mMeshes[i];
		
		// �}�e���A���̐ݒ�
		const aiMaterial* mat = pScene->mMaterials[meshes[i].pMesh->mMaterialIndex];
		
		aiColor4D bufColor;

		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &bufColor);
		m_SubsetArray[i].Material.Material.Diffuse.r = bufColor.r;
		m_SubsetArray[i].Material.Material.Diffuse.g = bufColor.g;
		m_SubsetArray[i].Material.Material.Diffuse.b = bufColor.b;
		m_SubsetArray[i].Material.Material.Diffuse.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &bufColor);
		m_SubsetArray[i].Material.Material.Ambient.r = bufColor.r;
		m_SubsetArray[i].Material.Material.Ambient.g = bufColor.g;
		m_SubsetArray[i].Material.Material.Ambient.b = bufColor.b;
		m_SubsetArray[i].Material.Material.Ambient.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &bufColor);
		m_SubsetArray[i].Material.Material.Specular.r = bufColor.r;
		m_SubsetArray[i].Material.Material.Specular.g = bufColor.g;
		m_SubsetArray[i].Material.Material.Specular.b = bufColor.b;
		m_SubsetArray[i].Material.Material.Specular.a = bufColor.a;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &bufColor);
		m_SubsetArray[i].Material.Material.Emission.r = bufColor.r;
		m_SubsetArray[i].Material.Material.Emission.g = bufColor.g;
		m_SubsetArray[i].Material.Material.Emission.b = bufColor.b;
		m_SubsetArray[i].Material.Material.Emission.a = bufColor.a;

		
		meshes[i].baseIndexNum = vertexCount;

		// �{���Ȃ炱����material��texture�֌W��`��
		//meshes[i].pTexture = new CTexture();
		//pScene->mMeshes[pNode->mMeshes[0]]->mVertices;
		VERTEX_3D vertex;
		for (int n = 0; n < meshes[i].pMesh->mNumVertices; n++) {
			vertex.Position.x = meshes[i].pMesh->mVertices[n].x;
			vertex.Position.y = meshes[i].pMesh->mVertices[n].y;
			vertex.Position.z = meshes[i].pMesh->mVertices[n].z;
			vertex.Normal.x = meshes[i].pMesh->mNormals[n].x;
			vertex.Normal.y = meshes[i].pMesh->mNormals[n].y;
			vertex.Normal.z = meshes[i].pMesh->mNormals[n].z;
			vertex.Diffuse.x = 3.0f;
			vertex.Diffuse.y = 3.0f;
			vertex.Diffuse.z = 3.0f;
			vertex.Diffuse.w = 1.0f;
			vertex.TexCoord.x = 0.0f;
			vertex.TexCoord.y = 0.0f;
			_VertexList.push_back(vertex);
			vertexCount++;
		}
		
		// �C���f�b�N�X�o�b�t�@�����
		meshes[i].pFaces = new FACE[meshes[i].pMesh->mNumFaces];
		unsigned int indexNum = 0;
		for (int in = 0; in < meshes[i].pMesh->mNumFaces; in++) {
			meshes[i].pFaces[in].pFace = &meshes[i].pMesh->mFaces[in];
			indexNum += meshes[i].pFaces[in].pFace->mNumIndices;
		}
		

		// �C���f�b�N�X�̊J�n�ʒu�����߂�
		m_SubsetArray[i].StartIndex = indexCnt;
		


		unsigned short indexIns;
		int a = 0;
		for (int f = 0; f < meshes[i].pMesh->mNumFaces; f++) {
			for (int idx = 0; idx < meshes[i].pFaces[f].pFace->mNumIndices; idx++) {
				indexIns = meshes[i].pFaces[f].pFace->mIndices[idx];
				_IndexList.push_back(indexIns);
				indexCnt++;
				a++;
			}
		}

		m_SubsetArray[i].IndexNum = a;
		
	}

	

	// ���_�o�b�t�@�̍쐬
	VERTEX_3D* vertexBuf = new VERTEX_3D[_VertexList.size()];
	unsigned int num = 0;
	for (VERTEX_3D oneVertex : _VertexList) {
		vertexBuf[num] = oneVertex;
		num++;
	}
	 
	// �C���f�b�N�X�o�b�t�@�̍쐻
	unsigned short* indexBuf = new unsigned short[_IndexList.size()];
	num = 0;
	for (unsigned short oneIndex : _IndexList) {
		indexBuf[num] = oneIndex;
		num++;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_3D) * _VertexList.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = vertexBuf;
	vbData.SysMemPitch = 0;
	vbData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vbData, &vertexBuffer);

	delete vertexBuf;




	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * _IndexList.size();
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = indexBuf;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&indexBufferDesc, &ibData, &indexBuffer);

	delete indexBuf;

	// �T�u�Z�b�g�ݒ�
	{

		m_SubsetNum = pScene->mNumMeshes;

		for (unsigned short i = 0; i < m_SubsetNum; i++)
		{
			//m_SubsetArray[i].Material.Texture = new CTexture();
			/*
			if (m_SubsetArray[i].Material.Texture[0] != '\0') {
				m_SubsetArray[i].Material.Texture->Load(model.SubsetArray[i].Material.TextureName);
			}
			*/	
		}
	}
	

	
	
}

void CModelAnimation::UnLoad()
{
	delete[] meshes;
	delete[] m_SubsetArray;
	aiReleaseImport(pScene);
	if (vertexBuffer) {
		vertexBuffer->Release();
	}
	if (indexBuffer) {
		indexBuffer->Release();
	}
}

void CModelAnimation::Update(int Frame){
	


	// 0�Ԗڂ̃A�j���[�V�������擾
	if (pScene->HasAnimations()) {
		/*�@�����̃A�j���[�V�������擾����ꍇ
		for(int i=0;i<g_pScene->mNumAnimations;i++){
			aiAnimation* pAnimation = new aiAnimation[g_pScene->mNumAnimations];
			pAnimation = g_pScene->mAnimations[i];
		}
		*/



		
		// 0�Ԃ̃A�j���[�V�������擾
		aiAnimation* pAnimation = pScene->mAnimations[0];
		for (auto c = 0; c < pAnimation->mNumChannels; c++) {

			aiNodeAnim* pNodeAnim = pAnimation->mChannels[c];

			int f = Frame % pNodeAnim->mNumRotationKeys;

			int s = Frame % pNodeAnim->mNumPositionKeys;

			m_NodeRotation[pNodeAnim->mNodeName.C_Str()] = pNodeAnim->mRotationKeys[f].mValue;

			m_NodePosition[pNodeAnim->mNodeName.C_Str()] = pNodeAnim->mPositionKeys[s].mValue;

		}
		
		
		/*

		// �e���_�̍��W�ϊ��i�{���̓V�F�[�_�����ׂ��j
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			for (auto& vertex : pDeformVertexs[m])
			{
				aiMatrix4x4 matrix[4];
				aiMatrix4x4 outMatrix;
				matrix[0] = Bones[vertex.boneName[0]].matrix;
				matrix[1] = Bones[vertex.boneName[1]].matrix;
				matrix[2] = Bones[vertex.boneName[2]].matrix;
				matrix[3] = Bones[vertex.boneName[3]].matrix;

				//�E�F�C�g���l�����ă}�g���N�X�Z�o
				{
					outMatrix.a1 = matrix[0].a1 * vertex.boneWeight[0]
						+ matrix[1].a1 * vertex.boneWeight[1]
						+ matrix[2].a1 * vertex.boneWeight[2]
						+ matrix[3].a1 * vertex.boneWeight[3];

					outMatrix.a2 = matrix[0].a2 * vertex.boneWeight[0]
						+ matrix[1].a2 * vertex.boneWeight[1]
						+ matrix[2].a2 * vertex.boneWeight[2]
						+ matrix[3].a2 * vertex.boneWeight[3];

					outMatrix.a3 = matrix[0].a3 * vertex.boneWeight[0]
						+ matrix[1].a3 * vertex.boneWeight[1]
						+ matrix[2].a3 * vertex.boneWeight[2]
						+ matrix[3].a3 * vertex.boneWeight[3];

					outMatrix.a4 = matrix[0].a4 * vertex.boneWeight[0]
						+ matrix[1].a4 * vertex.boneWeight[1]
						+ matrix[2].a4 * vertex.boneWeight[2]
						+ matrix[3].a4 * vertex.boneWeight[3];



					outMatrix.b1 = matrix[0].b1 * vertex.boneWeight[0]
						+ matrix[1].b1 * vertex.boneWeight[1]
						+ matrix[2].b1 * vertex.boneWeight[2]
						+ matrix[3].b1 * vertex.boneWeight[3];

					outMatrix.b2 = matrix[0].b2 * vertex.boneWeight[0]
						+ matrix[1].b2 * vertex.boneWeight[1]
						+ matrix[2].b2 * vertex.boneWeight[2]
						+ matrix[3].b2 * vertex.boneWeight[3];

					outMatrix.b3 = matrix[0].b3 * vertex.boneWeight[0]
						+ matrix[1].b3 * vertex.boneWeight[1]
						+ matrix[2].b3 * vertex.boneWeight[2]
						+ matrix[3].b3 * vertex.boneWeight[3];

					outMatrix.b4 = matrix[0].b4 * vertex.boneWeight[0]
						+ matrix[1].b4 * vertex.boneWeight[1]
						+ matrix[2].b4 * vertex.boneWeight[2]
						+ matrix[3].b4 * vertex.boneWeight[3];



					outMatrix.c1 = matrix[0].c1 * vertex.boneWeight[0]
						+ matrix[1].c1 * vertex.boneWeight[1]
						+ matrix[2].c1 * vertex.boneWeight[2]
						+ matrix[3].c1 * vertex.boneWeight[3];

					outMatrix.c2 = matrix[0].c2 * vertex.boneWeight[0]
						+ matrix[1].c2 * vertex.boneWeight[1]
						+ matrix[2].c2 * vertex.boneWeight[2]
						+ matrix[3].c2 * vertex.boneWeight[3];

					outMatrix.c3 = matrix[0].c3 * vertex.boneWeight[0]
						+ matrix[1].c3 * vertex.boneWeight[1]
						+ matrix[2].c3 * vertex.boneWeight[2]
						+ matrix[3].c3 * vertex.boneWeight[3];

					outMatrix.c4 = matrix[0].c4 * vertex.boneWeight[0]
						+ matrix[1].c4 * vertex.boneWeight[1]
						+ matrix[2].c4 * vertex.boneWeight[2]
						+ matrix[3].c4 * vertex.boneWeight[3];



					outMatrix.d1 = matrix[0].d1 * vertex.boneWeight[0]
						+ matrix[1].d1 * vertex.boneWeight[1]
						+ matrix[2].d1 * vertex.boneWeight[2]
						+ matrix[3].d1 * vertex.boneWeight[3];

					outMatrix.d2 = matrix[0].d2 * vertex.boneWeight[0]
						+ matrix[1].d2 * vertex.boneWeight[1]
						+ matrix[2].d2 * vertex.boneWeight[2]
						+ matrix[3].d2 * vertex.boneWeight[3];

					outMatrix.d3 = matrix[0].d3 * vertex.boneWeight[0]
						+ matrix[1].d3 * vertex.boneWeight[1]
						+ matrix[2].d3 * vertex.boneWeight[2]
						+ matrix[3].d3 * vertex.boneWeight[3];

					outMatrix.d4 = matrix[0].d4 * vertex.boneWeight[0]
						+ matrix[1].d4 * vertex.boneWeight[1]
						+ matrix[2].d4 * vertex.boneWeight[2]
						+ matrix[3].d4 * vertex.boneWeight[3];

				}

				vertex.deformPosition = vertex.position;
				vertex.deformPosition *= outMatrix;


				//�@���ϊ��p�Ɉړ��������폜
				outMatrix.a4 = 0.0f;
				outMatrix.b4 = 0.0f;
				outMatrix.c4 = 0.0f;

				vertex.deformNormal = vertex.normal;
				vertex.deformNormal *= outMatrix;
			}
		}

		*/

		// �ċA�I�Ƀ{�[���f�[�^���X�V����
		UpdateBoneMatrix(pScene->mRootNode, &aiMatrix4x4());

	}
}

CModelAnimation::CModelAnimation()
{
}


CModelAnimation::~CModelAnimation()
{
}


/*
Update(int Animation1,int Animation2,float blend,int Frame){
aiAnimation* animation1 = m_Scene[Animation1].mAnimation[0]
aiAnimation* animation2 = m_Scene[Animation2].mAnimation[0]
for(){
	aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
	aiNodeAnim* nodeAnim2 = animation2->mChannels[c];
	mNodeRotation[]=���ʐ��`���
	mNodePosition[]=���`���
}
}
*/