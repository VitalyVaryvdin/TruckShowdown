#ifndef _LuaState_h_
#define _LuaState_h_

class LuaState
{
public:
	LuaState();
	~LuaState();

	void initialize();
	void shutdown();

	void bindOgre();
	void bindOgreBullet() {}
	void bindOgreOggSound() {}
	void bindFramework();

	lua_State* getState() { return mState; }
private:
	lua_State* mState;
};

#endif // _LuaState_h_