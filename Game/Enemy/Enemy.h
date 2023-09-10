#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include"EEnum.h"


enum class MODE_A {
	WAVE1,
	WAVE2,
	WAVE3,
	WAVE4,

	NOMOTIAN,
};

enum class blow {
	DAISHA,
	FIGURA,

	NUM,
};


class Enemy
{
public:

	enum class BottomTypeClass {
		kA,
		kB,
		kX,
		kY
	};

	enum BottomType {
		kA,
		kB,
		kX,
		kY
	};

public:
	Enemy() = default;
	~Enemy() = default;

	void Initialize();

	void ModelLoad();

	//	更新処理
	void Update();

	//	描画
	void Draw(const Matrix4x4& viewProjection, std::vector<std::shared_ptr<Model>> botunModels);
	void Draw(const Matrix4x4& viewProjection);

public: // Korone

	void Initialize(int type, int num);

	//修正版
	void InitializeSP(float pos, int type, int num,
		std::vector<std::shared_ptr<Model>> models);

	int type_;

	BottomTypeClass GetBottomType() { return bottomType_; }

	int GetNum() { return num_; }

	void Die(bool left);

	bool IsDead() { return isDead_; }

private:

	
	//	元
	WorldTransform transform;

	//	モデルデータ配列
	std::vector<std::shared_ptr<Model>> models_;
	//	パーツ用データ
	std::vector<WorldTransform> parts_;


	//ボタンのワールド
	WorldTransform BottonW_;


private: // Korone
	BottomTypeClass bottomType_ = BottomTypeClass::kA;

	int num_ = 0;

	bool isDead_ = false;



private://Specha

	//アニメーション開始前初期化できているか
	bool isStart_blow_away = false;
	//飛ぶ時のベクトル
	Vector3 blowVec;
	//アニメの状態
	MODE_A animeState_;

	//敵のアニメーション処理
	void BlowAway();

	//アニメーションに関する初期化処理
	void AnimeInitialize();

	//WAVEごとのアニメーションの初期化処理
	bool SetAnimeStart = false;

	//変数T
	float T_;
	//変数Tに足す1fごとの数値
	float AddT_=1.0f/60.0f;

	//飛ぶ方向右,0,左１
	bool checkDirection_;

	//実際のイージングで使う構造体
	std::vector<esing> ESALL;

	//現在の回転軸の保存先
	std::vector<Vector3>nowR;
	
	//飛ばす方向を保存
	bool dire;

	//保存
	int ANIMETYPE;

	//吹っ飛びアニメ数
	static const int animeNUM = 4;

	//アニメーションの引数まとめ
	std::vector<esing> sprawled[animeNUM];

	//[3]の右バージョン
	std::vector<esing> anoMotion;
};


//イージングの処理、好きに中身かえてちょ
Vector3 ES(esing E, float t);
