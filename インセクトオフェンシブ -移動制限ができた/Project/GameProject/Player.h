#pragma once
#include "CharaBase.h"

class Player : public CharaBase
{
public:
	// �R���X�g���N�^
	Player(const CVector3D& pos);
	// �f�X�g���N�^
	~Player();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// ���
	enum class EState
	{
		Idle,	// �ҋ@���
		Jump,	// �W�����v��
		Attack,	// �U����
		Death,	// ���S
	};
	void ChangeState(EState state);

	// �ړ������̍X�V
	bool UpdateMove();

	// �ҋ@���̍X�V����
	void StateIdle();
	// �W�����v���̍X�V����
	void StateJump();
	// �U�����̍X�V����
	void StateAttack();
	// ���S���̍X�V����
	void StateDeath();

	EState m_state;
	int m_stateStep;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		Idle,	// �ҋ@
		Move,	// �ړ�
		Death,	// ���S
		Attack,	// �U��

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// �v���C���[�̉摜

	float m_moveSpeedY;	// Y���i�����j�̈ړ����x
};
