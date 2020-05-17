#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

#define CONSOLE_FONT_INDEX 9
#define CONSOLE_LINE_LENGTH 200
#define CONSOLE_LINE_COUNT 15

static const unsigned char legalchars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"'#%&/()=?[]\\*-_.:,; ";

LuaConsole::LuaConsole()
	: mIsVisible(false), 
	mIsInitialised(false), 
	mUpdateConsole(false), 
	mUpdatePrompt(false),
	mStartline(0)
{
	initialize();
}
 
LuaConsole::~LuaConsole()
{
	if(mIsInitialised)
		shutdown();
}

void LuaConsole::initialize()
{
	mLayer = gEnv->application->getScreen()->createLayer(15);
	mGlyphData = mLayer->_getGlyphData(CONSOLE_FONT_INDEX); // Font.CONSOLE_FONT_INDEX

	mConsoleText = mLayer->createMarkupText(CONSOLE_FONT_INDEX,  10,10, Ogre::StringUtil::BLANK);
	mConsoleText->width(gEnv->application->getScreen()->getWidth() - 10);
	mPromptText = mLayer->createCaption(CONSOLE_FONT_INDEX,  10,10, "> _");
	mDecoration = mLayer->createRectangle(8,8, gEnv->application->getScreen()->getWidth() - 16, mGlyphData->mLineHeight );
	mDecoration->background_gradient(Gorilla::Gradient_NorthSouth, Gorilla::rgb(128,128,128,128), Gorilla::rgb(64,64,64,128));
	mDecoration->border(2, Gorilla::rgb(128,128,128,128));
   
	mIsInitialised = true;
}

void LuaConsole::shutdown()
{
	if(!mIsInitialised)
		return;
 
	mIsInitialised = false;

	gEnv->application->getScreen()->destroy(mLayer); 
}


void LuaConsole::onKeyPressed(const OIS::KeyEvent &arg)
{
	if(!mIsVisible)
		return;
	
	if(arg.key == OIS::KC_RETURN || arg.key == OIS::KC_NUMPADENTER)
	{
		gEnv->luaInterpreter->executeString(prompt);

		if(prompt.size() != 0)
			print("%3> " + prompt + "%R");
		if(!gEnv->luaInterpreter->getOutput().empty())
			print("\t" + gEnv->luaInterpreter->getOutput());


		prompt.clear();
		mUpdateConsole = true;
		mUpdatePrompt = true;
	}
	else if(arg.key == OIS::KC_BACK)
	{
		if(prompt.size())
		{
			prompt.erase(prompt.end() - 1);
			mUpdatePrompt = true;
		}
	}
	else if(arg.key == OIS::KC_PGUP)
	{
		if(mStartline>0)
			mStartline--;
		mUpdateConsole = true;
	}
	else if(arg.key == OIS::KC_PGDOWN)
	{
		if(mStartline<lines.size())
			mStartline++;
		mUpdateConsole = true;
	}
	else
	{
		for(unsigned int c=0;c<sizeof(legalchars);c++)
		{
			if(legalchars[c]==arg.text)
			{
				prompt+=arg.text;
				break;
			}
		}
		mUpdatePrompt = true;
	} 
}

bool LuaConsole::frameStarted(const Ogre::FrameEvent& evt)
{
   if(mUpdateConsole)
	   updateConsole();
    
   if(mUpdatePrompt)
	   updatePrompt();

   if(!mIsVisible)
	   mLayer->hide();
   else if(!mLayer->isVisible())
	   mLayer->show();
   
   return true;
}

void LuaConsole::updateConsole()
{
	mUpdateConsole = false;

	std::stringstream text;
	std::list<Ogre::String>::iterator i,start,end;
	
	if(mStartline>lines.size())
		mStartline=lines.size();
	
	int lcount = 0;
	start = lines.begin();

	for(unsigned int c = 0; c < mStartline; c++)
		start++;

	end=start;

	for(unsigned int c = 0;c < CONSOLE_LINE_COUNT; c++)
	{
		if(end == lines.end())
			break;
		end++;
	}
	
	for(i = start; i !=end; i++)
	{
		lcount++;
		text << (*i) << "\n";
	}
	mConsoleText->text(text.str());
	
	// Move prompt downwards.
	mPromptText->top(10 + (lcount * mGlyphData->mLineHeight));
 
	// Change background height so it covers the text and prompt
	mDecoration->height(((lcount+1) * mGlyphData->mLineHeight) + 4);
	
	mConsoleText->width(gEnv->application->getScreen()->getWidth() - 20);
	mDecoration->width(gEnv->application->getScreen()->getWidth() - 16);
	mPromptText->width(gEnv->application->getScreen()->getWidth() - 20);
}

void LuaConsole::updatePrompt()
{
	mUpdatePrompt = false;
	std::stringstream text;
	text << "> " << prompt << "_";
	mPromptText->text(text.str());
}

void LuaConsole::print(const Ogre::String &text)
{
   const char *str = text.c_str();
   int len = text.length();
   Ogre::String line;

   for(int c = 0;c < len; c++)
   {
      if(str[c] == '\n' || line.length() >= CONSOLE_LINE_LENGTH)
	  {
         lines.push_back(line);
         line="";
      }

      if(str[c] != '\n')
         line += str[c];
   }

   if(line.length())
      lines.push_back(line);
   if(lines.size() > CONSOLE_LINE_COUNT)
      mStartline = lines.size() - CONSOLE_LINE_COUNT;
   else
      mStartline = 0;
   mUpdateConsole = true;
}