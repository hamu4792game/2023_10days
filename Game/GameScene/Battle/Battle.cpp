#include "Battle.h"
//#include "externals/imgui/imgui.h"
#include "math/Vector4.h"
#include "Game/GameScene/GameScene.h"
#include <algorithm>

// 確認のため追加 by.Korone
#include "Engine/Input/KeyInput/KeyInput.h"

decltype(Battle::masterSpeed) Battle::masterSpeed;

Battle::Battle(std::shared_ptr<Camera> camera)
{
	camera_ = camera;
	player_ = std::make_unique<Player>(camera_);


	score_ = std::make_unique<Score>();

	player_->SetScore(score_.get());

	player_->SetFlag(&tutorialFlag_);

	worldTransform = std::make_shared<WorldTransform>();

	ui_ = std::make_unique<UI>();
	ui_->SetScore(score_.get());

	timer_ = std::make_unique<Timer>();

	//	soundのロード
	bgm.SoundLoadWave("Resources/sound/battleBGM.wav");
	sLittlePunch.SoundLoadWave("Resources/sound/littlePunch.wav");
	sBigPunch.SoundLoadWave("Resources/sound/bigPunch.wav");
	sFallDown.SoundLoadWave("Resources/sound/fallDown.wav");
	sHitToKill.SoundLoadWave("Resources/sound/hitToKill.wav");

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

	player_->Initialize(mobModels_[0], worldTransform.get());

	player_->SetDistance(kEnemyPopPosLength_);

	player_->GaugeInitialize();

	timer_->Initialize();

	bottonTransform_.parent_ = &camera_->transform;
	//bottonTransform_.parent_ = &player_->GetPlayerTransform();
	bottonTransform_.translation_ = Vector3(0.0f, 0.0f, 1.0f);
	bottonTransform_.scale_ = Vector3(0.06f, 0.06f, 0.06f);
	
	tutorialFlag_ = true;

	clearFlag_ = true;

	blackTrans_.scale_ = Vector3(80.0f, 45.0f, 1.0f);
	blackTrans_.cMono->pibot = Vector2(642.0f, 359.0f);
	blackTrans_.cMono->rate = 800.0f;

	rand_ = 0;
	randCount_ = 0;
}


void Battle::ButtonRand()
{
	
	int ram = std::rand() % 2;

	if (ram == rand_) {
		randCount_++;

		if (randCount_ == 3) {
			while (ram == rand_) {

				ram = std::rand() % 2;
			}

			rand_ = ram;
			randCount_ = 0;
		}
	}
	else {
		rand_ = ram;
	}


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
			enemy->InitializeSP(pos, type, enemyNum_, mobModels_[1 + type]);

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
	//	ボタンの回転
	if (!player_->GetMoveFlag()) {
		bottonTransform_.rotation_.y += AngleToRadian(720.0f / player_->GetMAX_Frame()) * Battle::masterSpeed;
	}
	else {
		bottonTransform_.rotation_.y = 0.0f;
	}
	
	//	
	if (!tutorialFlag_) {
		timer_->Update();
	}
	else {
		if (score_->GetEvaluation()) {
			tutorialFlag_ = false;
		}
		else
		{
			blackTrans_.cMono->rate -= 10.0f;
			blackTrans_.cMono->rate = std::clamp<float>(blackTrans_.cMono->rate, 110.0f, 800.0f);
			blackTrans_.UpdateMatrix();
		}
		
	}

	for (Enemy* enemy : enemies_) {
		if (enemy->GetNum() == enemyKillCount_) {
			type_ = static_cast<uint16_t>(enemy->GetBottomType());
			player_->HitTest(enemy);

			if (score_->GetEvaluation()) {

				enemyKillCount_++;
				EnemyGeneration();
				ButtonRand();
				enemy->SetAnimeState(MODE_A::NORMAL);

				if (score_->GetCombo() >= 15) {
					sHitToKill.SoundStop();
					sHitToKill.SoundPlayWave();
					enemy->SetAnimeState(MODE_A::BAKUSAN);
				}
				else
				{
					//	ここで音の変化
					if (score_->GetEvaluation() == Score::Evaluation::kPerfect) {
						sBigPunch.SoundStop();
						sBigPunch.SoundPlayWave();
					}
					else if (score_->GetEvaluation() == Score::Evaluation::kMiss) {
						sFallDown.SoundStop();
						sFallDown.SoundPlayWave();
					}
					else {
						sLittlePunch.SoundStop();
						sLittlePunch.SoundPlayWave();
					}
				}
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

		if (timer_->GetTime() == 0) {
			int num = kEnemyMaxNum_ - enemyKillCount_;

			clearFlag_ = false;

			if (kEnemyMaxNum_ != score_->GetPerfect() + score_->GetGreat() + score_->GetGood() + score_->GetMiss()) {
				for (int i = 0; i < num; i++) {
					if (kEnemyMaxNum_ == score_->GetPerfect() + score_->GetGreat() + score_->GetGood() + score_->GetMiss()) {
						break;
					}
					else {
						score_->AddMiss();
					}
				}
			}
		}

		GameScene::GetInstance()->sceneChangeFlag = true;
	}


	player_->Update();
	ui_->Update();
	worldTransform->UpdateMatrix();

	
	if (score_->GetCombo() > 5 && rand_ == 1) {

	}
	else {
		camera_->transform.UpdateMatrix();
	}
	
	
	bottonTransform_.UpdateMatrix();
}

void Battle::Draw(const Matrix4x4& viewProjection)
{
	
	player_->Draw(viewProjection);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
	
	if (enemyKillCount_ < kEnemyMaxNum_) {
		Model::ModelDraw(bottonTransform_, viewProjection, 0xffffffff, bottonModels_[type_].get());
	}

}

void Battle::Draw2D(const Matrix4x4& viewProjection) {

	player_->GaugeDraw2D(viewProjection);
	ui_->Draw2D(viewProjection);
	timer_->Draw2D(viewProjection);

	if (tutorialFlag_) {
		Texture2D::TextureDraw(blackTrans_, viewProjection, 0x000000aa, blackBox_.get());
	}
}
