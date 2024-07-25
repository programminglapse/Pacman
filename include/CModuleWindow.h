#ifndef C_MODULE_WINDOW__H
#define C_MODULE_WINDOW__H

#include "AModule.h"

class SDL_Window;

class CModuleWindow : public AModule
{
public:
	CModuleWindow();

	bool Init() override;
	bool Update() override;
	bool Clear() override;

	SDL_Window* SDLWindow();

private:
	SDL_Window* mSDLWindow;
};

#endif // C_MODULE_WINDOW__H
