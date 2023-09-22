﻿#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {
public:
	void Intialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	// メンバ関数の追加
	bool IsDead() const { return isDead_; }

		// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 大きさ取得
	int GetRadius();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;
};