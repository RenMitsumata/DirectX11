#define NOMINMAX
#include "main.h"
#include "renderer.h"

#include "texture.h"


#include <Windows.h>
#include <vector>
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#pragma comment (lib,"assimp.lib")

#include "model.h"
// ����������Ƃ��́A����z�񉻂���
const aiScene* g_pScene = nullptr;
unsigned int* g_Texture = nullptr;
bool key;
static int Cnt = 0;
std::vector<XMMATRIX> _MatList;
std::vector<ID3D11Buffer*> _Buflist;
ID3D11Buffer*  i_VertexBuffer;
ID3D11Buffer*  i_IndexBuffer;

//void MakeBuffer(aiNode* pNode);


MESH* meshes;

void CModel::Init()
{
	m_Position = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );


	Load( "asset/miku_01.obj" );
		
}
/*
void MakeBuffer(aiNode* pNode) {
	for (int i = 0; i < pNode->mNumMeshes; i++) {
		for (int j = 0; j < g_pScene->mMeshes[pNode->mMeshes[i]]->mNumFaces; j++) {
			
		}
	}


}
*/

// int�̊Ǘ��ԍ���return����
void CModel::Init(const char* filename) {
	
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_pScene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
	if (g_pScene == nullptr) {
		MessageBox(NULL, "���f���t�@�C�����ǂݍ��߂܂���", "Assimp", MB_OK | MB_ICONHAND);
		exit(1);
	}

	//aiNode* node = g_pScene->mRootNode;

	//MakeBuffer(node);



	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̒P�ꉻ�̂��߂̃J�E���^
	unsigned int indexCnt = 0;



	meshes = new MESH[g_pScene->mNumMeshes];

	std::vector<VERTEX_3D> _VertexList;
	std::vector<unsigned short> _IndexList;

	m_SubsetArray = new DX11_SUBSET[g_pScene->mNumMeshes];
	for (int i = 0; i < g_pScene->mNumMeshes; i++) {
		meshes[i].pMesh = g_pScene->mMeshes[i];
		
		// �}�e���A���̐ݒ�
		const aiMaterial* mat = g_pScene->mMaterials[meshes[i].pMesh->mMaterialIndex];
		
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
		


		// �{���Ȃ炱����material��texture�֌W��`��
		
	//	meshes[i].pTexture = new CTexture();






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
		}

		// �C���f�b�N�X�̊J�n�ʒu�����߂�
		meshes[i].pFaces = new FACE[meshes[i].pMesh->mNumFaces];
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
	CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vbData, &m_VertexBuffer);

	delete vertexBuf;

	// �C���f�b�N�X�o�b�t�@�̍쐬
	unsigned short* indexBuf = new unsigned short[_IndexList.size()];
	num = 0;
	for (unsigned short oneIndex : _IndexList) {
		indexBuf[num] = oneIndex;
		num++;
	}


	D3D11_BUFFER_DESC indexBuffer;
	indexBuffer.ByteWidth = sizeof(unsigned short) * _IndexList.size();
	indexBuffer.Usage = D3D11_USAGE_DEFAULT;
	indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags = 0;
	indexBuffer.MiscFlags = 0;
	indexBuffer.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = indexBuf;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&indexBuffer, &ibData, &m_IndexBuffer);

	delete indexBuf;
	
	// �T�u�Z�b�g�ݒ�
	{
		
		m_SubsetNum = g_pScene->mNumMeshes;

		for (unsigned short i = 0; i < m_SubsetNum; i++)
		{
			m_SubsetArray[i].Material.Texture = new CTexture();
			/*
			if (m_SubsetArray[i].Material.Texture[0] != '\0') {
				m_SubsetArray[i].Material.Texture->Load(model.SubsetArray[i].Material.TextureName);
			}
			*/
		}
	}
	
	
	
}

void CModel::Init(const char * filename, XMFLOAT3 pos)
{
	m_Position = pos;
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);


	Load(filename);
}


void CModel::Uninit()
{
	delete[] meshes;
	Unload();
}


void CModel::Update()
{
	
}

void CModel::Update(XMFLOAT3 deltaPos)
{
	m_Position.x += deltaPos.x;
	m_Position.y += deltaPos.y;
	m_Position.z += deltaPos.z;
}

void CModel::Draw()
{

	// �}�g���N�X�ݒ�
	
	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );
	CRenderer::SetWorldMatrix( &world );
	
	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers( m_VertexBuffer );

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer( m_IndexBuffer );

	for( unsigned short i = 0; i < m_SubsetNum; i++ )
	{
		// �}�e���A���ݒ�
		CRenderer::SetMaterial( m_SubsetArray[i].Material.Material );

		// �e�N�X�`���ݒ�
		CRenderer::SetTexture( m_SubsetArray[i].Material.Texture );

		// �|���S���`��
		CRenderer::DrawIndexed( m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0 );
	}
	

}

