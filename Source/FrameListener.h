#ifndef _FrameListener_h_
#define _FrameListener_h_

class FrameListener : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
	FrameListener();
	virtual ~FrameListener();

	void initialize();
	void shutdown();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
private:
};

#endif // _FrameListener_h_