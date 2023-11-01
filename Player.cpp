#include"Player.h"
#include "WorldTransform.h"
#include <assert.h>
#include <cassert>
#include <imgui.h>
#include <math.h>
#include "MyMath.h"
#include <WinApp.h>

#define _USE_MATH_DEFINES

// デストラクタ
Player::~Player() {

	// bullet_の解放

	for (PlayerBullet* bullet : bullets_) {
		// bullets -> PlayerBullet*のリスト(配列のすごい版)
		// 範囲forで何をやっているか
		// bullet = bullets_[i];をやっていてbullets_の数分だけループする
		delete bullet;

		//Sprite::Create()のところの解放
		delete sprite2DReticle_;
	}
}

void Player::OnCollision() {
//何もしない
}

Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	worldTransform_.matWorld_.m;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

int Player::GetRadius() { return int(); }

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;

}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 PlayerPostion) {
	// 引数から受け取ったモデルが組み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;
	textureHandle_ = textureHandle;

	// X,Y,Z方向のスケーリングを設定
	/*worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};*/

	worldTransform_.translation_ = PlayerPostion;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 発射間隔初期化
	Interval();

	// 3Dレティクルのワールドトラベル初期化
	worldTransform3DReticle_.Initialize();

		// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}


void Player::Update(const ViewProjection& viewProjection) {

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向でベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向でベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// 移動限界座標
	const float kMoveLimitX = 1280;
	const float kMoveLimitY = 720;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	
	// 行列更新
	
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	/*ImGui::Begin("Debug");
	float playerpos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", playerpos, 0, 1280);
	worldTransform_.translation_.x = playerpos[0];
	worldTransform_.translation_.y = playerpos[1];
	worldTransform_.translation_.z = playerpos[2];

	ImGui::End();*/

	// キャラクター弾攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// 発射タイマーカウントダウン
	AttackTimer--;
	// 指定した時間に達した
	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {
		// 発射タイマーを戻す
		AttackTimer = kAttackInterval;
	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 0.0f;
		// 自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset);
		offset.x *= kDistancePlayerTo3DReticle;
		offset.y *= kDistancePlayerTo3DReticle;
		offset.z *= kDistancePlayerTo3DReticle;
		// 3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
		worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
		worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;

		worldTransform3DReticle_.UpdateMatrix();
	}


	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		 //3Dレティクルのワールド行列から、ワールド座標を取得

		 Vector3 positionReticle = GetWorldPosition();

		 // ビューポート行列
		  Matrix4x4 matViewport =
		      MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		 // ビュー行列とプロジェクション行列、ビューポート行列を合成する
		  Matrix4x4 matViewProjectionViewport =
		      Multiply(Multiply(viewProjection.matView, viewProjection.matProjection),
		      matViewport);

		 // ワールドスクリーン座標変換
		  positionReticle = Transform(positionReticle, matViewProjectionViewport);

		 // スプライトのレティクルに座標設定
		  sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}


	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{ POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
		  GetCursorPos(&mousePosition);

		  //クライアントエリア座標に変換する
		  HWND hwnd = WinApp::GetInstance()->GetHwnd();
		  // スプライトのレティクルに座標設定
		   sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));
		  ScreenToClient(hwnd, &mousePosition);
		  //マウス座標を2Dレティクルのスプライトに代入する
		  Matrix4x4 matViewport =MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		  //ビュープロジェクションビューポート合成行列
		  Matrix4x4 matVPV =Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
		  //合成行列の逆行列を計算する
		  Matrix4x4 matInverseVPV = Inverse(matVPV);
		  //スクリーン座標
		  Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
		  Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

		  //スクリーン座標系からワールド座標系へ
		  posNear = Transform(posNear, matInverseVPV);
		  posFar = Transform(posFar, matInverseVPV);

		  // マウスレイの方向
		  Vector3 mouseDirection = {
		      posFar.x - posNear.x,
		      posFar.y - posNear.y,
		      posFar.z - posNear.z 
		  };

		  mouseDirection = Normalize(mouseDirection);

		  // カメラから照準オブジェクトの距離
		  const float kDistanceTestObject = 50.0f;
		  worldTransform3DReticle_.translation_.x =
		      (mouseDirection.x * kDistanceTestObject) + posNear.x;
		  worldTransform3DReticle_.translation_.y =
		      (mouseDirection.y * kDistanceTestObject) + posNear.y;
		  worldTransform3DReticle_.translation_.z =
		      (mouseDirection.z * kDistanceTestObject) + posNear.z;

		  worldTransform3DReticle_.UpdateMatrix();
			

		  //ImGui::Begin("Player");
		  // ImGui::Text("2DReticle:(%f,%f)", mousePosition.x, mousePosition.y);
		  // ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y);
		  // ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y);
		  // ImGui::Text(
		  //     "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		  //     worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

		  // ImGui::Text(
		  //     " player : (%+.2f,%+.2f,%+.2f)", worldTransform_.translation_.x,
		  //     worldTransform_.translation_.y, worldTransform_.translation_.z);
		  // ImGui::End();
	}
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	// 弾を生成し、初期化

	if (input_->PushKey(DIK_SPACE) && AttackTimer <= 0) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;
	}
}

void Player::Interval() {
	// 発射タイマーを初期化
	AttackTimer = 3;
}

void Player::Draw(const ViewProjection& ViewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, ViewProjection, textureHandle_);

	// 3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, ViewProjection);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(ViewProjection);
	}

	
}

void Player::DrawUI() {
	// スプライト生成
	sprite2DReticle_->Draw();
}


