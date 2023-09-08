#pragma once
#include "math/Matrix4x4.h"
#include <vector>
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Camera/Camera.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"

class Title
{
public:
	Title(std::shared_ptr<Camera> camera = nullptr);
	~Title() = default;

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjection);
	void Draw2D(Matrix4x4 viewProjection2d);
public: // setter
	void SetModels(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_ = modeldate; }
	void SetModelsType2(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_type2 = modeldate; }
	void SetBottonModels(std::vector<std::shared_ptr<Model>> modeldate) { bottonModels_ = modeldate; }

private: // シーンで必要なモデル配列
	//	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;
	std::vector<std::shared_ptr<Model>> mobModels_type2;

	std::vector<std::shared_ptr<Model>> bottonModels_;
	
private:
	//	中心座標
	WorldTransform worldTransform;
	

	//struct Chara {
	//	//	キャラの座標
	//	WorldTransform character;
	//	//	モデルデータ配列
	//	std::vector<WorldTransform> parts_;
	//};
	//Chara player_;
	//std::vector<Chara> enemy_;

	std::vector<std::unique_ptr<Enemy>> enemys_;

	//	カメラのポインタ
	std::shared_ptr<Camera> camera_ = nullptr;


private: // メンバ関数
	//	パーツの初期位置をセット
	void SetParts();

};
