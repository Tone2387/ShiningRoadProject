#ifndef SCENE_MOVIE_BASE_H_
#define SCENE_MOVIE_BASE_H_

#include "SceneBase.h"



//���[�r�[�V�[���̊��N���X.
class clsSCENE_MOVIE_BASE : public clsSCENE_BASE
{
public:
	clsSCENE_MOVIE_BASE( 
		clsPOINTER_GROUP* const ptrGroup,
		const enSCENE enScene,
		const char* sCutFrameFilePath );
	~clsSCENE_MOVIE_BASE();

protected:
	//�c��̓�͌p����ō��.
	void CreateProduct() override;//�p����̂��̊֐��̍Ō�ł��̃N���X�̂��̊֐����g������.
//	void RenderProduct( const D3DXVECTOR3 &vCamPos ) override;
//	void RenderUi() override;//�u UI�� �vRender.

	virtual void InitMovieProduct() = 0;
	virtual void UpdateMovieProduct( int iOtherDataIndex ) = 0;
	//�t���[�����������Ă��Ȃ��Ă����̃J�b�g�֔��.
	void NextCut();

protected:
	//�J�����̈ړ����x�Ȃ�.
	std::vector< float > m_vecfOtherData;
	//���̃J�b�g�̃t���[���J�E���g.
	float m_fMovieFrame;
	//�e�J�b�g��񂫂�̃t���O�g���K�[.
	bool m_isTrigger;
	//�J�b�g����.
	int m_iCut;

private:
	void UpdateProduct( enSCENE &enNextScene ) final;
	//���[�r�[���ۂ����������߂ׂ̈̊֐�.
	void InitMovie();
	void UpdateMovie();

private:
	//�Ō�̃J�b�g�ԍ�.
	int m_iCutEnd;
	//���̃V�[��.
	const enSCENE m_enNEXT_SCENE;

	//�S�J�b�g�̃t���[����.
	std::vector< float > m_vecfMovieFrameNext;

};


#endif//#ifndef SCENE_MOVIE_BASE_H_