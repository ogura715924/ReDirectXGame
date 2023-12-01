#include "RailCamera.h"
#include <MyMath.h>

void RailCamera::Initialize(Vector3 pos, Vector3 rotation) {

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_.x = pos.x;
	worldTransform_.translation_.y = pos.y;
	worldTransform_.translation_.z = pos.z;

	worldTransform_.rotation_.x = rotation.x;
	worldTransform_.rotation_.y = rotation.y;
	worldTransform_.rotation_.z = rotation.z;

	//ビュープロジェクションの初期化
	//viewProjection_.farZ = 0.1f;
	viewProjection_.Initialize();
}

void RailCamera::Update() {

//ワールドトランスフォーム座標の数値を加算したりする(移動)
	//worldTransform_.translation_.z += 0.1f;

	//ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//ワールドトランスフォームのワールド行列再計算
	

	 //カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);


	// カメラの座標を画面表示する処理
	//ImGui::Begin("Camera");
	////スライダーでカメラのtranslationを表示
	//ImGui::SliderFloat3("Position", &worldTransform_.translation_.x, -18.0f, 34.0f);
	////スライダーでカメラのrotationを表示
	//ImGui::SliderFloat3("Rotate", &worldTransform_.rotation_.x, -18.0f, 34.0f);
	//ImGui::End();
}
