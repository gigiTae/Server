#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "ServerEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	server::ServerEngine* engine = new server::ServerEngine();

	engine->InitializeEngine(1000, 500, L"Server");
	engine->Process();
	engine->FinalizeEngine();

	delete engine;

	return 0;
}

