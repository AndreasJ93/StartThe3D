#include "System Class.h"


#pragma region Constructor and Deconstructor

SystemClass::SystemClass()
{
	mInput = 0;
	mGraphics = 0;
}

SystemClass::~SystemClass()
{

}

#pragma endregion

#pragma region Initialize and Run

bool SystemClass::SystemInitialization()
{
	CreateConsole();

	cout << ">>Information Window Initialized" << endl;

	if (!WindowInitialization())
	{
		return false;
	}

	cout << ">>Window Initialized" << endl;

	mInput = new InputClass;
	if (!mInput)
	{
		return false;
	}
	
	mInput->Initialize();

	cout << ">>>>Input Controls Active" << endl;

	cout << ">>>>Timer Initalized" << endl;

	mGraphics = new GraphicsClass;
	if (!mGraphics)
	{
		return false;
	}

	if (!mGraphics->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, mHandle))
	{
		return false;
	}

	cout << ">>Graphics Fully Initialized" << endl;


	return true;
}

void SystemClass::CreateConsole()
{
	AllocConsole();  // Create Console Window

	FILE *stream;

	wstring ConsoleName = L"Information Console";

	_wfreopen_s(&stream, L"CONIN$", L"rb", stdin);   // reopen stdin handle as console window input

	_wfreopen_s(&stream, L"CONOUT$", L"wb", stdout);  // reopen stout handle as console window output

	_wfreopen_s(&stream, L"CONOUT$", L"wb", stderr); // reopen stderr handle as console window output

	SetConsoleTitle(ConsoleName.c_str());
	
}

bool SystemClass::WindowInitialization()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer.
	ApplicationHandle = this;

	//Get the instance of this application
	mHinstance = GetModuleHandle(NULL);

	//Set the name of the application
	mApplicationName = L"3D Experimentation";

	//Setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the desktop screen
	mScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	mScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings. If it's in fullscreen, we specify for that.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)mScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)mScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		// Create the window with the screen settings and get the handle to it.
		mHandle = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, mScreenWidth, mScreenHeight, NULL, NULL, mHinstance, NULL);

		//Hide the cursor
		ShowCursor(false);

	}
	else
	{

		mScreenWidth = WINDOW_WIDTH;
		mScreenHeight = WINDOW_HEIGHT;

		// Place the window where it should be.
		posX = (GetSystemMetrics(SM_CXSCREEN) - mScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - mScreenHeight) / 2;

		// Create the window with the screen settings and get the handle to it.
		mHandle = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
			WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_SYSMENU,
			posX, posY, mScreenWidth, mScreenHeight, NULL, NULL, mHinstance, NULL);
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mHandle, SW_SHOW);
	//SetForegroundWindow(mHandle);
	SetFocus(mHandle);

	return true;
}

#pragma endregion

#pragma region Run commands

void SystemClass::SystemLoop()
{

	MSG msg;
	bool done, result;

	//Initialize message structure;
	ZeroMemory(&msg, sizeof(MSG));

	//Intialize Timer
	mTimer.Reset();

	//Loop until there is a quit message from the windows or the use
	done = false;
	while (!done)
	{
		//Handle the window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If we tell the window to quit, quit.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Do the frame processing, quit if it tells us to.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	//Check so we're actually in the window. If not, we just loop without doing anything.
	if (GetActiveWindow() == mHandle)
	{
		//Timer
		mTimer.Tick();
		CalculateAverageFrames();
		//Check if we pressed escape, and thus want to quit.

		POINT currentPos;
		GetCursorPos(&currentPos);
		mGraphics->SetCurrentMousePosition(currentPos.x, currentPos.y);

		if (mInput->IsKeyDown(VK_ESCAPE))
		{
			return false;
		}

		mGraphics->Frame(mInput, mTimer.DeltaTime());

	}

	else
	{
		Sleep(200);
	}

	return true;
}

void SystemClass::CalculateAverageFrames()
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	if ((mTimer.Time() - timeElapsed) > 1.0f)
	{
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		wostringstream outs;
		outs.precision(6);
		outs << mApplicationName << L"    " << L"FPS:  " << fps << L"    " << L"Frame Time: " << mspf
			<< L"  (ms)";
		SetWindowText(mHandle, outs.str().c_str());


		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

#pragma endregion


#pragma region Message Handling

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{

	switch (umsg)
	{

		//check if a key has been pressed on the keyboard
	case WM_KEYDOWN:
	{
		//If a key is pressed send it to the input object so it can record that state
		mInput->KeyDown((unsigned int)wParam);
		break;
	}
	//check if a key has been released
	case WM_KEYUP:
	{
		mInput->KeyUp((unsigned int)wParam);
		break;
	}
	case WM_LBUTTONUP:
	{
		mInput->MouseDown();
		break;
	}

	//Send every other message to the default message handler
	default:
	{
		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}

	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
	return 0;
}

#pragma endregion

#pragma region Shutdown

void SystemClass::SystemShutdown()
{
	if (mGraphics)
	{
		mGraphics->Shutdown();
		delete mGraphics;
		mGraphics = 0;
	}

	FreeConsole();
}


#pragma endregion

