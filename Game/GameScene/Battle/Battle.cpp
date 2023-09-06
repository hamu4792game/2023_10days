#include "Battle.h"
#include "externals/imgui/imgui.h"
#include "math/Vector4.h"

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	player_ = std::make_unique<Player>(camera_);

	score_ = std::make_unique<Score>();

	//プレイヤーモデルの初期化
	for (uint16_t i = 0u; i < PARTS::Num; i++) {
		mobModels_.push_back(std::make_shared<Model>());
	}
  
	mobparts_.resize(mobModels_.size());


	worldTransform = std::make_shared<WorldTransform>();
}

Battle::~Battle() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void Battle::Initialize()
{
	//	カメラの設定
	camera_->transform.translation_ = Vector3(0.0f, 16.5f, -21.7f);
	camera_->transform.rotation_.x = 0.471f;


	//EnemyReset();

	//EnemyGeneration();

	score_->Reset();

	player_->Initialize(mobModels_, worldTransform.get());

}

void Battle::ModelLoad()
{
	//頭・身体・腰
	mobModels_[Body]->Texture("Resources/player/Parts/pBody/pBody.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[Head]->Texture("Resources/player/Parts/pHead/pHead.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[BodyUnder]->Texture("Resources/player/Parts/pBodyUnder/pBodyUnder.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左腕１・左腕２・左手
	mobModels_[LArm1]->Texture("Resources/player/Parts/pLArm1/pLArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LArm2]->Texture("Resources/player/Parts/pLArm2/pLArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LHand]->Texture("Resources/player/Parts/pLHand/pLHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右腕１・右腕２・右手
	mobModels_[RArm1]->Texture("Resources/player/Parts/pRArm1/pRArm1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RArm2]->Texture("Resources/player/Parts/pRArm2/pRArm2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RHand]->Texture("Resources/player/Parts/pRHand/pRHand.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//左足首１・左足首２・左足
	mobModels_[LLeg1]->Texture("Resources/player/Parts/pLLeg1/pLLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LLeg2]->Texture("Resources/player/Parts/pLLeg/pLLeg.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[LFoot]->Texture("Resources/player/Parts/pLFoot/pLFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	//右足首１・右足首２・右足
	mobModels_[RLeg1]->Texture("Resources/player/Parts/pRLeg1/pRLeg1.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RLeg2]->Texture("Resources/player/Parts/pRLeg2/pRLeg2.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	mobModels_[RFoot]->Texture("Resources/player/Parts/pRFoot/pRFoot.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");

	//player_->ModelLoad();
	
	//削除済
#pragma region スペチャ：：//パーツの基本的な親子関係を作成
	/*
	//Bodyの親子関係のみ呼び出しクラス内で作成
	mobparts_[Head].parent_ = &mobparts_[Body];
	mobparts_[BodyUnder].parent_ = &mobparts_[Body];

	mobparts_[LArm1].parent_ = &mobparts_[Body];
	mobparts_[LArm2].parent_ = &mobparts_[LArm1];
	mobparts_[LHand].parent_ = &mobparts_[LArm2];

	mobparts_[RArm1].parent_ = &mobparts_[Body];
	mobparts_[RArm2].parent_ = &mobparts_[RArm1];
	mobparts_[RHand].parent_ = &mobparts_[RArm2];

	mobparts_[LLeg1].parent_ = &mobparts_[BodyUnder];
	mobparts_[LLeg2].parent_ = &mobparts_[LLeg1];
	mobparts_[LFoot].parent_ = &mobparts_[LLeg2];

	mobparts_[RLeg1].parent_ = &mobparts_[BodyUnder];
	mobparts_[RLeg2].parent_ = &mobparts_[RLeg1];
	mobparts_[RFoot].parent_ = &mobparts_[RLeg2];

	//座標設定
	mobparts_[Body].translation_ = { 0, 0, 0 };
	mobparts_[BodyUnder].translation_ = { 0, 0, 0 };
	mobparts_[Head].translation_ = { 0, 2.6f, 0 };

	mobparts_[LArm1].translation_ = { -0.8f, 1.57f, 0 };
	mobparts_[LArm2].translation_ = { -1.73f, 0, 0 };
	mobparts_[LHand].translation_ = { -2.37f, 0, 0 };

	mobparts_[RArm1].translation_ = { 0.8f, 1.57f, 0 };
	mobparts_[RArm2].translation_ = { 1.73f, 0, 0 };
	mobparts_[RHand].translation_ = { 2.37f, 0, 0 };

	mobparts_[LLeg1].translation_ = { -0.3f, -1.7f, 0 };
	mobparts_[LLeg2].translation_ = { 0, -2.2f, 0 };
	mobparts_[LFoot].translation_ = { -0.12f, -2.2f, 0 };

	mobparts_[RLeg1].translation_ = { 0.3f, -1.7f, 0 };
	mobparts_[RLeg2].translation_ = { 0, -2.2f, 0 };
	mobparts_[RFoot].translation_ = { 0.12f, -2.2f, 0 };
	*/
#pragma endregion 
	

}

void Battle::EnemyGeneration() {

	while (enemyNum_ - enemyKillCount_ < kEnemyIntervalNum_) {

		if (enemyNum_ == kEnemyMaxNum_) {
			break;
		}
		else {
			Enemy* enemy = new Enemy();

			Vector3 pos = { 0,0,kEnemyPopPosLength_ * (enemyNum_ + 1) };

			int type = rand() % 4;

			if (type == preEnemyType_) {

				typeCount_++;
				if (typeCount_ == 4) {

					while (type == preEnemyType_)
					{
						type = rand() % 4;
						if (type != preEnemyType_) {
							break;
						}
					}

					typeCount_ = 0;
				}
			}
			else {
				typeCount_ = 0;
			}

			// Initializeを変える必要がある
			enemy->Initialize(type, enemyNum_);

			enemies_.push_back(enemy);

			enemyNum_++;

			preEnemyType_ = type;
		}
	}

}

void Battle::EnemyReset() {
	for (Enemy* enemy : enemies_) {
		enemy->Die();
	}
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	enemyNum_ = 0;
	enemyKillCount_ = 0;
	
	preEnemyType_ = 0;
	typeCount_ = 0;
}

void Battle::Update()
{
	//	カメラの調整
	ImGui::DragFloat3("cameratr", &camera_->transform.translation_.x, 0.1f);
	ImGui::DragFloat3("cameraro", &camera_->transform.rotation_.x, AngleToRadian(1.0f));

	for(Enemy* enemy : enemies_){
		if (enemy->GetNum() == enemyKillCount_) {
			player_->HitTest(enemy, score_.get());
		}

		enemy->Update();
	}
	
	// 評価
	if (score_->GetEvaluation()) {
		EnemyGeneration();
	}

	//player_->Update();

	//ImGui::DragFloat("worldRo", &transform.rotation_.x, AngleToRadian(1.0f));
	//	行列の更新　回転行列のみ必要なためUpdateはしていない
	player_->Update();
	worldTransform->worldMatrix = MakeRotateMatrix(worldTransform->rotation_);

}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	player_->Draw(viewProjection);
}
