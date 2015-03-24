/* $Id$
 * sdl_engine.h
 *
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLENGINE_SDL_ENGINE_H
#define __SDLENGINE_SDL_ENGINE_H

#include "sdle_import.h"
#include "sdle_settings.h"
#include "sdle_string.h"
#include "vertex.h"
#include "structures.h"

#include <string>

#include <SDL_keyboard.h>
#include <SDL_events.h>

#ifdef SDLENGINE_CONSOLE
#	include "console.h"
#endif

namespace sdle {
/**
 * Simple Engine using SDL and OpenGL
 */
class SDLENGINE_DLLAPI SDLEngine {
private:
	/** Boolean indicating support of non-power-of-two textures */
	static bool m_supportPot;

	/** Control 2D mode */
	bool m_mode2d;

	/** Value of the near clip. Default: 20 */
	float near_clip;

	/** Value of the far clip. Default: 1000 */
	float far_clip;

	/** Current Window width */
	unsigned int m_width;

	/** Current Window height */
	unsigned int m_height;

	/** Window title */
	char caption[256];

	/** Previous BlendSource function */
	int m_2dcache_blend_src;

	/** Previous BlendDest function */
	int m_2dcache_blend_dst;

	/** Previous Blend enabled state */
	int m_2dcache_blend_on;

	/** Previous Texture enabled state */
	int m_2dcache_texture_on;

	/** Modifier pressed */
	Uint16 m_mod;

	/** Last key that was pressed */
	unsigned int m_lastkey;

	/** Initializes all internal variables */
	void init();

	/** Singleton pointer.
	 * Aka: We should have ONE and ONLY ONE SDLEngine active.
	 */
	static SDLEngine* m_singleton;

#ifdef SDLENGINE_CONSOLE
	Console m_console;
#endif

protected:
	bool keys[SDL_NUM_SCANCODES];

	/** Called before initialization. Useful when creating subclasses. */
	virtual void BeforeInit();

	/** Called after initialization. Useful when creating subclasses. */
	virtual void AfterInit();


	char screenshot_fn[256];
	bool m_screenshot;
	void SaveScreen();

public:
	SDLEngine();
	SDLEngine(const char* caption);
	virtual ~SDLEngine();

	static SDLEngine* getSingleton();

	static bool supportPot();

	void setNearClip(float);
	void setFarClip(float);

	unsigned int getWidth() const;
	unsigned int getHeight() const;

	/**
	 * Initializes SDL and OpenGL
	 * @returns bool true on success
	 */
	bool InitDisplay(const unsigned int& = 640, const unsigned int& = 480, const bool& = false, const unsigned int& bpp = 16);

	/** Closes the window */
	void CloseDisplay();

	/** Copies the backbuffer into the screen buffer and resets the transformation matrix */
	void Sync(unsigned long = 0.0f);
	virtual void WindowResize();

	/**
	 * Alters the matrix for our purposes
	 * @param sX eyeX
	 * @param sY eyeY
	 * @param sZ eyeZ
	 * @param dX destinationX
	 * @param dY destinationY
	 * @param dZ destinationZ
	 */
	void LookAt(float sX, float sY, float sZ, float dX = 0.0f, float dY = 0.0f, float dZ = 0.0f);

	/**
	 * Retrieves the world coordinates for the given screen coordinates
	 */
	void unProject(int x, int y, Vertex* ret);

	void ProcessKeyboard();
	const bool* getKeys() const;

	/**
	 * Starts 2D Mode.
	 * Useful for implementing a GUI.
	 */
	void Mode2DStart();

	/**
	 * Finishes the 2D Mode
	 */
	void Mode2DEnd();

	/**
	 * Checks if mode 2D is set.
	 */
	bool Mode2D() const;

	// Events
	virtual void evtQuit();
	virtual bool evtKeyPress(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtKeyRelease(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtMouseClick(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseRelease(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	int Screenshot(char *filename);

	// Helpers
	void enable(unsigned int);
	void disable(unsigned int);

	void enableTexture2D();
	void disableTexture2D();

#ifdef SDLENGINE_CONSOLE
	Console& getConsole();
#endif

	// Some primitives
	static void cube(const float& side = 1.0f);

	SDL_Window *hWindow;
};

}

#endif /* __SDLENGINE_SDL_ENGINE_H */
