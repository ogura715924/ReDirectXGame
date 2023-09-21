#include"PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("ng.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}
void PlayerBullet::Update() {

	// ���[���h�g�����X�t�H�[���̍X�V�@�ꏊ�������Ƃ��Ɏg����
	worldTransform_.UpdateMatrix();

	// ���W���ړ�������
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// worldTransform_.translation_ += velocity_;

	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void PlayerBullet::Draw(const ViewProjection& viewProjection_) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}