// ��]�A�X�P�[���������Ȃ�A��Q������transform�s��ɂ��āArootMatrix�Ƃ��ēo�^����B
void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();	
	rootMatrix *= XMMatrixRotationY(AI_MATH_PI);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);
	
	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);
	
	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H
	
	DrawChild(pCurrentNode,0.0f,0.0f);
	_MatList.pop_back();
	Cnt = 0;
	CRenderer::SetDepthEnable(false);
}

// ��]�A�X�P�[���������Ȃ�A��Q������transform�s��ɂ��āArootMatrix�Ƃ��ēo�^����B
void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();
	rootMatrix *= XMMatrixRotationRollPitchYaw(yawpitchroll.x, yawpitchroll.y + AI_MATH_PI, yawpitchroll.z);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);

	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);

	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H

	DrawChild(pCurrentNode,0.0f,0.0f);
	_MatList.pop_back();
	Cnt = 0;
	CRenderer::SetDepthEnable(false);
}

void CModel::Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll, float canonAngle, float canonUpAngle) {
	// �܂��Arootnode��transform�s����v�b�V������(openGL��)

	XMMATRIX rootMatrix = XMMatrixIdentity();
	rootMatrix *= XMMatrixRotationRollPitchYaw(yawpitchroll.x, yawpitchroll.y, yawpitchroll.z);
	rootMatrix *= XMMatrixScaling(0.01f, 0.01f, 0.01f);
	rootMatrix *= XMMatrixTranslation(rootPos.x, rootPos.y, rootPos.z);

	CRenderer::SetWorldMatrix(&rootMatrix);
	_MatList.push_back(rootMatrix);
	CRenderer::SetDepthEnable(true);

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(m_VertexBuffer);
	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(m_IndexBuffer);

	aiNode* pCurrentNode = g_pScene->mRootNode; // ���̃V�[���́H

	DrawChild(pCurrentNode,canonAngle,canonUpAngle);
	_MatList.pop_back();
	Cnt = 0;
	//CRenderer::SetDepthEnable(false);
}

void CModel::DrawChild(aiNode* pCurrentNode, float canonAngle, float canonUpAngle) {

	if (strcmp(pCurrentNode->mName.data, "Body") == 0) {
		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationY(AI_MATH_PI);
		_MatList.push_back(bodyRotate);

	}

	

	

	// �܂��̓��[�J�����W�����߂āA�X�^�b�N�ɉ�������
	aiMatrix4x4 localMat = pCurrentNode->mTransformation;
	aiTransposeMatrix4(&localMat);
	XMMATRIX pushMatrix = XMMatrixSet(localMat.a1, localMat.a2, localMat.a3, localMat.a4,
		localMat.b1, localMat.b2, localMat.b3, localMat.b4,
		localMat.c1, localMat.c2, localMat.c3, localMat.c4,
		localMat.d1, localMat.d2, localMat.d3, localMat.d4
		);
	_MatList.push_back(pushMatrix);


	if (strcmp(pCurrentNode->mName.data, "Canon") == 0) {

		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationZ(canonAngle);
		_MatList.push_back(bodyRotate);


	}

	if (strcmp(pCurrentNode->mName.data, "CanonBody") == 0) {

		XMMATRIX bodyRotate;
		bodyRotate = XMMatrixIdentity();
		bodyRotate *= XMMatrixRotationX(canonUpAngle);
		_MatList.push_back(bodyRotate);


	}
	

	XMMATRIX worldMatrix = XMMatrixIdentity();
	for (XMMATRIX matrix : _MatList) {
		worldMatrix = matrix * worldMatrix;
	}
	CRenderer::SetWorldMatrix(&worldMatrix);

	// �܂��A������`�悷��
	for (int m = 0; m < pCurrentNode->mNumMeshes; m++) {
		// �}�e���A���ݒ�
		
		CRenderer::SetMaterial(m_SubsetArray[pCurrentNode->mMeshes[m]].Material.Material);
		
		// �e�N�X�`���ݒ�
		//CRenderer::SetTexture(meshes[pCurrentNode->mMeshes[m]].pTexture);

		// �|���S���`��
		CRenderer::DrawIndexed(m_SubsetArray[pCurrentNode->mMeshes[m]].IndexNum, m_SubsetArray[pCurrentNode->mMeshes[m]].StartIndex,0);
				

	}

	// ���̌�A�q��������Ύq����`�悷��
	for (int i = 0; i < pCurrentNode->mNumChildren; i++) {
		DrawChild(pCurrentNode->mChildren[i],canonAngle,canonUpAngle);
	}

	if (strcmp(pCurrentNode->mName.data, "CanonBody") == 0) {

		_MatList.pop_back();

	}
	if (strcmp(pCurrentNode->mName.data, "Canon") == 0) {

		_MatList.pop_back();

	}

	if (strcmp(pCurrentNode->mName.data, "Body") == 0) {
		
		_MatList.pop_back();

	}

	// �I�������A�s����|�b�v����
	_MatList.pop_back();
}




