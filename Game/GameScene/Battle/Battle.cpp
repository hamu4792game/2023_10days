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
  
	mobarts_.resize(mobModels_.size());
}

Battle::~Battle() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void Battle::Initialize()
{

	//EnemyReset();

	//EnemyGeneration();

	score_->Reset();

	player_->Initialize(mobModels_, transform);

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

	player_->Update();

	transform.rotation_.x += AngleToRadian(1.0f);
	//ImGui::DragFloat("worldRo", &transform.rotation_.x, AngleToRadian(1.0f));
	transform.worldMatrix = MakeRotateMatrix(transform.rotation_);
	player_->Update();

}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	player_->Draw(viewProjection);
}
