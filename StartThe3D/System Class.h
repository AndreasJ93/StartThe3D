#ifndef SYSTEM_CLASS_H
#define SYSTEM_CLASS_H

#include "Help Functions and Libraries.h"
#include "Graphic Class.h"
#include "Timer Class.h"

#define MAX_CONSOLE_LINES 500

class SystemClass
{
private:

	//Window Functions and Variables
	bool WindowInitialization();
	LPCWSTR mApplicationName;
	HINSTANCE mHinstance;
	HWND mHandle;
	int mScreenHeight, mScreenWidth;

	void CreateConsole();

	InputClass* mInput;

	//Graphic Control Unit
	GraphicsClass* mGraphics;

	//Runtime Commands and variables
	bool Frame();
	void CalculateAverageFrames();

	TimerClass mTimer;


public:
	SystemClass();
	~SystemClass();

	//Input control
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

	bool SystemInitialization();

	void SystemLoop();

	void SystemShutdown();


};

//WndProc and ApplicationHandle is used to make sure we can handle windows messages
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;


#endif