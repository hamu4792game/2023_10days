#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Player/Player.h"



class Battle
{
public:
	Battle(std::shared_ptr<Camera> camera = nullptr);
	~Battle() = default;

	//	初期化
	void Initialize();

	//	モデルのロード
	void ModelLoad();

	//	更新処理
	void Update();

	//	3D描画処理
	void Draw(const Matrix4x4& viewProjection);

private://	必要なメンバ変数

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;

  //	プレイヤーデータのモデル配列
	std::vector<std::shared_ptr<Model>> mobModels_;

	//	パーツ用ペアレントデータ
	std::vector<WorldTransform> mobarts_;
  
  //	中心座標
	WorldTransform transform;

};
