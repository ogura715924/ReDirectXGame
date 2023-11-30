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
#include<sstream>
#include <Scene.h>
#include"Result.h"

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

	//敵発生データの読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	/// 敵の発生処理
	void SpawnEnemy(Vector3 pos);

	bool IsSceneEnd() { return isSceneEnd_; }
	Scene::SceneType NextScene() { return Scene::SceneType::kGameResult; }

	/// <summary>
	/// エネミーが発射した弾を登録する関数
	/// </summary>
	/// <param name="enemyBullet"></param>
//	void AddEnemyBullet(EnemyBullet* enemyBullet);

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

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	Result* result_=nullptr;

		// 敵が発生待機中か
	bool isWait_ = false;
	// 敵が発生するまでの時間
	int32_t waitTime_ = 0;

	//敵の引っ越しさせたやつたち
	// 弾
	std::list<EnemyBullet*> enemyBullets_;

bool isSceneEnd_ = false;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};