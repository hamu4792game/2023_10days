#pragma once


#include "Engine/Texture/Texture2D.h"

class Score;

class GameScene;

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

	void SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int textureName);

	void SetColor(uint32_t color, int textureName) { colors_[textureName] = color; }

	void SetIsDraw(bool is, int textureName) { isDraw_[textureName] = false; }

	void Update();
	
	void Draw2D(const Matrix4x4& viewProjectionMat);

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void ResetIsDraw();

	void TitleInitialize();

	void TitleUpdate();

	void BattleInitialize();

	void BattleUpdate();

	void ResultInitialize();

	void ResultUpdate();

	void DrawUITextures(const Matrix4x4& viewProjectionMat);

	void DrawUITexture(const Matrix4x4& viewProjectionMat, int textureName);

private:
	

	std::vector<std::shared_ptr<Texture2D>> UITextures_;

	std::vector<std::shared_ptr<WorldTransform>> worldTransforms_;

	Score* score_ = nullptr;

	uint32_t colors_[kUITexturesMaxNum_] = {};

	bool isDraw_[kUITexturesMaxNum_] = {};

};
