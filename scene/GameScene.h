#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Skydome.h>
#include <RailCamera.h>
#include<list>
#include"EnemyBullet.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	// 攻撃
	void Fire();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// スプライト
	uint32_t texHandle_ = 0u;
	// モデル
	Model* model_ = nullptr;
	//3Dモデル スカイドーム
	Model* modelSkydome_ = nullptr;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	uint32_t textureHandle_ = 0u;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// カメラ
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
	// 敵キャラ
	Enemy* enemy_ = nullptr;
	//スカイドーム
	Skydome* skydome_ = nullptr;
	//	レールカメラ
	RailCamera* railCamera_ = nullptr;


	//敵の引っ越しさせたやつたち
	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	// テクスチャハンドル

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};