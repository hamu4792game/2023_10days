#include "Battle.h"
#include "externals/imgui/imgui.h"
#include "math/Vector4.h"

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

	masterSpeed = 1.0f;

	EnemyReset();

	EnemyGeneration();

	score_->Initialize();

	ui_->Initialize();

	player_->Initialize(mobModels_, worldTransform.get());

}


void Battle::EnemyGeneration() {

	for (int i = 0; i < kEnemyMaxNum_; i++) {

		Enemy* enemy = new Enemy();

		float  pos = kEnemyPopPosLength_ * (enemyNum_) + 1.0f;

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

void Battle::EnemyReset() {
	for (Enemy* enemy : enemies_) {
		enemy->Die(0);
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
	ImGui::DragFloat("master", &worldTransform->scale_.y, 0.01f);

	for(Enemy* enemy : enemies_){
		if (enemy->GetNum() == enemyKillCount_) {
			player_->HitTest(enemy);
			
			if (score_->GetEvaluation()) {

				enemyKillCount_++;
			}

			break;
		}
	}

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// 確認のため追加 by.Korone
	if (KeyInput::PushKey(DIK_SPACE)) {
		score_->AddPerfect();
	}
	if (KeyInput::PushKey(DIK_R)) {
		score_->AddMiss();
	}


	//ImGui::DragFloat("worldRo", &transform.rotation_.x, AngleToRadian(1.0f));
	//	行列の更新　回転行列のみ必要なためUpdateはしていない
	player_->Update();
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

	ui_->Draw2D(viewProjection);
}
