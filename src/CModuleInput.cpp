#include "CModuleInput.h"

#include "CManager.h"
#include "CModuleObjectManager.h"
#include "Globals.h"

#include "SDL.h"

bool CModuleInput::Update()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 )
	{
		if( e.type == SDL_QUIT )
			return false;
		if( e.type == SDL_MOUSEBUTTONDOWN && manager->mModuleObjectManager->GetGameState() == MENU )
			manager->mModuleObjectManager->SetGameState( PLAYING );
	}

	const auto keystates = SDL_GetKeyboardState(NULL);

	if( keystates[SDL_SCANCODE_LEFT] )
		manager->mModuleObjectManager->Player().SetDirection( LEFT );
	else if( keystates[SDL_SCANCODE_RIGHT] )
		manager->mModuleObjectManager->Player().SetDirection( RIGHT );
	else if( keystates[SDL_SCANCODE_UP] )
		manager->mModuleObjectManager->Player().SetDirection( UP );
	else if( keystates[SDL_SCANCODE_DOWN] )
		manager->mModuleObjectManager->Player().SetDirection( DOWN );
	
	return true;
}
