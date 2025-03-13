#include "Slime.h"

#define CHIP_SIZE 256		// 1�R�}�̃T�C�Y
#define CENTER_POS CVector2D(128.0f, 184.0f)	// ���S���W

// �X���C���̃A�j���[�V�����f�[�^
TexAnimData Slime::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 6}, { 1, 6},
			{ 2, 6}, { 3, 6},
		},
		4
	},
	{
		new TexAnim[5]
		{
			{5, 6}, {6, 6}, {7, 6},
			{8, 6}, {9, 6},
		},
		5
	},
};

// �R���X�g���N�^
Slime::Slime(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;
	m_rect.TopX = CHIP_SIZE / 4; m_rect.TopY = CHIP_SIZE / 4;
	m_rect.TopZ = -10; m_rect.Width = CHIP_SIZE / 2;
	m_rect.Height = CHIP_SIZE / 2; m_rect.Depth = 20;

	// �X���C���̉摜��ǂݍ���
	std::string imagePath;
	if (m_type == 0) imagePath = "slime_a.png";
	else if (m_type == 1) imagePath = "slime_b.png";
	else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// �摜�t�@�C���̃p�X
		ANIM_DATA,			// �A�j���[�V�����̃f�[�^
		CHIP_SIZE, CHIP_SIZE		// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

// �f�X�g���N�^
Slime::~Slime()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

// ���S����
void Slime::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::Death);
}

// ���݂̏�Ԃ�؂�ւ�
void Slime::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// �ҋ@���̍X�V����
void Slime::StateIdle()
{
	// �ҋ@�A�j���[�V�������Đ�
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// ���S���̍X�V����
void Slime::StateDeath()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�������Đ�
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Death, false);
			m_stateStep++;
			break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
		case 1:
			// �A�j���[�V�������I��������A�폜
			if (mp_image->CheckAnimationEnd())
			{
				// �폜�t���O�𗧂Ă�
				Kill();
			}
			break;
	}
}

void Slime::Damage(int damage)
{
	m_hp = max(m_hp - damage, 0);

	if (m_hp <= 0) {
		//���S����
		Death();
	}
}

bool Slime::CheckCollisionEnemy(float _playerShotX, float _playerShotY, float _playerShotZ, SRect3D _playerShotRect)
{
	if (IsHitBox3D(_playerShotX, _playerShotY, _playerShotZ, m_pos.x, m_pos.y,
		m_pos.z, _playerShotRect, m_rect));
	{
		printf("�_���[�W�I\n");
		Damage(1);	//1�͎b��l
		return true;
	}
	return false;
}

void Slime::HitCheck(void)
{

}

// �X�V����
void Slime::Update()
{
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Death);
	}
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Death:	StateDeath();	break;
	}

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Slime::Render()
{
	mp_image->Draw();
}
