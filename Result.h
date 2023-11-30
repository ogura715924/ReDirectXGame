#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Score.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Scene.h>



class Result {
public:
	Result();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Result();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	///< summary>
	/// 描画
	/// </summary>
	void Draw();

	void GCOnColision();
	void GOOnColision();

	bool IsSceneEnd() { return isSceneEnd_; }
	Scene::SceneType NextScene() { return Scene::SceneType::kTitle; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	bool isSceneEnd_ = false;

	uint32_t soundDataHandle_ = 0;

	// 画像
	uint32_t textureHandleGO_ = 0;
	Sprite* spriteGO_ = nullptr;
	uint32_t textureHandleGC_ = 0;
	Sprite* spriteGC_ = nullptr;
};