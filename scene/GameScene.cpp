#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete debugCamera_;
	delete player_;
	delete enemy_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//	敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	// 敵キャラの初期化
	Vector3 velocity(0.0f, 0.0f, 1.0f); // 速度を設定する
	enemy_->Initialize(model_, velocity);

	
	
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドームの生成
	skydome_ = new Skydome();
	// スカイドーム初期化
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {

	// #ifdef _DEBUG
	//	if (input_->TriggerKey(A)){
	////デバッグカメラ有効フラグをトグル
	//}
	// #endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバックカメラの更新処理入れる
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列のの更新と転送
		viewProjection_.UpdateMatrix();
	}
	// 自キャラの更新
	player_->Update();
	player_->Rotate();
	// 敵キャラの更新
	enemy_->Update();
	
	// 当たり判定
	CheckAllCollisions();
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// #pragma region背景スプライト描画

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

//判定対象AとBの座標
	Vector3 PosA, PosB;
	int RadiusA, RadiusB;
	float PositionMeasure;
	int RadiusMeasure;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	


	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	//敵弾の座標
		PosB = bullet->GetWorldPosition();
		RadiusB = bullet->GetRadius();
		//座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			        (PosB.y - PosA.y) * (PosB.y - PosA.y) +
		            (PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (RadiusA + RadiusB) * (RadiusA + RadiusB);
		//弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	PosA = enemy_->GetWorldPosition();
	RadiusA = enemy_->GetRadius();
	// 敵キャラと敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		PosB = bullet->GetWorldPosition();
		RadiusB = bullet->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) + 
			        (PosB.y - PosA.y) * (PosB.y - PosA.y) +
		            (PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (RadiusA + RadiusB) * (RadiusA + RadiusB);
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

	#pragma region 自弾と敵弾の当たり判定

	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
			// 自弾の座標
			PosA = bulletA->GetWorldPosition();
			RadiusA = bulletA->GetRadius();
			// 敵弾の座標
			PosB = bulletB->GetWorldPosition();
			RadiusB = bulletB->GetRadius();
			PositionMeasure  = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			            (PosB.y - PosA.y) * (PosB.y - PosA.y) +
			            (PosB.z - PosA.z) * (PosB.z - PosA.z);

			 RadiusMeasure = (RadiusA + RadiusB) * (RadiusA + RadiusB);

			if (PositionMeasure <= RadiusMeasure) {
				// 自弾の衝突時コールバックを呼び出す
				bulletA->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				bulletB->OnCollision();
			}
		}
	}
#pragma endregion

	#pragma region 自キャラと敵キャラの当たり判定


	// 自キャラの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	// 自キャラと敵キャラ全ての当たり判定
			// 敵キャラの座標
		PosB = enemy_->GetWorldPosition();
		RadiusB = enemy_->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) + (PosB.y - PosA.y) * (PosB.y - PosA.y) +
		            (PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (RadiusA + RadiusB) * (RadiusA + RadiusB);
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
		}
#pragma endregion

	}


