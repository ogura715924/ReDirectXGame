#include "Enemy.h"
#include "Player.h"
#include <assert.h>
#include <cassert>
//#include "Normalize.h"

//#include "MyMath.h"

// デストラクタ
Enemy::~Enemy() {

	// bullet_の解放

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	worldTransform_.matWorld_.m;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::Initialize(Model* model, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	velocity_ = velocity;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("jousi.png");

	// 接近フェーズ初期化
	ApproachInitialize();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_.z = 3.0f;
}

void Enemy::Update() {

	// 座標を移動させる
	// worldTransform_.translation_.z -= 0.1f;

	// フェーズ
	switch (phase_) {
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		worldTransform_.translation_.x += 0.1f;
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.x > 2.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		// 移動（ベクトルを加算）
		// worldTransform_.translation_.x += 0.1f;
		//ここあとで変える
		worldTransform_.translation_.z -= 0.1f;
		break;
	}


	// ワールドトランスフォームの更新　場所動かすときに使える
	worldTransform_.UpdateMatrix();
	
	// 弾関連

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 発射タイマーカウントダウン
	FireTimer--;
	// 指定した時間に達した
	if (FireTimer <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		FireTimer = kFireInterval;
	}
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得する
	player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	GetWorldPosition();
	// 敵キャラ->自キャラの差分ベクトルを求める
	Vector3 DifferenceVector = {
	    GetWorldPosition().x - player_->GetWorldPosition().x,
	    GetWorldPosition().y - player_->GetWorldPosition().y,
	    GetWorldPosition().z - player_->GetWorldPosition().z};
	// ベクトルの正規化
	//DifferenceVector = Normalize(DifferenceVector);
	// ベクトルの長さを速さに合わせる
	velocity_.x = DifferenceVector.x * kBulletSpeed;
	velocity_.y = DifferenceVector.y * kBulletSpeed;
	velocity_.z = DifferenceVector.z * kBulletSpeed;

	// 弾を生成し初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Intialize(model_, worldTransform_.translation_, velocity_);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	FireTimer = 3;
}

void Enemy::Draw(const ViewProjection& viewProjection_) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}