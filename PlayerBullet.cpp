#include"PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("ng.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}
void PlayerBullet::Update() {

	// ワールドトランスフォームの更新　場所動かすときに使える
	worldTransform_.UpdateMatrix();

	// 座標を移動させる
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void PlayerBullet::Draw(const ViewProjection& viewProjection_) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}