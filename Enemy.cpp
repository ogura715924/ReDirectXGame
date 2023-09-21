#include "Enemy.h"
#include "Player.h"
#include <assert.h>
#include <cassert>

// �f�X�g���N�^
Enemy::~Enemy() {

	// bullet_�̉��

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Enemy::GetWorldPosition() {

	// ���[���h���W������ϐ�
	Vector3 worldPos{};
	worldTransform_.matWorld_.m;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::Initialize(Model* model, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �������烂�f���ƃe�N�X�`���n���h�����󂯎��
	model_ = model;
	velocity_ = velocity;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("jousi.png");

	// �ڋ߃t�F�[�Y������
	ApproachInitialize();

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_.z = 3.0f;
}

void Enemy::Update() {

	// ���W���ړ�������
	// worldTransform_.translation_.z -= 0.1f;

	// �t�F�[�Y
	switch (phase_) {
	case Phase::Approach:
	default:
		// �ړ�(�x�N�g�������Z)
		worldTransform_.translation_.x += 0.1f;
		// ����̈ʒu�ɓ��B�����痣�E
		if (worldTransform_.translation_.x > 2.0f) {
			phase_ = Phase::Leave;
		}
	case Phase::Leave:
		// �ړ��i�x�N�g�������Z�j
		// worldTransform_.translation_.x += 0.1f;
		worldTransform_.translation_.z -= 0.1f;
		break;
	}

	// ���[���h�g�����X�t�H�[���̍X�V�@�ꏊ�������Ƃ��Ɏg����
	worldTransform_.UpdateMatrix();

	// �e�֘A

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// ���˃^�C�}�[�J�E���g�_�E��
	FireTimer--;
	// �w�肵�����ԂɒB����
	if (FireTimer <= 0) {
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		FireTimer = kFireInterval;
	}
}

void Enemy::Fire() {
	assert(player_);

	// �e�̑��x
	const float kBulletSpeed = 1.0f;
	// ���L�����̃��[���h���W���擾����
	player_->GetWorldPosition();
	// �G�L�����̃��[���h���W���擾����
	GetWorldPosition();
	// �G�L����->���L�����̍����x�N�g�������߂�
	Vector3 DifferenceVector = {
	    GetWorldPosition().x - player_->GetWorldPosition().x,
	    GetWorldPosition().y - player_->GetWorldPosition().y,
	    GetWorldPosition().z - player_->GetWorldPosition().z};
	// �x�N�g���̐��K��
	DifferenceVector = Normalize(DifferenceVector);
	// �x�N�g���̒����𑬂��ɍ��킹��
	velocity_.x = DifferenceVector.x * kBulletSpeed;
	velocity_.y = DifferenceVector.y * kBulletSpeed;
	velocity_.z = DifferenceVector.z * kBulletSpeed;

	// �e�𐶐���������
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Intialize(model_, worldTransform_.translation_, velocity_);

	// �e��o�^����
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() {
	// ���˃^�C�}�[��������
	FireTimer = 3;
}

void Enemy::Draw(const ViewProjection& viewProjection_) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}