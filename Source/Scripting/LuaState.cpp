#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

#include "Object.h"
#include "PhysicsObject.h"

using namespace luabind;

// function proxies
Ogre::SceneNode* createChildSceneNode(Ogre::SceneNode* obj, const Ogre::String name)
{
	return obj->createChildSceneNode(name);
}

Ogre::Real Sin(Ogre::Real value)
{
	return Ogre::Math::Sin(value, false);
}

Ogre::Real Cos(Ogre::Real value)
{
	return Ogre::Math::Cos(value, false);
}

LuaState::LuaState()
{
	initialize();
}

LuaState::~LuaState()
{
	shutdown();
}

void LuaState::initialize()
{
	mState = luaL_newstate();
	luaL_openlibs(mState);

	open(mState);

	bindOgre();
	bindFramework();
}

void LuaState::shutdown()
{
	if(mState)
		lua_close(mState);
}

void LuaState::bindOgre()
{
	// Ogre::SceneManager
	module(mState)
	[
		class_<Ogre::SceneManager>("SceneManager")
		.def("setDisplaySceneNodes", &Ogre::SceneManager::setDisplaySceneNodes)
	];

	// Ogre::Vector3
	module(mState)
	[
		class_<Ogre::Vector3>("Vector3")
		.def_readwrite("x", &Ogre::Vector3::x)
		.def_readwrite("y", &Ogre::Vector3::y)
		.def_readwrite("z", &Ogre::Vector3::z) 
		.def(constructor<>())
		.def(constructor<Ogre::Vector3&>()) 
		.def(constructor<Ogre::Real, Ogre::Real, Ogre::Real>()) 
		.def("absDotProduct", &Ogre::Vector3::absDotProduct) 
		.def("crossProduct", &Ogre::Vector3::crossProduct) 
		.def("directionEquals", &Ogre::Vector3::directionEquals)
		.def("distance", &Ogre::Vector3::distance) 
		.def("dotProduct", &Ogre::Vector3::dotProduct) 
		.def("getRotationTo", &Ogre::Vector3::getRotationTo)
		.def("isZeroLength", &Ogre::Vector3::isZeroLength) 
		.def("length", &Ogre::Vector3::length) 
		.def("makeCeil", &Ogre::Vector3::makeCeil) 
		.def("makeFloor", &Ogre::Vector3::makeFloor) 
		.def("midPoint", &Ogre::Vector3::midPoint) 
		.def("normalise", &Ogre::Vector3::normalise) 
		.def("nornaliseCopy", &Ogre::Vector3::normalisedCopy)
		.def("perpendicular", &Ogre::Vector3::perpendicular)
		.def("positionCloses", &Ogre::Vector3::positionCloses)
		.def("positionEquals", &Ogre::Vector3::positionEquals) 
		.def("randomDeviant", &Ogre::Vector3::randomDeviant)
		.def("reflect", &Ogre::Vector3::reflect)
		.def("squaredDistance", &Ogre::Vector3::squaredDistance)
		.def("squaredLength", &Ogre::Vector3::squaredLength) 
		.def(self + other<Ogre::Vector3>()) 
		.def(self - other<Ogre::Vector3>()) 
		.def(self * other<Ogre::Vector3>()) 
		.def(self * Ogre::Real()) 
		.def(tostring(self))
	]; 

	object g = globals(mState); 
	object table = g["Vector3"];
	table["ZERO"] = Ogre::Vector3::ZERO;
	table["UNIT_X"] = Ogre::Vector3::UNIT_X;
	table["UNIT_Y"] = Ogre::Vector3::UNIT_Y;
	table["UNIT_Z"] = Ogre::Vector3::UNIT_Z;
	table["NEGATIVE_UNIT_X"] = Ogre::Vector3::NEGATIVE_UNIT_X;
	table["NEGATIVE_UNIT_Y"] = Ogre::Vector3::NEGATIVE_UNIT_Y;
	table["NEGATIVE_UNIT_Z"] = Ogre::Vector3::NEGATIVE_UNIT_Z;
	table["UNIT_SCALE"] = Ogre::Vector3::UNIT_SCALE;

	// Ogre::Quaternion

	// Math
	module(mState)
	[
		class_<Ogre::Math>("Math")
		.scope
		[
			def("Sin", &Sin),
			def("Cos", &Cos),
			def("DegreesToRadians", &Ogre::Math::DegreesToRadians)
		]
	];

	// Ogre::Entity
	module(mState)
	[
		class_<Ogre::MovableObject>("MovableObject"),
		class_<Ogre::Entity, Ogre::MovableObject>("Entity")
		.def(tostring(self))
		.def("setMaterialName", &Ogre::Entity::setMaterialName)
		.def("setDisplaySkeleton", &Ogre::Entity::setDisplaySkeleton)
	];

	// Ogre::SceneNode
	module(mState)
	[
		class_<Ogre::SceneNode>("SceneNode")
		.def("createChildSceneNode", createChildSceneNode)
		.def("attachObject", &Ogre::SceneNode::attachObject)
		.def("setPosition", (void(Ogre::SceneNode::*)(const Ogre::Vector3&))&Ogre::SceneNode::setPosition)
		.def("setPosition", (void(Ogre::SceneNode::*)(Ogre::Real,Ogre::Real,Ogre::Real))&Ogre::SceneNode::setPosition)
	];
}

