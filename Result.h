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



class Result {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	int32_t MeterCount = 0;
	Score* score_ = nullptr;

	// リザルト
	uint32_t TextureHandle_[11]{};

	// スコアの描画
	Sprite* sprite0 = nullptr;
	Sprite* sprite1 = nullptr;
	Sprite* sprite2 = nullptr;
	Sprite* sprite3 = nullptr;
	Sprite* sprite4 = nullptr;

	// スコア変動
	int32_t eachNumber[4]{};


};