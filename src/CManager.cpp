#include "CManager.h"

#include "AModule.h"
#include "CModuleWindow.h"
#include "CModuleRenderer.h"
#include "CModuleInput.h"
#include "CModuleObjectManager.h"
#include "CModuleTimer.h"

#include "SDL.h"

#include <iostream>

bool InitSDL();

void CManager::Play()
{
	if( !InitSDL() )
		return;

	InitModules();

	for( auto& module : mModules )
		if( !module->Init() )
			return;

	bool playing = true;
	while( playing )
	{
		for( auto& module : mModules )
		{
			if( !module->PreUpdate() )
			{
				playing = false;
				break;
			}
		}
		
		for( auto& module : mModules )
		{
			if( !module->Update() )
			{
				playing = false;
				break;
			}
		}
	}

	for( auto& module : mModules )
		if( !module->Clear() )
			return;
}

void CManager::InitModules()
{
	mModules.push_back( mModuleTimer = new CModuleTimer() );
	mModules.push_back( mModuleWindow = new CModuleWindow() );
	mModules.push_back( mModuleRenderer = new CModuleRenderer() );
	mModules.push_back( mModuleInput = new CModuleInput() );
	mModules.push_back( mModuleObjectManager = new CModuleObjectManager() );
}

bool InitSDL()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}
