#include "UI.h"

#include "Game/Score/Score.h"

UI::UI() {

	for (int i = 0; i < kUITexturesMaxNum_; i++) {
		worldTransforms_.push_back(std::make_shared<WorldTransform>());

		colors_[i] = 0xFFFFFFFF;
	}

}

void UI::Initialize() {



}

void UI::SetWorldTransform(const Vector2& screenPos, float scale, float rotate, UITextureNames textureName) {

	Vector2 pos = { screenPos.x - WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - screenPos.y };

	worldTransforms_[textureName].get()->translation_.x = pos.x;
	worldTransforms_[textureName].get()->translation_.y = pos.y;

	worldTransforms_[textureName].get()->rotation_.z = rotate;

	worldTransforms_[textureName].get()->scale_.x = scale;
	worldTransforms_[textureName].get()->scale_.y = scale;

	worldTransforms_[textureName].get()->UpdateMatrix();
}

void UI::Update() {

}

void UI::DrawUITexture(const Matrix4x4& viewProjectionMat, UITextureNames textureName) {

	Texture2D::TextureDraw(*(worldTransforms_[textureName].get()), viewProjectionMat, colors_[textureName], UITextures_[textureName].get());
}

void UI::Draw2D(const Matrix4x4& viewProjectionMat) {

}
