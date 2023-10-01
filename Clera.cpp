#include "Clera.h"
#include <TextureManager.h>

Clera::Clera() {}

Clera::~Clera() {}

void Clera::Initialize() {
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("skyC.png");
}

void Clera::Update() {}

void Clera::Draw(uint32_t textureReticle) { 
	sprite2DReticle_->Draw(textureReticle) }
