#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <windows.h>
#include "ClientProcess.h"

using namespace client;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	ClientProcess* engine = new ClientProcess();

	engine->Initialize();
	engine->Process();
	engine->Finalize();

	return 0;
}