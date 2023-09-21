#include"Player.h"
#include "WorldTransform.h"
#include <assert.h>
#include <cassert>
#include <imgui.h>
#include <math.h>
#define _USE_MATH_DEFINES

// �f�X�g���N�^
Player::~Player() {

	// bullet_�̉��

	for (PlayerBullet* bullet : bullets_) {
		// bullets -> PlayerBullet*�̃��X�g(�z��̂�������)
		// �͈�for�ŉ�������Ă��邩
		// bullet = bullets_[i];������Ă���bullets_�̐����������[�v����
		delete bullet;
	}
}

Vector3 Player::GetWorldPosition() {

	// ���[���h���W������ϐ�
	Vector3 worldPos{};
	worldTransform_.matWorld_.m;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// ��������󂯎�������f�����g�ݍ��܂�Ă��邩�`�F�b�N
	assert(model);
	// �������烂�f���ƃe�N�X�`���n���h�����󂯎��
	model_ = model;
	textureHandle_ = textureHandle;

	// X,Y,Z�����̃X�P�[�����O��ݒ�
	/*worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};*/

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();

	// ���ˊԊu������
	Interval();
}
void Player::Update() {

	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	// �L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	// �����������Ńx�N�g����ύX(���E)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// �����������Ńx�N�g����ύX(�㉺)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	// ���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// �ړ����E���W
	const float kMoveLimitX = 1280;
	const float kMoveLimitY = 720;

	// �͈͂𒴂��Ȃ�����
	move.x = max(move.x, -kMoveLimitX);
	move.x = min(move.x, +kMoveLimitX);
	move.y = max(move.y, -kMoveLimitY);
	move.y = min(move.y, +kMoveLimitY);

	// worldTransform_.translation_.x = sqrt(move.x * move.x + move.y * move.y);
	// worldTransform_.translation_.y = sqrt(move.x * move.x + move.y * move.y);
	// worldTransform_.translation_.z = sqrt(move.x * move.x + move.y * move.y);

	// �s��X�V
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// �L�����N�^�[�̍��W����ʕ\�����鏈��
	ImGui::Begin("Debug");
	float playerpos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerpos, 0, 1280);
	worldTransform_.translation_.x = playerpos[0];
	worldTransform_.translation_.y = playerpos[1];
	worldTransform_.translation_.z = playerpos[2];

	ImGui::End();

	// �L�����N�^�[�e�U������
	Attack();
	// �e�X�V
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// ���˃^�C�}�[�J�E���g�_�E��
	AttackTimer--;
	// �w�肵�����ԂɒB����
	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {
		// ���˃^�C�}�[��߂�
		AttackTimer = kAttackInterval;
	}

	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.UpdateMatrix();
}

void Player::Rotate() {
	// ��]����[���W�A��/frame]
	const float kRotSpeed = 0.02f;
	// �����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	// �e�𐶐����A������

	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {

		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		// �e��o�^����
		bullets_.push_back(newBullet);
	}
}

void Player::Interval() {
	// ���˃^�C�}�[��������
	AttackTimer = 3;
}

void Player::Draw(ViewProjection& ViewProjection) {
	// 3D���f����`��
	model_->Draw(worldTransform_, ViewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(ViewProjection);
	}
}