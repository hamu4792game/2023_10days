#pragma once


#include "Engine/Texture/Texture2D.h"
#include "Engine/Input/AudioInput/AudioInput.h"

class Score;

class GameScene;

class UI {

public:

	enum UITextureNames {
		kBack,
		kScore,
		kHighScore,
		kCombo,
		kHighCombo,
		kPerfect,
		kGreat,
		kGood,
		kMiss,
	};

	static const int kUITexturesMaxNum_ = 9;

public:

	UI();

	void Initialize();

	void SetScore(Score* score) { score_ = score; }

	void SetUITexture(std::vector<std::shared_ptr<Texture2D>> UITextures) { uiTextures_ = UITextures; }

	void SetWorldTransform(const Vector2& screenPos, float scale, float rotate, int textureName);

	void SetColor(uint32_t color, int textureName) { colors_[textureName] = color; }

	void SetIsDraw(bool is, int textureName) { isDraw_[textureName] = false; }

	void Update();
	
	void Draw2D(const Matrix4x4& viewProjectionMat);

	void SetClearFlag(bool* flag) { clearFlag_ = flag; }

private:

	void EvaluationUpdate();

	void ComboUpdate();

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void ResetIsDraw();

	void SetAllTransform(int scene);

	void TitleInitialize();

	void TitleUpdate();

	void BattleInitialize();

	void BattleUpdate();

	void ResultInitialize();

	void ResultUpdate();

	void DrawUITextures(const Matrix4x4& viewProjectionMat);

	void DrawUITexture(const Matrix4x4& viewProjectionMat, int textureName);

private:
	
	static const int kUseSceneNum_ = 3;

	// これはscoreクラスのstatic const int kParameterNum_と同じ数値にすること
	static const int kScoreNum_ = 9;

	enum Scene {
		kBattleScene,
		kResultScene,
		kResultScene2,
	};

	static const int kClassNum_ = 2;

	enum ClassEmum {
		kUIClass,
		kScoreClass,
	};

	static const int kUIParameterNum_ = 2;

	enum ParameterNum {
		kPos,
		kScale
	};

	const char* groupNames_[kUseSceneNum_][kClassNum_] = {

		// BattleScene
		{
		"UIBattleScene",
		"ScoreBattleScene",
		},
		// ResultScene
		{
		"UIResultScene",
		"ScoreResultScene",
		},
		{
		"UIResultScene2",
		"ScoreResultScene2",
		}
	};

	const char* uiItemNames_[kUIParameterNum_][kUITexturesMaxNum_] = {

		// Pos
		{
		"BackPos",
		"ScorePos",
		"HighScorePos",
		"ComboPos",
		"HighComboPos",
		"PerfectPos",
		"GreatPos",
		"GoodPos",
		"MissPos",
		},
		// Scale
		{
		"BackScale",
		"ScoreScale",
		"HighScoreScale",
		"ComboScale",
		"HighComboScale",
		"PerfectScale",
		"GreatScale",
		"GoodScale",
		"MissScale",
		}
	};

	const char* scoreItemNames_[kUIParameterNum_][kScoreNum_] = {

		// Pos
		{
		"ScorePos",
		"HighScorePos",
		"ComboPos",
		"HighComboPos",
		"TotalHighComboPos",
		"PerfectPos",
		"GreatPos",
		"GoodPos",
		"MissPos",
		},
		// Scale
		{
		"ScoreScale",
		"HighScoreScale",
		"ComboScale",
		"HighComboScale",
		"TotalHighComboScale",
		"PerfectScale",
		"GreatScale",
		"GoodScale",
		"MissScale",
		}
	};

	bool* clearFlag_ = nullptr;

	int count_ = 0;

	int comboCountFrame_ = 0;

	int kMaxFrameCombo_ = 30;

	int kInterval_ = 40;

	std::vector<std::shared_ptr<Texture2D>> uiTextures_;

	std::vector<std::shared_ptr<WorldTransform>> worldTransforms_;

	Vector2 uiPos_[kUseSceneNum_][kUITexturesMaxNum_] = {};

	float uiScales_[kUseSceneNum_][kUITexturesMaxNum_] = {};

	Score* score_ = nullptr;

	Vector2 scoreNumPos_[kUseSceneNum_][kScoreNum_] = {};

	float scoreNumScales_[kUseSceneNum_][kScoreNum_] = {};

	uint32_t colors_[kUITexturesMaxNum_] = {};

	bool isDraw_[kUITexturesMaxNum_] = {};

private: // 音
	AudioInput pon;

};
