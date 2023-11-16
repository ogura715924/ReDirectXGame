#include "Result.h"

Result::Result() {}

Result::~Result(){

};

void Result::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
}

void Result::Update() {}

void Result::Draw() {
	
}