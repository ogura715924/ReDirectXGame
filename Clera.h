#pragma once
#include "Scene.h"
#include <Audio.h>
#include <DirectXCommon.h>
#include <Input.h>
#include <Sprite.h>

class Clera {
public:
	// コンストラクタ
	Clera();
	// デストラクタ
	~Clera();
	// 初期化
	void Initialize();
	// マイフレームの処理
	void Update();
	// 描画
	void Draw(const ViewProjection& viewProjection_);

	SceneType NextScene() { return SceneType::kGaamePlay; };
	bool IsSceneEnd() { return isSceneEnd; }

private: // メンバ変数
	bool isSceneEnd = false;
	DirectXCommon* dxCommon = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};