void LuaState::bindFramework()
{
	// ShapeParams
	module(mState)
	[
		class_<ShapeParams>("ShapeParams")
		.def(constructor<>())
		.enum_("ShapeType")
		[
			value("BoxShape", 0),
			value("CylinderShape", 1),
			value("SphereShape", 2),
			value("ConvexHullShape", 3),
			value("CompoundShape", 4),
			value("GImpactShape", 5)
		]
		.def_readwrite("type", &ShapeParams::type)
		.def_readwrite("isStatic", &ShapeParams::isStatic)
		.def_readwrite("boundingBox", &ShapeParams::boundingBox)
		.def_readwrite("halfExtents", &ShapeParams::halfExtents)
		.def_readwrite("axis", &ShapeParams::axis)
		.def_readwrite("radius", &ShapeParams::radius)
	];

	// ObjectCreationParams
	module(mState)
	[
		class_<ObjectCreationParams>("ObjectCreationParams")
		.def(constructor<>())
		.def_readwrite("name", &ObjectCreationParams::name)
		.def_readwrite("mesh", &ObjectCreationParams::mesh)
		.def_readwrite("position", &ObjectCreationParams::position)
		.def_readwrite("orientation", &ObjectCreationParams::orientation)
	];

	// PhysicsObjectCreationParams
	module(mState)
	[
		class_<PhysicsObjectCreationParams, ObjectCreationParams>("PhysicsObjectCreationParams")
		.def(constructor<>())
		.def_readwrite("name", &PhysicsObjectCreationParams::name)
		.def_readwrite("mesh", &PhysicsObjectCreationParams::mesh)
		.def_readwrite("position", &PhysicsObjectCreationParams::position)
		.def_readwrite("orientation", &PhysicsObjectCreationParams::orientation)
		.def_readwrite("restitution", &PhysicsObjectCreationParams::restitution)
		.def_readwrite("friction", &PhysicsObjectCreationParams::friction)
		.def_readwrite("mass", &PhysicsObjectCreationParams::mass)
		.def_readwrite("shape", &PhysicsObjectCreationParams::shape)
	];

	// Object
	module(mState)
	[
		class_<Object>("Object")
		.def(constructor<>())
		.def("create", &Object::create)
		.def("destroy", &Object::destroy)
		.def("setPosition", &Object::setPosition)
		.def("setOrientation", &Object::setOrientation)
		.def("getPosition", &Object::getPosition)
		.def("getOrientation", &Object::getOrientation)
		.def("getEntity", &Object::getEntity)
		.def("getNode", &Object::getNode)
	];

	// PhysicsObject
	module(mState)
	[
		class_<PhysicsObject, Object>("PhysicsObject")
		.def(constructor<>())
		.def("create", &PhysicsObject::create)
		.def("destroy", &PhysicsObject::destroy)
		.def("setDamping", &PhysicsObject::setDamping)
		.def("getLinearDamping", &PhysicsObject::getLinearDamping)
	];

	// Application
	module(mState)
	[
		class_<Application>("Application")
		.def("getSceneManager", &Application::getSceneManager)
	];

	// Environment
	module(mState)
	[
		class_<Environment>("Environment")
		.def_readonly("application", &Environment::application)
	];

	globals(mState)["gEnv"] = gEnv;
	globals(mState)["SceneManager"] = gEnv->application->getSceneManager();
	globals(mState)["ObjectsInitialized"] = 0;
}