#pragma once
#include "Input.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include <list>

/// 自キャラ

class Player {
public:
	// 初期化
	void Initialize(Model* model_, uint32_t textureHandle);
	// 更新
	void Update();
	// 回転
	void Rotate();
	// 攻撃
	void Attack();
	// デストラクタ
	~Player();
	// 発射間隔
	void Interval();
	// 描画
	void Draw(const ViewProjection& viewProjection_);

	// 発射間隔
	static const int kAttackInterval = 20;

	// キーボード入力
	Input* input_ = nullptr;
	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// 弾
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 発射タイマー
	int32_t AttackTimer = 0;
};