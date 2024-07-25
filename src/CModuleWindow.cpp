#include "CModuleWindow.h"

#include "SDL.h"

#include "Globals.h"

SDL_Window* CreateSDLWindow();

CModuleWindow::CModuleWindow() :
	mSDLWindow( CreateSDLWindow() )
{
}

bool CModuleWindow::Init()
{
	return mSDLWindow != nullptr;
}

bool CModuleWindow::Update()
{
	return true;
}

bool CModuleWindow::Clear()
{
	SDL_DestroyWindow( mSDLWindow );
    mSDLWindow = nullptr;

	return true;
}

SDL_Window* CModuleWindow::SDLWindow()
{
	return mSDLWindow;
}

SDL_Window* CreateSDLWindow()
{
	return SDL_CreateWindow( "PACMAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GRID_SIZE, GRID_SIZE, SDL_WINDOW_SHOWN );
}
