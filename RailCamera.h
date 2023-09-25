#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>

class RailCamera {

	public:
		//初期化
	    void Initialize(Vector3 pos, Vector3 rotation);
		//更新
	    void Update();

		// プレイヤーワールド座標を取得
	    const WorldTransform& GetWorldTransform() { return worldTransform_; }

		private:
	    // ワールド変換データ
	    WorldTransform worldTransform_;
	    // ビュープロジェクション
	    ViewProjection viewProjection_;
};
