#ifndef _Script_h_
#define _Script_h_

class Script
{
public:
	Script();
	Script(Ogre::String file);

	void load(Ogre::String file);
	void reload();

	void execute();
	void update();

	bool fileChanged();
protected:
	Ogre::String mFileName;
	Ogre::String mMemoryBuffer;

	bool mReady;
};

#endif // _Script_h_