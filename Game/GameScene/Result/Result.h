#pragma once
#include "math/Matrix4x4.h"
#include "Game/UI/UI.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "EEnum.h"
#include "Engine/Input/AudioInput/AudioInput.h"

class Result
{
public:
	Result(std::shared_ptr<Camera> camera = nullptr);
	~Result() = default;

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjection);

	void Draw2D(const Matrix4x4& viewProjection);

public: // Modelデータのロード
	void SetModels(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_ = modeldate; }
	void SetShopModel(std::vector<std::shared_ptr<Model>> modeldate) { shopModels_ = modeldate; }
	void SetTentyoModel(std::vector<std::shared_ptr<Model>> modeldate) { tentyoModels_ = modeldate; }

private: // 音関係
	//AudioInput pon;

private: // Modelの宣言

	std::vector<std::shared_ptr<Model>> mobModels_;

	std::vector<std::shared_ptr<Model>> shopModels_;

	std::vector<std::shared_ptr<Model>> tentyoModels_;

private:

	std::vector<WorldTransform> parts_;
	
	std::vector<WorldTransform> shopTransform_;

	std::vector<WorldTransform> tentyoTransform_;

	//プレイヤーの基本W
	WorldTransform playerW_;

	//店長の基本W
	WorldTransform tentyoW_;

	WorldTransform cameraR_target_;

	std::shared_ptr<Camera> camera_;


	//ラーメン食べるアニメーション
	void EatRamen();
	bool isLoop_ = false;
	bool isAnimeStart_ = false;

	float T_ = 0;

	//ラーメン食う！！
	std::vector<esing> eatRamen;

	//王騎将軍ポーズ
	std::vector<esing> okisyogun;

	//悔しいです！！！！！！！！！
	std::vector<esing> kuyasii;
	void Kuyasii();

	void OkiSyogun();
	bool isL_ = false;
	bool isOkiStart_=false;

	bool* clearFlag_ = nullptr;

	float OkiT_ = 0;
public: // korone

	void SetUI(UI* ui) { ui_ = ui; }

	void SetClearFlag_(bool* flag) { clearFlag_ = flag; }

private:

	UI* ui_ = nullptr;

};
