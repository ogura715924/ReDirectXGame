﻿#include"Player.h"
#include "WorldTransform.h"
#include <assert.h>
#include <cassert>
#include <imgui.h>
#include <math.h>
#include "MyMath.h"

#define _USE_MATH_DEFINES

// デストラクタ
Player::~Player() {

	// bullet_の解放

	for (PlayerBullet* bullet : bullets_) {
		// bullets -> PlayerBullet*のリスト(配列のすごい版)
		// 範囲forで何をやっているか
		// bullet = bullets_[i];をやっていてbullets_の数分だけループする
		delete bullet;
	}
}

void Player::OnCollision() {
//何もしない
}

Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	worldTransform_.matWorld_.m;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

int Player::GetRadius() { return int(); }

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;

}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 PlayerPostion) {
	// 引数から受け取ったモデルが組み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	textureHandle_ = textureHandle;

	// X,Y,Z方向のスケーリングを設定
	/*worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};*/

	worldTransform_.translation_ = PlayerPostion;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 発射間隔初期化
	Interval();
}
void Player::Update() {

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向でベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向でベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// 移動限界座標
	const float kMoveLimitX = 1280;
	const float kMoveLimitY = 720;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// worldTransform_.translation_.x = sqrt(move.x * move.x + move.y * move.y);
	// worldTransform_.translation_.y = sqrt(move.x * move.x + move.y * move.y);
	// worldTransform_.translation_.z = sqrt(move.x * move.x + move.y * move.y);

	// 行列更新
	/*worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();*/
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug");
	float playerpos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerpos, 0, 1280);
	worldTransform_.translation_.x = playerpos[0];
	worldTransform_.translation_.y = playerpos[1];
	worldTransform_.translation_.z = playerpos[2];

	ImGui::End();

	// キャラクター弾攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// 発射タイマーカウントダウン
	AttackTimer--;
	// 指定した時間に達した
	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {
		// 発射タイマーを戻す
		AttackTimer = kAttackInterval;
	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	// 弾を生成し、初期化

	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Interval() {
	// 発射タイマーを初期化
	AttackTimer = 3;
}

void Player::Draw(const ViewProjection& ViewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, ViewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(ViewProjection);
	}
}