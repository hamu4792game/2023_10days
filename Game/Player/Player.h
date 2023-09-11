#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include"EEnum.h"
#include "Engine/Texture/Texture2D.h"

class Score;

class Enemy;



class Player
{
public:
	Player(std::shared_ptr<Camera> camera = nullptr);
	~Player() = default;

	//モデルデータ配列,パーツWのデータ配列、親の処理
	void Initialize(std::vector<std::shared_ptr<Model>> models,WorldTransform* world);

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection);

public: // Korone

	enum GaugeResource {
		kBackResource,
		kGaugeResource,
	};

	static const int kGaugeResourceNum_ = 2;

	enum GaugeDrawEnum {
		kBack,
		kGaugeBack,
		kGaugePerfect,
		kGaugeGreat,
		kGaugeGood
	};

	static const int kGaugeDrawNum_ = 5;

	void SetGaugeTextures(std::vector<std::shared_ptr<Texture2D>> textures) { gaugeTextures_ = textures; }

	void SetScore(Score* score) { score_ = score; }

	void HitTestInitialize();

	//void SetScore(Score* score) { score = score; }

	void HitTest(Enemy* enemy);


	void GaugeInitialize();

	void GaugeUpdate();

	void GaugeDraw2D(const Matrix4x4& viewProjection);

private: // Korone

	//void HitTest(Enemy* enemy, Score* score);

	void SetGaugeWorldTransform(const Vector2& screenPos, const Vector2& scale, float rotate, int gaugeEnumName);

	void SetFloatTransform();

	void SetColor(uint32_t color, int gaugeEnumName) { gaugeColors_[gaugeEnumName] = color; }

	void SetIsDraw(bool is, int gaugeEnumName) { gaugeIsDraw_[gaugeEnumName] = false; }

	void GaugeDraw(const Matrix4x4& viewProjection, int gaugeEnumName);

	void HitEvalution(Enemy* enemy);

	void SetKoroneGlobalVariable();

	void ApplyKoroneGlobalVariable();

private:

	
	//	世界の中心のptr
	WorldTransform* world_ = nullptr;
	
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;
	//	カメラ共有ptr
	std::shared_ptr<Camera> camera_ = nullptr;
	//	カメラの初期位置
	Vector3 offset;

	//	移動処理用のフラグ
	bool flag = false;

	//	移動座標 (敵の位置)
	float movePos = 0.0f;
	//	移動前の座標（ease用始点）
	float oldPos = 0.0f;
	//	客の間隔
	float enemyDistance;
	//	加算していくフレーム
	float frame;
	float waitFrame;
	//	最大フレーム
	float MAX_frame;

public:
	void SetDistance(const float& dis) { enemyDistance = dis; }

private:

	//	移動処理
	void Move();
	//	tyoe2
	void MoveType2();

private: // Korone

	enum Evalution {
		kPerfect,
		kGreat,
		kGood,
		kMiss
	};

	static const int kGaugeTransformNum_ = 3;

	Score* score_ = nullptr;

	//Input* input_ = nullptr;

	// 一番遅いフレーム。これを基準にコンボ数によって早くする。
	const int kIntervalFrame_ = 30;

	int intervalCount_ = 0;

	int kEvalutionframe_[4] = { 20, 60, 120, kEvalutionframe_[Evalution::kGood] + 1};

	int evalutionCount_ = 0;

	//Score* score = nullptr;

	std::vector<std::shared_ptr<Texture2D>> gaugeTextures_;

	std::vector<std::shared_ptr<WorldTransform>> gaugeWorldTransforms_;

	const float kTextureSize_ = 4.0f;

	float kBaseScale_ = 60.0f;

	float kBaseScaleY_ = 18.0f;

	float kBaseWhiteSpace_ = 4.0f;

	float kGaugeScale_[kGaugeTransformNum_] = {
		kBaseScale_ * kEvalutionframe_[Evalution::kPerfect] / kEvalutionframe_[Evalution::kGood],
		kBaseScale_ * (kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect]) / kEvalutionframe_[Evalution::kGood],
		kBaseScale_ * (kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat]) / kEvalutionframe_[Evalution::kGood]
	};

	Vector2 kBasePos_ = { float(WinApp::kWindowWidth) / 2.0f, 600.0f };

	float kGaugeStartPos_[kGaugeTransformNum_] = {
		kBasePos_.x - kTextureSize_ / 2 * kBaseScale_ + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect],
		kGaugeStartPos_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat],
		kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood],
	};

	float kGaugeEndPos_[kGaugeTransformNum_] = {
		kGaugeStartPos_[Evalution::kPerfect] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kPerfect],
		kGaugeStartPos_[Evalution::kGreat] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGreat],
		kGaugeStartPos_[Evalution::kGood] + kTextureSize_ / 2 * kGaugeScale_[Evalution::kGood]
	};

	/*float kGaugePerfectScale_ = kBaseScale_ * kEvalutionframe_[Evalution::kPerfect] / kEvalutionframe_[Evalution::kGood];

	float kGaugeGreatScale_ = kBaseScale_ * (kEvalutionframe_[Evalution::kGreat] - kEvalutionframe_[Evalution::kPerfect]) / kEvalutionframe_[Evalution::kGood];

	float kGaugeGoodScale_ = kBaseScale_ * (kEvalutionframe_[Evalution::kGood] - kEvalutionframe_[Evalution::kGreat]) / kEvalutionframe_[Evalution::kGood];*/

	uint32_t gaugeColors_[kGaugeDrawNum_] = {};

	bool gaugeIsDraw_[kGaugeDrawNum_] = {};


private: //Spe

	enum ANIMETYPE {
		Normal,
		ATK_R,
		ATK_L,

		A_NUM,
	};

	//アニメーション状態、ここを変えるとそれぞれアニメーションが始まる
	ANIMETYPE state_;

	//行動管理
	enum ANIMEWAVE {
		ATKWAIT,//攻撃予備動作
		ATK,
		BACK,
	};

	//アニメーションモード、基本アニメーション関数内で変更
	ANIMEWAVE wave_A;

	//アニメーション初期化
	void AnimeInitialize();

	//アニメーションアップデート
	void Animetion();

	//右攻撃まとめ
	void ATK_R_F();

	//

	//イージング
	float T_;
	//アニメーションフレーム加算地
	float AddT_ = 1.0f / 30.0f;

	//アニメーション開始時の初期化関数
	bool isAnimeStart_;

	//実際のイージングで使う構造体
	std::vector<esing> ESALL;

	//現在の回転軸の保存先
	std::vector<Vector3>nowR;

	void GetplayerR();
	//大の字
	std::vector<esing> AnimeType[A_NUM];

};
