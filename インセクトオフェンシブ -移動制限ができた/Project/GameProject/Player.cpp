#include "Player.h"

#define CHIP_WIDTH 640		// 1�R�}�̃T�C�Y
#define CHIP_HEIGHT 360 
#define CENTER_POS CVector2D(CHIP_WIDTH/2, CHIP_HEIGHT/2)	// ���S���W
#define MOVE_SPEED_X 5.0f	// �������̈ړ����x
#define MOVE_SPEED_Z 3.0f	// �������̈ړ����x
#define JUMP_SPEED 15.0f	// �W�����v���x
#define GRAVITY -1.0f		// �d��
#define LEFT_UPPER_LIMIT 0.0f	//���ړ�����(����)
#define RIGHT_UPPER_LIMIT 3.0f	//�E�ړ�����(����)
#define UPPER_LIMIT 5.0f	//��ړ�����(����)
#define LOWER_LIMIT 3.0f	//���ړ�����(����)


// �v���C���[�̃A�j���[�V�����f�[�^�̑O�錾
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// �ҋ@�A�j���[�V����
	{
		new TexAnim[4]
		{
			{0, 10}, {1, 10}, {2, 10},
			{3, 10},
		},
		4
	},
	// �ړ��A�j���[�V����
	{
		new TexAnim[3]
		{
			{12, 6}, {13, 6}, {14, 6},

		},
		3
	},
	// ���S�A�j���[�V����
	{
		new TexAnim[3]
		{
			{12, 12},
			{13, 12},
			{14, 12},
		},
		3
	},
	// �U���A�j���[�V����
	{
		new TexAnim[4]
		{
			{5, 6},
			{6, 6},
			{7, 6},
			{8, 6},
		},
		4
	},
};


// �R���X�g���N�^
Player::Player(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
{
	m_hp = 100;

	// �v���C���[�̉摜��ǂݍ���
	mp_image = CImage::CreateImage
	(
		"�J�}�L���Q�A�j���[�V����.png",	// �摜�t�@�C���̃p�X
		ANIM_DATA,		// �A�j���[�V�����̃f�[�^
		CHIP_WIDTH, CHIP_HEIGHT	// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation((int)EAnimType::Idle);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetSize(CHIP_WIDTH * 0.7, CHIP_HEIGHT * 0.7);

}

// �f�X�g���N�^
Player::~Player()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

// ���݂̏�Ԃ�؂�ւ�
void Player::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// �ړ������̍X�V
bool Player::UpdateMove()
{
	bool isMove = false;
	// ���L�[�������Ă����
	if (HOLD(CInput::eLeft))
	{
		// �������ֈړ�
		m_pos.x = max(m_pos.x - MOVE_SPEED_X, LEFT_UPPER_LIMIT);
		mp_image->SetFlipH(true);	//�摜�̌���
		isMove = true;
	}
	// �E�L�[�������Ă����
	else if (HOLD(CInput::eRight))
	{
		// �E�����ֈړ�
		m_pos.x = min(m_pos.x + MOVE_SPEED_X, SCREEN_WIDTH - CHIP_WIDTH / RIGHT_UPPER_LIMIT);
		mp_image->SetFlipH(false);	//�摜�̌���
		isMove = true;
	}
	// ��L�[�������Ă����
	if (HOLD(CInput::eUp))
	{
		// �������ֈړ�
		m_pos.z = max(m_pos.z - MOVE_SPEED_Z, -SCREEN_HEIGHT / UPPER_LIMIT);
		isMove = true;
	}
	// ���L�[�������Ă����
	else if (HOLD(CInput::eDown))
	{
		// ��O�����ֈړ�
		m_pos.z = min(m_pos.z + MOVE_SPEED_Z, SCREEN_HEIGHT / 2 - CHIP_HEIGHT / LOWER_LIMIT);
		isMove = true;
	}

	return isMove;
}

// �ҋ@���̍X�V����
void Player::StateIdle()
{
	// �ړ�����
	bool isMove = UpdateMove();

	// �ړ���Ԃɍ��킹�āA�A�j���[�V������؂�ւ�
	EAnimType anim = isMove ? EAnimType::Move : EAnimType::Idle;
	mp_image->ChangeAnimation((int)anim);

	// [SPACE]�L�[�ŃW�����v��Ԃֈڍs
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Jump);
	}
	// [X]�L�[�ōU����Ԃֈڍs
	else if (PUSH(CInput::eButton2))
	{
		ChangeState(EState::Attack);
	}
}

// �W�����v���̍X�V����
void Player::StateJump()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch(m_stateStep)
	{
		// �X�e�b�v0�F�W�����v�J�n
		case 0:
			// Y���i�����j�̈ړ����x�ɃW�����v�𑬓x��ݒ肵�A
			// �ڒn��Ԃ���������
			m_moveSpeedY = JUMP_SPEED;
			m_isGrounded = false;
			m_stateStep++;
			break;
		// �X�e�b�v1�F�W�����v�I��
		case 1:
			// �ڒn������A�ҋ@��Ԃֈڍs
			if (m_isGrounded)
			{
				ChangeState(EState::Idle);
			}
			break;
	}

	// �ړ�����
	bool isMove = UpdateMove();
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// �U�����̍X�V����
void Player::StateAttack()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F�U���A�j���[�V�����ɐ؂�ւ�
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Attack, false);
			m_stateStep++;
			break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
		case 1:
			// �U���A�j���[�V�������I��������A�ҋ@��Ԃֈڍs
			if (mp_image->CheckAnimationEnd())
			{
				ChangeState(EState::Idle);
			}
			break;
	}
}

// ���S���̍X�V����
void Player::StateDeath()
{
}

// �X�V����
void Player::Update()
{
	// ���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	}

	// Y���i�����j�̈ړ������W�ɔ��f
	m_pos.y += m_moveSpeedY;
	m_moveSpeedY += GRAVITY;	// Y���̈ړ����x�ɏd�͂����Z
	// �n�ʂ�艺�ɂ�����
	if (m_pos.y <= 0.0f)
	{
		// �n�ʂ̍��W�֖߂�
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
	}

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Player::Render()
{
	mp_image->Draw();
	RenderShadow();

}