void CModel::Draw(XMFLOAT3 m_Position)
{
	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(m_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(m_IndexBuffer);

	for (unsigned short i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		CRenderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		CRenderer::SetTexture(m_SubsetArray[i].Material.Texture);

		// �|���S���`��
		CRenderer::DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

void CModel::Load( const char *FileName )
{

	MODEL model;
	LoadObj( FileName, &model );



	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX_3D ) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.VertexArray;

		CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( unsigned short ) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.IndexArray;

		CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_IndexBuffer );
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new DX11_SUBSET[ model.SubsetNum ];
		m_SubsetNum = model.SubsetNum;

		for( unsigned short i = 0; i < model.SubsetNum; i++ )
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;
			m_SubsetArray[i].Material.Texture = new CTexture();
			if (model.SubsetArray[i].Material.TextureName[0] != '\0') {
				m_SubsetArray[i].Material.Texture->Load(model.SubsetArray[i].Material.TextureName);
			}
			
		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}

void CModel::SetScale(XMFLOAT3 scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	m_Scale.z = scale.z;
}

void CModel::Unload()
{
	if (m_VertexBuffer) {
		m_VertexBuffer->Release();
	}
	if (m_IndexBuffer) {
		m_IndexBuffer->Release();
	}
	for (int i = 0; i < g_pScene->mNumMeshes; i++) {
		/*
		if (meshes[i].pTexture) {
			delete meshes[i].pTexture;
		}
		*/
	}
	delete[] meshes;

	delete[] m_SubsetArray;

}

//���f���Ǎ�////////////////////////////////////////////
void CModel::LoadObj( const char *FileName, MODEL *Model )
{

	XMFLOAT3	*positionArray;
	XMFLOAT3	*normalArray;
	XMFLOAT2	*texcoordArray;

	unsigned short	positionNum = 0;
	unsigned short	normalNum = 0;
	unsigned short	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned short	indexNum = 0;
	unsigned short	in = 0;
	unsigned short	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = nullptr;
	unsigned short	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			do
			{
				fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//�������m��
	positionArray = new XMFLOAT3[ positionNum ];
	normalArray = new XMFLOAT3[ normalNum ];
	texcoordArray = new XMFLOAT2[ texcoordNum ];


	Model->VertexArray = new VERTEX_3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[ subsetNum ];
	Model->SubsetNum = subsetNum;




	//�v�f�Ǎ�
	XMFLOAT3 *position = positionArray;
	XMFLOAT3 *normal = normalArray;
	XMFLOAT2 *texcoord = texcoordArray;

	unsigned short vc = 0;
	unsigned short ic = 0;
	unsigned short sc = 0;


	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//�}�e���A���t�@�C��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, "asset/" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//�I�u�W�F�N�g��
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//���_���W
			fscanf( file, "%f", &position->x );
			fscanf( file, "%f", &position->y );
			fscanf( file, "%f", &position->z );
			//position->x *= SCALE_MODEL;
			//position->y *= SCALE_MODEL;
			//position->z *= SCALE_MODEL;
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//�@��
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//�e�N�X�`�����W
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//�}�e���A��
			fscanf( file, "%s", str );

			if( sc != 0 )
				Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

			Model->SubsetArray[ sc ].StartIndex = ic;


			for( unsigned short i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].Name ) == 0 )
				{
					Model->SubsetArray[ sc ].Material.Material = materialArray[i].Material;
					strcpy( Model->SubsetArray[ sc ].Material.TextureName, materialArray[i].TextureName );
					strcpy( Model->SubsetArray[ sc ].Material.Name, materialArray[i].Name );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//��
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->VertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok( NULL, "/" );
					Model->VertexArray[vc].TexCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				s = strtok( NULL, "/" );	
				Model->VertexArray[vc].Normal = normalArray[ atoi( s ) - 1 ];

				Model->VertexArray[vc].Diffuse = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

	fclose(file);



	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//�}�e���A���ǂݍ���///////////////////////////////////////////////////////////////////
void CModel::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum )
{
	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	if( file == NULL )
	{
		assert(false);
		return;
	}

	MODEL_MATERIAL *materialArray;
	unsigned short materialNum = 0;

	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//�v�f�Ǎ�
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//�}�e���A����
			mc++;
			fscanf( file, "%s", materialArray[ mc ].Name );
			strcpy( materialArray[ mc ].TextureName, "" );
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//�A���r�G���g
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.b );
			materialArray[ mc ].Material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//�f�B�t���[�Y
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.b );
			materialArray[ mc ].Material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//�X�y�L����
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.b );
			materialArray[ mc ].Material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//�X�y�L�������x
			fscanf( file, "%f", &materialArray[ mc ].Material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//�A���t�@
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//�e�N�X�`��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, "asset/" );
			strcat( path, str );

			strcat( materialArray[ mc ].TextureName, path );
		}
	}

	fclose(file);
	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}


