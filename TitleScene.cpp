#include "TitleScene.h"
#include <TextureManager.h>

 TitleScene::TitleScene() {}

 TitleScene::~TitleScene() {}

void TitleScene::Initialize() {

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("skyT.png");
}

 void TitleScene::Update() {
	
 }

void TitleScene::Draw() {}
