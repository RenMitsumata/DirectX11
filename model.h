#pragma once



// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// ���f���\����
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned short	VertexNum;
	unsigned short	*IndexArray;
	unsigned short	IndexNum;
	SUBSET			*SubsetArray;
	unsigned short	SubsetNum;
};



//�@�O���錾
struct aiNode;
struct aiFace;
struct aiMesh;
class CTexture;


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



class CModel
{
private:

	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMFLOAT3					m_Scale;

	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;

	DX11_SUBSET*	m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;
	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum );
	
public:
	void Init();
	void Init(const char * filename);
	void Init(const char * filename,XMFLOAT3 pos);
	void Uninit();
	void Update();
	void Update(XMFLOAT3 deltaPos);
	void Draw();
	void Draw(unsigned int mgtNum, XMFLOAT3 rootPos);
	void Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll);
	void Draw(unsigned int mgtNum, XMFLOAT3 rootPos, XMFLOAT3 yawpitchroll, float canonAngle, float canonUpAngle);

	void DrawChild(aiNode * pCurrentNode, float canonAngle, float canonUpAngle);
	
	void Draw(XMFLOAT3 m_Position);
	void Load( const char *FileName );
	void SetScale(XMFLOAT3 scale);
	void Unload();
	enum e_FILETYPE {
		e_FILEOBJ = 0,
		e_FILEFBX
	};
};