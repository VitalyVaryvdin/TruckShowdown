#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

InputListener::InputListener()
{
	initialize();
}

InputListener::~InputListener()
{
	shutdown();
}

void InputListener::initialize()
{
	size_t hWnd; gEnv->application->getWindow()->getCustomAttribute("WINDOW", &hWnd);

	mInputManager = OIS::InputManager::createInputSystem(hWnd);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true)); mKeyboard->setEventCallback(this);
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true)); mMouse->setEventCallback(this);
}

void InputListener::shutdown()
{
	if(mInputManager)
	{
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputManager);
	}
}

bool InputListener::keyPressed(const OIS::KeyEvent& arg)
{
	if(arg.key == OIS::KC_SYSRQ)
		Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Saved screenshot ") + gEnv->application->getWindow()->writeContentsToTimestampedFile("screenshot", ".png"));

	if(arg.key == OIS::KC_GRAVE)
		gEnv->luaConsole->setVisible(!gEnv->luaConsole->isVisible());

	if(gEnv->luaConsole->isVisible())
		gEnv->luaConsole->onKeyPressed(arg);
	else
	{
		gEnv->application->keyPressed(arg);
		gEnv->application->getCameraManager()->injectKeyDown(arg);
	}

	return true;
}

bool InputListener::keyReleased(const OIS::KeyEvent& arg)
{
	if(!gEnv->luaConsole->isVisible())
	{
		gEnv->application->keyReleased(arg);
		gEnv->application->getCameraManager()->injectKeyUp(arg);
	}

	return true;
}

bool InputListener::mouseMoved(const OIS::MouseEvent& arg)
{
	if(!gEnv->luaConsole->isVisible())
	{
		gEnv->application->mouseMoved(arg);
		gEnv->application->getCameraManager()->injectMouseMove(arg);
	}

	mXAbs = arg.state.X.abs;
	mYAbs = arg.state.Y.abs;

	return true;
}

bool InputListener::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(!gEnv->luaConsole->isVisible())
	{
		gEnv->application->mousePressed(arg, id);
		gEnv->application->getCameraManager()->injectMouseDown(arg, id);
	}

	return true;
}

bool InputListener::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(!gEnv->luaConsole->isVisible())
	{
		gEnv->application->mouseReleased(arg, id);
		gEnv->application->getCameraManager()->injectMouseUp(arg, id);
	}

	return true;
}