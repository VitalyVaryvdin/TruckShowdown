#ifndef _LuaInterpreter_h_
#define _LuaInterpreter_h_

class LuaInterpreter
{
public:
	LuaInterpreter();
	~LuaInterpreter();

	void executeString(Ogre::String& str);

	Ogre::String getOutput() { return mOutput; }
	std::string getDebugString();
private:
	Ogre::String mOutput;
};

#endif // _LuaInterpreter_h_