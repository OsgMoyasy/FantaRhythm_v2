#include "FantaRhythm_v2.h"
#include "SceneManager.h"
void Main(){
	Profiler::EnableAssetCreationWarning(false);
	Window::Resize(1280,720);
	Graphics::SetTargetFrameRateHz(60);

	SceneManager sceneManager;
	while (System::Update()){
		ClearPrint();
		sceneManager.sceneUpdate();

		//デバッグ出力: FPSの表示
		Print << Profiler::FPS();
	}
}
