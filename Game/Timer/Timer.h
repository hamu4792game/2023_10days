#pragma once

#include "Engine/Texture/Texture2D.h"

class Timer {

public:


public:

	Timer();

	void Initialize();

	void Update();

	int GetTime() { return time_; }

	void SetNumberTexture(std::vector<std::shared_ptr<Texture2D>> numberTextures) { numberTextures_ = numberTextures; }

	// posは一番左の数字の真ん中の座標。
	void SetWorldTransform(const Vector2& screenPos, float scale, float rotate);

	void SetColor(uint32_t color) { color_ = color; }

	void SetIsDraw(bool is) { isDraw_ = is; }

	void Draw2D(const Matrix4x4& viewProjectionMat);

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:

	// 最大桁数
	static const int kMaxDigits_ = 2;

	// textureのサイズ
	static const int kNumberTextureWidth_ = 53;

	static const int kMaxTime_ = 60;

private:

	int time_ = kMaxTime_;

	int countFrame_ = 0;
	
	uint32_t color_ = 0xFFFFFFFF;

	bool isDraw_ = true;

	float scale_ = 1.0f;

	Vector2 pos_ = { 640.0f,360.0f };

	std::vector<std::shared_ptr<Texture2D>> numberTextures_;

	std::vector<std::shared_ptr<WorldTransform>> worldTransforms_;


};
