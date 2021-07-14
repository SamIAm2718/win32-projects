#include <Windows.h>

#include "constants.h"
#include "GameWindow.h"
#include "resource.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	if (SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		WCHAR gameName[constants::MAX_LOADSTRING];
		LoadString(hInstance, IDS_GAME_NAME, gameName, sizeof(gameName) / sizeof(WCHAR));
		HMENU gameMenu{ LoadMenu(hInstance, MAKEINTRESOURCE(IDR_GAMEMENU)) };
		HACCEL hAccel{ LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR)) };

		GameWindow gameWindow{ constants::BEGINNER_WIDTH, constants::BEGINNER_HEIGHT, constants::BEGINNER_CMINES };

		if (!gameWindow.Create(gameName, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900, 0, gameMenu))
		{
			return 0;
		}

		ShowWindow(gameWindow.Window(), nCmdShow);

		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(gameWindow.Window(), hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	CoUninitialize();
	return 0;
}