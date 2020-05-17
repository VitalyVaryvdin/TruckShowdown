#ifndef _InputListener_h_
#define _InputListener_h_

class InputListener : public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputListener();
	virtual ~InputListener();

	void initialize();
	void shutdown();

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual bool keyReleased(const OIS::KeyEvent& arg);

	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	int getXAbs() { return mXAbs; }
	int getYAbs() { return mYAbs; }

	OIS::InputManager* getInputManager() { return mInputManager; }
	OIS::Mouse* getMouse() { return mMouse; }
	OIS::Keyboard* getKeyboard() { return mKeyboard; }
private:
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	int mXAbs;
	int mYAbs;
};

#endif // _InputListener_h_