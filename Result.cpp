﻿#include "Result.h"

Result::Result() {}

Result::~Result(){

};

void Result::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// 画像
	textureHandleGO_ = TextureManager::Load("GameOv.png");
	
	// 画像
	textureHandleGC_ = TextureManager::Load("GameCl.png");
};

void Result::Update(){};

void Result::Draw() {
	 // コマンドリストの取得
	 ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	 // 背景スプライト描画前処理
	 Sprite::PreDraw(commandList);

	 /// <summary>
	 /// ここに背景スプライトの描画処理を追加できる
	 /// </summary>
	 // 画像
	 spriteGC_->Draw();
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
void Result::GCOnColision() {
	spriteGO_ = Sprite::Create(textureHandleGO_, {0, 0}); 
if (input_->TriggerKey(DIK_SPACE)) {
		 isSceneEnd_ = true;
	 }
};
void Result::GOOnColision() {
	 spriteGC_ = Sprite::Create(textureHandleGC_, {0, 0});
	 if (input_->TriggerKey(DIK_SPACE)) {
		 isSceneEnd_ = true;
	 }
};