#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete player_;
	delete enemy_;
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