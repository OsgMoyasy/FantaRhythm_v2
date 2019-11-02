#include "SceneManager.h"
#include "MyKey.h"
#include "OperationSE.h"
#include "OperationTexture.h"

void Main(){
	Profiler::EnableAssetCreationWarning(false);
	Window::Resize(1280,720);
	Graphics::SetTargetFrameRateHz(60);

	SceneManager::initialize();
	OperationSE::initialize();
	OperationTexture::initialize();
	while (System::Update()){
		ClearPrint();
		MyKey::isGamePad();
		SceneManager::updateScene();
		SceneManager::drawScene();

		//デバッグ出力: FPSの表示
		//Print << Profiler::FPS();
	}
	SceneManager::finalize();
	OperationSE::finalize();
	OperationTexture::finalize();
}
