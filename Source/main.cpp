#include "stdafx.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	Application app("Framework");

	app.start();

	return 0;
}