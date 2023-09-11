#include <Windows.h>
#include "Engine/Engine.h"
#include "Engine/Texture/Texture2D.h"
#include "externals/imgui/imgui.h"
#include "math/Matrix4x4.h"
#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Input/AudioInput/AudioInput.h"

#include "Game/GameScene/GameScene.h"

#include "GlobalVariables/GlobalVariables.h"
#include "Engine/TimeBaseLoopExecuter/TimeBaseLoopExecuter.h"


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	static D3DResourceLeakChecker leak;
	//OutputDebugStringA("Hello,DirectX!\n");
	int32_t windowWidth = 1280; int32_t windowHeight = 720;
	Engine::Initialize("閉店一分前", windowWidth, windowHeight);
	
	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	GameScene::GetInstance()->Initialize();

	
	//std::unique_ptr<TimeBaseLoopExecuter> fpsManager;
	//fpsManager = std::make_unique<TimeBaseLoopExecuter>(50.0f);

	//	ウィンドウの×ボタンが押されるまでループ
	while (!WinApp::ProcessMessage()) {
		//	フレームの開始
		TimeBaseLoopExecuter fpsManager;
		
		Engine::BeginFrame();
#ifdef _DEBUG
		//	ImGui のフレームに一つ目の ImGui のウィンドウを描く
		ImGui::Begin("Control panel");
		ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
		//ImGui::Text("Frame rate2: %6.2f fps", fpsManager);
		ImGui::End();
#endif 


		// グローバル変数の更新
		GlobalVariables::GetInstance()->Update();

		GameScene::GetInstance()->Update();

		GameScene::GetInstance()->Draw();

		

		//	フレームの終了
		Engine::EndFrame();
		fpsManager.TimeAdjustment();
		if (KeyInput::PushKey(DIK_ESCAPE)) {
			break;
		}
	}

	Engine::Finalize();

	return 0;
}