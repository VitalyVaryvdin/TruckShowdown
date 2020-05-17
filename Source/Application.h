#ifndef _Application_h_
#define _Application_h_

#include "Script.h"

class Application : public OgreBites::SdkTrayListener
{
public:
	Application(Ogre::String tittle = "Application");
	virtual ~Application();

	bool initialize(Ogre::String tittle = "Application");
	void shutdown();

	void start();
	void update(Ogre::Real deltaTime);

	void keyPressed(const OIS::KeyEvent& arg);
	void keyReleased(const OIS::KeyEvent& arg);

	void mouseMoved(const OIS::MouseEvent& arg);
	void mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	void mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	Gorilla::Silverback* getGorilla() { return mGorilla; }
	Gorilla::Screen* getScreen() { return mScreen; }
	// hack for luaconsole
	Ogre::Viewport* getViewport() { return mViewport; }

	Ogre::Root* getRoot() { return mRoot; }
	Ogre::RenderWindow* getWindow() { return mWindow; }
	Ogre::SceneManager* getSceneManager() { return mSceneMgr; }

	OgreBites::SdkTrayManager* getTrayManager() { return mTrayMgr; }
	OgreBites::SdkCameraMan* getCameraManager() { return mCameraMan; }

	Script testscript;

	bool quit() { return mQuit; }
private:
	void initializeResources();
	void loadResourceGroup(Ogre::String group);
	void loadResources();

	// hack for luaconsole
	Ogre::Viewport* mViewport;

	Ogre::String mResourcesCfg;

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;

	Gorilla::Silverback* mGorilla;
	Gorilla::Screen* mScreen;
	Gorilla::Layer* mInfoLayer;
	Gorilla::Caption* mFPS;
	Gorilla::Caption* mObjects;

	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;

	bool mQuit;
};

#endif // _Application_h_