#ifndef _BulletListener_H_
#define _BulletListener_H_

static const Ogre::Vector3 WorldGravity = Ogre::Vector3(0, -9.81, 0);
static const Ogre::AxisAlignedBox WorldBounds = Ogre::AxisAlignedBox(Ogre::Vector3(-10000, -10000, -10000), Ogre::Vector3(10000,  10000,  10000));

class BulletListener
{
public:
	BulletListener(bool useDebugDrawer = true);
	~BulletListener();

	void initialize(bool useDebugDrawer = true);
	void shutdown();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	
	void addGround();

	OgreBulletDynamics::DynamicsWorld* getWorld() { return mWorld; }
	btDynamicsWorld* getBulletDynamicsWorld() { return mWorld->getBulletDynamicsWorld(); }
protected:
	void initializeWorld(bool useDebugDrawer = true, const Ogre::Vector3& gravity = WorldGravity, const Ogre::AxisAlignedBox& bounds = WorldBounds);
	OgreBulletDynamics::RigidBody* addStaticPlane( const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction);

	OgreBulletDynamics::DynamicsWorld* mWorld;
	OgreBulletCollisions::DebugDrawer* mDebugDrawer;
};

#endif // _BulletListener_h_