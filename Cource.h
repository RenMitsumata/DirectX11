#pragma once

#ifndef COURCE_H
#define COURCE_H
//-----------------------------------

#define COURCE_LENGTH


//�@CourceData�F�R�[�X�i���b�V���t�B�[���h�j�̍��W����сA�v���C���[�̓���̎��Ԃɂ�������W��\�����́B
typedef struct VERTEX_Tag {
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
	XMFLOAT2 texcoord;
}VERTEX;
typedef struct CourceData_Tag {
	float distance;			//�@�X�^�[�g�n�_�i�O�j����̋����B���b�V���t�B�[���h�p�B
	XMFLOAT3 centerpos;	//�@�P�ʋ���������̃��b�V���t�B�[���h�̒��S���W
	VERTEX vertex[4];	//�@�R�[�X�i���b�V���t�B�[���h�j�p�Ɍv�Z���ꂽ���_���W�����[����B
	XMFLOAT3 vecFront;	//�@�R�[�X�̐i�s�����B�v�Z�ɗp����
	XMFLOAT3 pitchyawroll; //�@�ǂꂭ�炢�X���Ă��邩
}CourceData;

class Cource : public CGameObject
{
private:
	int courceLength;		//�@�R�[�X�̒���
	ID3D11Buffer* vertexBuffer = nullptr;	// ���_�o�b�t�@
	ID3D11Buffer* indexBuffer = nullptr; // �C���f�b�N�X�o�b�t�@
	CourceData** courceData;
	float yaw;				//�@�j���[�g�����iplayer��front���y�{��������������ԁj����A�ǂꂾ���x����]�����Ă��邩��\���l�B
	float pitch;			//�@�j���[�g��������A�ǂꂾ���w����]�����Ă��邩��\���l�B
	float roll;				//�@�j���[�g��������A�ǂꂾ���y����]�����Ă��邩��\���l�B
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



