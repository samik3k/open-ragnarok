/* $Id$ */
#ifndef __RO_ENGINE_H
#define __RO_ENGINE_H

#include "sdl_engine.h"
#include "ro_object_cache.h"
#include "gl_object_cache.h"
#include "texturemanager.h"
#include "file_manager.h"
#include "frustum.h"
#include "camera.h"

#include "ro/ro.h"
#include "rogl/rogl.h"
#ifdef _MSC_VER
#	include <SDL.h>
#else
#	include <SDL/SDL.h>
#endif
#include "gui/gui.h"
#include "roengine/full_act.h"

class ROEngine : public SDLEngine {
protected:
	TextureManager m_texturemanager;
	GLObjectCache m_gl_objects;
	ROObjectCache m_ro_objects;
	FileManager m_filemanager;
	Frustum m_frustum;
	GUI::Gui m_gui;
	FullAct cursor;

	/** Indicates the current cursor sprite in use */
	int m_cursorSprite;

	/** Stores the tick count to handle mouse animation */
	long m_cursorTick;

	bool m_quit;

	virtual void HandleKeyboard();
	virtual void BeforeRun();
	virtual void AfterRun();
	virtual void BeforeDraw();
	virtual void AfterDraw();
	virtual void AfterInit();

	long lastTick;
	long tickDelay;

	Camera cam;

	int mousex;
	int mousey;

public:
	ROEngine(const std::string& name = "");

	~ROEngine();
	void Run();

	void ReadIni(const std::string& name = "data.ini");

	virtual void evtQuit();

	virtual bool evtKeyPress(const int& key, const int& mod = 0);
	virtual bool evtKeyRelease(const int& key, const int& mod = 0);
	virtual bool evtMouseClick(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseRelease(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	TextureManager& getTextureManager();
	GLObjectCache& getGLObjects();
	ROObjectCache& getROObjects();
	FileManager& getFileManager();
	Frustum& getFrustum();
	FullAct& getCursor();
	void setCursor(FullAct& cur);

	/** Process mouse animation and drawing */
	void ProcessMouse(int xless = 0, int yless = 0);

	/** Retrieve mouse X position */
	int getMouseX();

	/** Retrieve mouse Y position */
	int getMouseY();
};

#ifdef _MSC_VER
#	ifndef __ROENGINE_NO_AUTOIMPORT_LIB__
#		pragma comment(lib, "roengine.lib")
#	endif
#endif

#endif /* __RO_ENGINE_H */