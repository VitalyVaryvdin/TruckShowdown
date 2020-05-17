#ifndef _LuaConsole_h_
#define _LuaConsole_h_

class LuaConsole
{  
public:    
    LuaConsole();    
   ~LuaConsole();
    
    void initialize();
    void shutdown();
    
    void setVisible(bool isVisible) { mIsVisible = isVisible; }
    bool isVisible(){return mIsVisible;}
    
    void print(const Ogre::String& text);
    
    bool frameStarted(const Ogre::FrameEvent& evt);
    
    void onKeyPressed(const OIS::KeyEvent& arg);
private:    
    void updateConsole();
    void updatePrompt();
    
    bool                 mIsVisible;
    bool                 mIsInitialised;
    Gorilla::Layer*      mLayer;
    Gorilla::Caption*    mPromptText;
    Gorilla::MarkupText* mConsoleText;
    Gorilla::Rectangle*  mDecoration;
    Gorilla::GlyphData*  mGlyphData;
    
    bool                 mUpdateConsole;
    bool                 mUpdatePrompt;

    unsigned int         mStartline;
    std::list<Ogre::String> lines;
    Ogre::String prompt;
};

#endif // _LuaConsole_h_