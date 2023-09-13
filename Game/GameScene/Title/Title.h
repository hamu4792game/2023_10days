#pragma once
#include "math/Matrix4x4.h"
#include <vector>
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Input/AudioInput/AudioInput.h"

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
	void SetModels(std::vector<std::vector<std::shared_ptr<Model>>> modeldate) { mobModels_ = modeldate; }
	//void SetModelsType2(std::vector<std::shared_ptr<Model>> modeldate) { mobModels_type2 = modeldate; }
	void SetBottonModels(std::vector<std::shared_ptr<Model>> modeldate) { bottonModels_ = modeldate; }
	void SetShopModel(std::vector<std::shared_ptr<Model>> modeldate) { shopModels_ = modeldate; }
	void SetTentyoModel(std::vector<std::shared_ptr<Model>> modeldate) { tentyoModels_ = modeldate; }

private: // シーンで必要なモデル配列
	//	プレイヤーデータのモデル配列
	//std::vector<std::shared_ptr<Model>> mobModels_;
	//std::vector<std::shared_ptr<Model>> mobModels_type2;

	std::vector<std::vector<std::shared_ptr<Model>>> mobModels_;

	std::vector<std::shared_ptr<Model>> bottonModels_;

	std::vector<std::shared_ptr<Model>> shopModels_;

	std::vector<std::shared_ptr<Model>> tentyoModels_;

	
private:
	//	中心座標
	WorldTransform worldTransform;
	//	キャラ用
	WorldTransform chara;

	std::vector<WorldTransform> tentyoTransform;

	//	モデルデータ配列
	std::vector<WorldTransform> parts_;
	std::unique_ptr<Enemy> player;
	std::vector<WorldTransform> shopTrans;

	std::vector<std::unique_ptr<Enemy>> enemy_;

	//	カメラのポインタ
	std::shared_ptr<Camera> camera_ = nullptr;

	enum CAMERASTEP {
		Zero,
		First,
		Second,
		Bounce,
		BounceFace,
		FirstReturn,
	};
	CAMERASTEP cameraStep = CAMERASTEP::Zero;

	bool stepFlag = false;
	Vector3 startingPoint;
	Vector3 startingRotate;
	Vector3 endPoint;
	Vector3 endRotate;

	int easeNowFrame;
	int easeMaxFrame;


	WorldTransform pushAtrans_;
	std::unique_ptr<Texture2D> pushAtext_;


private: // 音
	AudioInput bgm;
	AudioInput sDawnsound;
	AudioInput sAppearance;

private: // メンバ関数
	//	パーツの初期位置をセット
	void SetParts();

	void CameraMove();

	//店長T
	WorldTransform TenchoW_;
	//王騎将軍ポーズ
	std::vector<esing> okisyogun;
	void OkiSyogun();
	bool isL_ = false;
	bool isOkiStart_ = false;

	float OkiT_ = 0;

	//店長座標
	Vector3 Tpos[4];
	Vector3 Trota[4];
};
