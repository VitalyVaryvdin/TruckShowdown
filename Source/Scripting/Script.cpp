#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

#include "Script.h"

Script::Script()
	: mReady(false)
{

}

Script::Script(Ogre::String file)
	: mReady(false)
{
	load(file);
}

void Script::load(Ogre::String file)
{
	mFileName = file;

	std::ifstream scriptFile;
	scriptFile.open(file, std::ios::in);

	std::stringstream buffer;
	buffer << scriptFile.rdbuf();
	mMemoryBuffer = buffer.str();

	scriptFile.close();

	if(!mMemoryBuffer.empty())
	{
		luaL_dostring(gEnv->luaState->getState(), mMemoryBuffer.c_str());
		mReady = true;
	}
}

void Script::reload()
{
}

void Script::execute()
{
	if(mReady)
	{
		try
		{
			luabind::call_function<void>(gEnv->luaState->getState(), "main");
		}
		catch(const std::exception& error) 
		{
			gEnv->luaConsole->print(error.what() + std::string(" ") + gEnv->luaInterpreter->getDebugString());
		}
	}
}

void Script::update()
{
}

bool Script::fileChanged()
{
	//HANDLE file = FindFirstChangeNotification("Scripts", false, FILE_NOTIFY_CHANGE_LAST_WRITE);
	return false;
}