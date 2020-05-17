#include "stdafx.h"

#include "Object.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

FrameListener::FrameListener()
{
	initialize();
}

FrameListener::~FrameListener()
{
	shutdown();
}

void FrameListener::initialize()
{
	Ogre::WindowEventUtilities::addWindowEventListener(gEnv->application->getWindow(), this);
}

void FrameListener::shutdown()
{

}

bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	gEnv->luaConsole->frameStarted(evt);

	gEnv->bulletListener->frameStarted(evt);

	for(auto it = gEnv->objects.begin(); it != gEnv->objects.end(); it++)
		(*it)->update();

	gEnv->soundManager->update(evt.timeSinceLastFrame);
	gEnv->application->update(evt.timeSinceLastFrame);

	return true;
}


bool FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(gEnv->application->getWindow()->isClosed() || gEnv->application->quit())
		return false;

	gEnv->inputListener->getKeyboard()->capture();
	gEnv->inputListener->getMouse()->capture();

	gEnv->application->getCameraManager()->frameRenderingQueued(evt);
	gEnv->application->getTrayManager()->frameRenderingQueued(evt);
	
	return true;
}

bool FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	gEnv->bulletListener->frameEnded(evt);

	return true;
}

void FrameListener::windowResized(Ogre::RenderWindow* rw)
{
}

void FrameListener::windowClosed(Ogre::RenderWindow* rw)
{
	gEnv->inputListener->shutdown();
}