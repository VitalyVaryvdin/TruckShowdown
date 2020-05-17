#include "stdafx.h"

#include "LuaInterpreter.h"

#include "LuaState.h"

LuaInterpreter::LuaInterpreter()
{
}

LuaInterpreter::~LuaInterpreter()
{
}

void LuaInterpreter::executeString(Ogre::String& str)
{
	int error = 0;

	if(!str.empty())
	{
		luaL_loadstring(gEnv->luaState->getState(), str.c_str());
		error = lua_pcall(gEnv->luaState->getState(), 0, 0, 0);
	}

	if(error)
	{
		mOutput = lua_tostring(gEnv->luaState->getState(), -1);
		lua_pop(gEnv->luaState->getState(), 1);
	}
}

std::string LuaInterpreter::getDebugString()
{
	/*lua_Debug debugInfo;

	int r = lua_getstack(gEnv->luaState->getState(), 0, &debugInfo);
	if(r == 0)
	{
		return "Requested stack depth exceeded current stack depth";
	}

	lua_getinfo(gEnv->luaState->getState(), "Slun", &debugInfo);*/
	const char* er = lua_tostring(gEnv->luaState->getState(), -1);
	lua_pop(gEnv->luaState->getState(), 1);

	/*std::stringstream s;
	s <<	"Error "<<debugInfo.event <<" at (" << debugInfo.currentline << "): " << er << "\n" << 
			"\tIn a "<< debugInfo.namewhat << " " <<debugInfo.source << " function called " << debugInfo.name <<".\n" <<
			"\tIn " << debugInfo.short_src << " defined at "<< debugInfo.linedefined << ".";

	return s.str();*/

	return er;
}