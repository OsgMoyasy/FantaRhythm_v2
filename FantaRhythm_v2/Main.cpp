#include "FantaRhythm_v2.h"
#include "SceneManager.h"
void Main(){
	Profiler::EnableAssetCreationWarning(false);
	Window::Resize(1280,720);
	Graphics::SetTargetFrameRateHz(60);

	SceneManager::initialize();
	while (System::Update()){
		ClearPrint();
		SceneManager::updateScene();
		SceneManager::drawScene();

		//デバッグ出力: FPSの表示
		Print << Profiler::FPS();
	}
	SceneManager::finalize();
}
