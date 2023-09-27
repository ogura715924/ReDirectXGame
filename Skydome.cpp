#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {

	assert(model);
	model_ = model;


	//初期化
	worldTransform_.Initialize();
	//Transration
	worldTransform_.translation_ = {0, 0, 0};
	//Scale
	worldTransform_.scale_ = {500,500,500};

// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {
}

void Skydome::Draw(ViewProjection& viewProjection_) {
	// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}

