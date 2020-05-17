#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

BulletListener::BulletListener(bool useDebugDrawer)
    : mWorld(0)
{
	initialize(useDebugDrawer);
}

BulletListener::~BulletListener()
{
	shutdown();
}

void BulletListener::initialize(bool useDebugDrawer)
{
	initializeWorld(useDebugDrawer);
}

void BulletListener::shutdown()
{
	if(mDebugDrawer)
		delete mDebugDrawer;
	if(mWorld)
		delete mWorld;
}

bool BulletListener::frameStarted(const Ogre::FrameEvent& evt)
{
	mWorld->stepSimulation(evt.timeSinceLastFrame);

	return true;
}

bool BulletListener::frameEnded(const Ogre::FrameEvent& evt)
{
	mWorld->stepSimulation(evt.timeSinceLastFrame);

	return true;
}

void BulletListener::addGround()
{
    addStaticPlane(0.1, 0.8);
}

void BulletListener::initializeWorld(bool useDebugDrawer, const Ogre::Vector3& gravityVector, const Ogre::AxisAlignedBox& bounds)
{
	mWorld = new OgreBulletDynamics::DynamicsWorld(gEnv->application->getSceneManager(), bounds, gravityVector, true, true, 10000);

	mDebugDrawer = new OgreBulletCollisions::DebugDrawer();

	if(useDebugDrawer)
	{
		mDebugDrawer->setDrawAabb(true);
		mDebugDrawer->setDrawWireframe(true);
		mDebugDrawer->setDrawContactPoints(true);
		mWorld->setShowDebugShapes(true);
	}

	mWorld->setDebugDrawer(mDebugDrawer);

	Ogre::SceneNode* debugDrawer = gEnv->application->getSceneManager()->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	debugDrawer->attachObject(static_cast<Ogre::SimpleRenderable*>(mDebugDrawer));
}

OgreBulletDynamics::RigidBody* BulletListener::addStaticPlane(const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction)
{
	int i = 0;
	Ogre::StaticGeometry* s = gEnv->application->getSceneManager()->createStaticGeometry("StaticFloor");
	s->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
	s->setOrigin(Ogre::Vector3::ZERO);
	for(Ogre::Real z = -80.0; z <= 80.0; z += 20.0)
	{
		for(Ogre::Real x = -80.0; x <= 80.0; x += 20.0)
		{
			Ogre::String name = Ogre::String("Plane_") + Ogre::StringConverter::toString(i++);

			Ogre::Entity* entity = gEnv->application->getSceneManager()->createEntity(name, "plane.mesh");
			entity->setMaterialName("BulletPlane");
			entity->setCastShadows(false);
			s->addEntity(entity, Ogre::Vector3(x,0,z));
		}
	}
	s->build();


	OgreBulletCollisions::CollisionShape* Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0);

	OgreBulletDynamics::RigidBody* defaultPlaneBody = new OgreBulletDynamics::RigidBody("Plane", mWorld);

	defaultPlaneBody->setStaticShape(Shape, bodyRestitution, bodyFriction);

	return defaultPlaneBody;
}