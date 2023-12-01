#include "GameOver.h"


GameOver::GameOver() {}

GameOver::~GameOver(){};

void GameOver::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// 画像
	textureHandleGO_ = TextureManager::Load("GameOv.png");

};

void GameOver::Update(){
	isSceneEnd_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
		isSceneEnd_ = true;
	}
};

void GameOver::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	spriteGO_ = Sprite::Create(textureHandleGO_, {0, 0});
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// 画像
	spriteGO_->Draw();
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
