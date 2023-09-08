#pragma once
#include "math/Matrix4x4.h"
#include <vector>
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"

class Title
{
public:
	Title() = default;
	~Title() = default;

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjection);
	void Draw2D(Matrix4x4 viewProjection2d);
public: // setter
	void SetModels(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_ = modeldate; };
	void SetModelsType2(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_type2 = modeldate; };
	void SetBottonModels(std::vector<std::shared_ptr<Model>> modeldate) { bottonModels_ = modeldate; };

private: // シーンで必要なモデル配列
	//	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;

	std::vector<std::shared_ptr<Model>> bottonModels_;
	//	
	std::vector<std::shared_ptr<Texture2D>> numberTextures_;


};
