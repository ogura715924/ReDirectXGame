#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {

	assert(model);
	model_ = model;

	//初期化
	worldTransform_.Initialize();
	//場所
	worldTransform_.translation_ = {0, 0, 0};
	//大きさ
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection_) {
	// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}

