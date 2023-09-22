#pragma once
#include "EnemyBullet.h"
//#include "Model.h"
#include "ViewProjection.h"
//#include "WorldTransform.h"
#include <list>

// 自機クラスの前方宣言
class Player;

class Enemy {
public:
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize(Model* model_, const Vector3& velocity);
	// 更新
	void Update();
	// 描画
	void Draw(const ViewProjection& viewProjection_);
	// 攻撃
	void Fire();
	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();
	// 接近フェーズの初期化
	void ApproachInitialize();
	// setterの利用
	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	// 大きさ取得
	int GetRadius();

	// 発射間隔
	static const int kFireInterval = 60;

	

private:
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {0,0,0};

	// フェーズ　
	Phase phase_;
	// 弾
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t FireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	
};