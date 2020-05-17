#include "stdafx.h"

#include "PhysicsObject.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

#include "Script.h"

Application::Application(Ogre::String tittle)
	: mResourcesCfg("resources.cfg"),
	mRoot(0),
	mWindow(0),
	mSceneMgr(0),
	mQuit(false)
{
	gEnv->application = this;

	initialize(tittle);
}

Application::~Application()
{
	shutdown();
}

bool Application::initialize(Ogre::String tittle)
{
	mRoot = new Ogre::Root(Ogre::StringUtil::BLANK, Ogre::StringUtil::BLANK, tittle + ".log");

	Ogre::RenderSystem* renderSystem;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#ifdef _DEBUG
		mRoot->loadPlugin("RenderSystem_Direct3D9_d");
	#else
		mRoot->loadPlugin("RenderSystem_Direct3D9");
	#endif

	renderSystem = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
#else
	#ifdef _DEBUG
		mRoot->loadPlugin("RenderSystem_GL_d");
	#else
		mRoot->loadPlugin("RenderSystem_GL");
	#endif

	renderSystem = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
#endif

#ifdef _DEBUG
	mRoot->loadPlugin("OgreOggSound_d");
#else
	mRoot->loadPlugin("OgreOggSound");
#endif

	renderSystem->setConfigOption("Full Screen", "No");
	renderSystem->setConfigOption("VSync", "Yes");
	renderSystem->setConfigOption("Video Mode", "1280 x 720 @ 32-bit colour");
	renderSystem->setConfigOption("FSAA", "8 [Quality]");

	mRoot->setRenderSystem(renderSystem);

	mWindow = mRoot->initialise(true, tittle);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW",(void*)&hwnd);
	LONG iconID =(LONG)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON));
	SetClassLong(hwnd, GCL_HICON, iconID);
#endif

	initializeResources();
	loadResources();

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	mGorilla = new Gorilla::Silverback();
	mGorilla->loadAtlas("dejavu");

	gEnv->frameListener = new FrameListener();
	gEnv->inputListener = new InputListener();
	gEnv->bulletListener = new BulletListener();

	gEnv->soundManager = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
	gEnv->soundManager->init("", 100, 100, mSceneMgr);

	gEnv->luaState = new LuaState();
	gEnv->luaInterpreter = new LuaInterpreter();

	mRoot->addFrameListener(gEnv->frameListener);

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, gEnv->inputListener->getMouse(), this);
	mTrayMgr->hideCursor();

	Ogre::Camera* camera = mSceneMgr->createCamera("Main");
	camera->setPosition(Ogre::Vector3(0,0,80));
	camera->lookAt(Ogre::Vector3(0,0,-300));
	camera->setNearClipDistance(1);
	mCameraMan = new OgreBites::SdkCameraMan(camera);
	Ogre::Viewport* vp = mViewport = mWindow->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// должен быть инициализирован после создания камеры/вьюпорта
	mScreen = mGorilla->createScreen(mViewport, "dejavu");

	mInfoLayer = mScreen->createLayer(14);
	mFPS = mInfoLayer->createCaption(9, 10, 10, Ogre::StringUtil::BLANK);
	mObjects = mInfoLayer->createCaption(9, 10, 10, Ogre::StringUtil::BLANK);

	gEnv->luaConsole = new LuaConsole();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	gEnv->bulletListener->addGround();

	return true;
}

void Application::shutdown()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, gEnv->frameListener);
	delete mRoot;
}

void Application::start()
{
	auto sound = gEnv->soundManager->createSound("Sound1", "DarkWinds.ogg", true, true);
	sound->disable3D(true);
	sound->play();

	testscript.load("Scripts/box.lua");

	mRoot->startRendering();
}

void Application::update(Ogre::Real deltaTime)
{
	testscript.update();

	std::stringstream fpsString;
	fpsString << "FPS: " << mWindow->getLastFPS() << "    Batches: " << mRoot->getRenderSystem()->_getBatchCount() << "\n";
	mFPS->top(mViewport->getActualHeight() - 20);
	mFPS->text(fpsString.str());

	std::stringstream objectsString;
	objectsString << "Objects: " << gEnv->objects.size();
	mObjects->top(mViewport->getActualHeight() - 40);
	mObjects->text(objectsString.str());
}

void Application::keyPressed(const OIS::KeyEvent& arg)
{
	if(arg.key == OIS::KC_SPACE)
		testscript.execute();
	if(arg.key == OIS::KC_ESCAPE)
		mQuit = true;
}

void Application::keyReleased(const OIS::KeyEvent& arg)
{
}

void Application::mouseMoved(const OIS::MouseEvent& arg)
{
}

void Application::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
}

void Application::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
}

void Application::initializeResources()
{
	Ogre::ConfigFile cf;
	Ogre::String secName, typeName, archName;

	cf.load(mResourcesCfg);
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName, false);
		}
	}
}

void Application::loadResourceGroup(Ogre::String group)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(group);
}

void Application::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}