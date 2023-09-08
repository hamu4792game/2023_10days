#pragma once


#include "Engine/Texture/Texture2D.h"

class Score;

class UI {

public:

	enum UITextureNames {
		kScore,
		kHighScore,
		kCombo,
		kHighCombo,
		kPerfect,
		kGreat,
		kGood,
		kMiss,
	};

	static const int kUITexturesMaxNum_ = 8;

public:

	UI();

	void Initialize();

	void SetScore(Score* score) { score_ = score; }

	void SetUITexture(std::vector<std::shared_ptr<Texture2D>> UITextures) { UITextures_ = UITextures; }

	// posは一番左の数字の真ん中の座標。
	void SetWorldTransform(const Vector2& screenPos, float scale, float rotate, UITextureNames textureName);

	void SetColor(uint32_t color, UITextureNames textureName) { colors_[textureName] = color; }

	void Update();

	void DrawUITexture(const Matrix4x4& viewProjectionMat, UITextureNames textureName);
	
	void Draw2D(const Matrix4x4& viewProjectionMat);

private:
	

	std::vector<std::shared_ptr<Texture2D>> UITextures_;

	std::vector<std::shared_ptr<WorldTransform>> worldTransforms_;

	Score* score_ = nullptr;

	uint32_t colors_[kUITexturesMaxNum_] = {};

};
