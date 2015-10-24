#include "System Class.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{

	SystemClass* system;

	system = new SystemClass;

	system->SystemInitialization();

	system->SystemLoop();
	
	system->SystemShutdown();



	return 0;
}