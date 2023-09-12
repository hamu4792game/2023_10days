#include "Battle.h"
#include "externals/imgui/imgui.h"
#include "math/Vector4.h"
#include "Game/GameScene/GameScene.h"

// 確認のため追加 by.Korone
#include "Engine/Input/KeyInput/KeyInput.h"

decltype(Battle::masterSpeed) Battle::masterSpeed;

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	player_ = std::make_unique<Player>(camera_);


	score_ = std::make_unique<Score>();

	player_->SetScore(score_.get());

	worldTransform = std::make_shared<WorldTransform>();

	ui_ = std::make_unique<UI>();
	ui_->SetScore(score_.get());

	timer_ = std::make_unique<Timer>();

	bottonTransform_.resize(2);
}

Battle::~Battle() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void Battle::Initialize()
{
	//	カメラの設定
	//camera_->transform.translation_ = Vector3(0.0f, 16.5f, -21.7f);
	//camera_->transform.rotation_ = Vector3(0.471f, 0.0f, 0.0f);
	
	
	masterSpeed = 1.0f;

	EnemyReset();

	EnemyGeneration();

	score_->Initialize();

	ui_->Initialize();

	player_->Initialize(mobModels_, worldTransform.get());

	player_->SetDistance(kEnemyPopPosLength_);

	player_->GaugeInitialize();

	timer_->Initialize();

}


void Battle::EnemyGeneration() {

	unsigned int currentTime = static_cast<unsigned int>(time(nullptr));
	srand(currentTime);

	while (enemyNum_ - enemyKillCount_ < kEnemyIntervalNum_) {

		if (enemyNum_ == kEnemyMaxNum_) {
			break;
		}
		else {
			Enemy* enemy = new Enemy();

			float  pos = kEnemyPopPosLength_ * (enemyNum_)+6.0f;

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
			enemy->InitializeSP(pos, type, enemyNum_, mobModels_type2);

			enemies_.push_back(enemy);

			enemyNum_++;

			preEnemyType_ = type;
		}
	}

}

void Battle::EnemyReset() {

	enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});

	enemyNum_ = 0;
	enemyKillCount_ = 0;
	
	preEnemyType_ = 0;
	typeCount_ = 0;
}

void Battle::Update()
{

	timer_->Update();

	for(Enemy* enemy : enemies_){
		if (enemy->GetNum() == enemyKillCount_) {
			type_ = static_cast<uint16_t>(enemy->GetBottomType());
			player_->HitTest(enemy);
			
			if (score_->GetEvaluation()) {

				enemyKillCount_++;
				EnemyGeneration();
			}

			break;
		}
	}


	player_->GaugeUpdate();

	enemies_.remove_if([](Enemy* enemy) {

		return enemy->GetDelete() == true;

		});
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	//	一旦仮置き 敵を最大数倒したらシーン切り替え
	if (enemyKillCount_ >= kEnemyMaxNum_ || timer_->GetTime() == 0) {
		GameScene::GetInstance()->sceneChangeFlag = true;
	}


	player_->Update();
	ui_->Update();
	worldTransform->UpdateMatrix();

}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	
	player_->Draw(viewProjection);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection, bottonModels_);
	}
	
	
}

void Battle::Draw2D(const Matrix4x4& viewProjection) {

	player_->GaugeDraw2D(viewProjection);
	ui_->Draw2D(viewProjection);
	timer_->Draw2D(viewProjection);

	for (uint16_t i = 0u; i < bottonTransform_.size(); i++) {
		Texture2D::TextureDraw(bottonTransform_[i], viewProjection, 0xffffffff, bottonTexture_[type_].get());
	}

}
