#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "ClientEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	client::ClientEngine* engine = new client::ClientEngine();

	engine->Initialize(1000, 500, L"Client");
	engine->Process();
	engine->Finalize();
	delete engine;

	return 0;
}