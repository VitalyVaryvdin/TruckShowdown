#ifndef _Environment_h_
#define _Environment_h_

class Application;
class FrameListener;
class InputListener;
class BulletListener;

class LuaState;
class LuaInterpreter;
class LuaConsole;

class Object;
class PhysicsObject;

struct Environment
{
	Application* application;
	FrameListener* frameListener;
	InputListener* inputListener;
	BulletListener* bulletListener;
	OgreOggSound::OgreOggSoundManager* soundManager;

	LuaState* luaState;
	LuaInterpreter* luaInterpreter;
	LuaConsole* luaConsole;
	
	std::vector<Object*> objects;
};

extern Environment* gEnv;

#endif // _Environment_h_