
#include "../Common/WindowSystem.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    common::WindowSystem ws{};
    ws.Initalize(1000, 500, L"Client");

    while (ws.Process())
    {
    }

    ws.Finalize();

    return 0;
}