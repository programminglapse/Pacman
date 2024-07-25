#include "CModuleRenderer.h"

#include "CManager.h"
#include "CModuleWindow.h"
#include "CModuleObjectManager.h"
#include "Globals.h"

#include "SDL.h"

SDL_Renderer* CreateSDLRenderer();

CModuleRenderer::CModuleRenderer() :
	mSDLRenderer( CreateSDLRenderer() )
{
}

bool CModuleRenderer::Init()
{
	mMap = CreateMapTexture(manager->mModuleObjectManager->GetGrid().GetMap());

	return mSDLRenderer != nullptr;
}

bool CModuleRenderer::PreUpdate()
{
	SDL_RenderClear( mSDLRenderer );
	return true;
}

bool CModuleRenderer::Update()
{
	// Render map
	RenderTexture( mMap, {0,0,GRID_SIZE,GRID_SIZE} );
	// Render coins
	RenderSpecialTiles();

	// Render Player
	RenderTexture( manager->mModuleObjectManager->Player().SpriteAnimation().Texture().SDLTexture(),
			manager->mModuleObjectManager->Player().GetRect(), manager->mModuleObjectManager->Player().GetRotationAngle() );

	//Render score
	const auto score = "Score: " +std::to_string( manager->mModuleObjectManager->GetScore() );
	FC_Draw( manager->mModuleObjectManager->Font(), mSDLRenderer, 10, 0, score.c_str() );

	// Render click to play message
	if( manager->mModuleObjectManager->GetGameState() == MENU )
		FC_Draw( manager->mModuleObjectManager->Font(), mSDLRenderer, 452, 565, "Click to play" );

	// Render ghosts
	auto& ghosts = manager->mModuleObjectManager->GhostManager().Ghosts();
	for( auto ghostIt = ghosts.begin(); ghostIt != ghosts.end(); ++ghostIt )
		RenderTexture( manager->mModuleObjectManager->GhostManager().GetGhostTexture( std::distance( ghosts.begin(), ghostIt ) ), (*ghostIt).GetRect() );

	SDL_RenderPresent( mSDLRenderer );
	return true;
}

bool CModuleRenderer::Clear()
{
	SDL_DestroyRenderer( mSDLRenderer );
	mSDLRenderer = nullptr;

	SDL_DestroyTexture( mMap );
	mMap = nullptr;

	return true;
}

SDL_Renderer* CModuleRenderer::SDLRenderer()
{
	return mSDLRenderer;
}

void CModuleRenderer::RenderTexture( SDL_Texture* aTexture, const SDL_Rect& aRect, int aRotationAngle )
{
	if( aRotationAngle )
		SDL_RenderCopyEx( mSDLRenderer, aTexture, NULL, &aRect, aRotationAngle, 0, SDL_FLIP_VERTICAL );
	else
		SDL_RenderCopy( mSDLRenderer, aTexture, NULL, &aRect );
}

void CModuleRenderer::RenderEmptyTile( int aX, int aY )
{
	RenderTile( aX*TILE_SIZE, aY*TILE_SIZE, TILE_SIZE, TILE_SIZE, {0,0,0,255} );
}

void CModuleRenderer::RenderWallTile( int aX, int aY )
{
	const auto offset = (TILE_SIZE-BORDER_TILE_SIZE)/2;
	const auto offset2 = (TILE_SIZE-INTERIOR_TILE_SIZE)/2;
	RenderTile( aX*TILE_SIZE, aY*TILE_SIZE, TILE_SIZE, TILE_SIZE, {0,0,0,255} );
	RenderTile( aX*TILE_SIZE+offset, aY*TILE_SIZE+offset, BORDER_TILE_SIZE, BORDER_TILE_SIZE, {0,0,255,255} );
	RenderTile( aX*TILE_SIZE+offset2, aY*TILE_SIZE+offset2, INTERIOR_TILE_SIZE, INTERIOR_TILE_SIZE, {0,0,0,255} );
}

void CModuleRenderer::RenderGhostWallTile( int aX, int aY )
{
	const auto offset = (TILE_SIZE-BORDER_TILE_SIZE)/2;
	const auto offset2 = (TILE_SIZE-INTERIOR_TILE_SIZE)/2;
	RenderTile( aX*TILE_SIZE, aY*TILE_SIZE, TILE_SIZE, TILE_SIZE, {0,0,0,255} );
	RenderTile( aX*TILE_SIZE+offset, aY*TILE_SIZE+offset, BORDER_TILE_SIZE, BORDER_TILE_SIZE, {0,255,0,255} );
	RenderTile( aX*TILE_SIZE+offset2, aY*TILE_SIZE+offset2, INTERIOR_TILE_SIZE, INTERIOR_TILE_SIZE, {0,0,0,255} );
}

void CModuleRenderer::RenderSpecialTiles()
{
	const auto& map = manager->mModuleObjectManager->GetGrid().GetMap();

	for( int i = 0; i < map.size(); ++i )
		for( int j = 0; j < map[i].size(); ++j )
			if( map[i][j] == 2 || map[i][j] == 3 )
			{
				const auto& multiplier = map[i][j] == 3 ? 2 : 1;
				const auto color = map[i][j] == 3 ? 255 : 150;
				SDL_SetRenderDrawColor( mSDLRenderer, color, color, 0, 255 );
				SDL_Rect rect = { i*TILE_SIZE+(TILE_SIZE-COIN_SIZE*multiplier)/2, j*TILE_SIZE+(TILE_SIZE-COIN_SIZE*multiplier)/2, COIN_SIZE*multiplier, COIN_SIZE*multiplier };
				SDL_RenderFillRect( mSDLRenderer, &rect );
			}
}

void CModuleRenderer::RenderCoinTile( int aX, int aY )
{
	RenderTile( aX*TILE_SIZE, aY*TILE_SIZE, TILE_SIZE, TILE_SIZE, {0,0,0,255} );
}

void CModuleRenderer::RenderTile( int aX, int aY, int aWidth, int aHeight, const SDL_Color& aColor )
{
	SDL_SetRenderDrawColor( mSDLRenderer, aColor.r, aColor.g, aColor.b, aColor.a );
	SDL_Rect rect = { aX, aY, aWidth, aHeight };
	SDL_RenderFillRect( mSDLRenderer, &rect );
}

SDL_Renderer* CreateSDLRenderer()
{
	return SDL_CreateRenderer( manager->mModuleWindow->SDLWindow(), -1, SDL_RENDERER_ACCELERATED );
}

SDL_Texture* CModuleRenderer::CreateMapTexture( const CGrid& aGrid )
{
	SDL_Texture* texture = SDL_CreateTexture( mSDLRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GRID_SIZE, GRID_SIZE );
    SDL_SetRenderTarget( mSDLRenderer, texture );
	const auto& map = aGrid.GetMap();
	for( int i = 0; i < map.size(); ++i )
	{
		for( int j = 0; j < map[i].size(); ++j )
		{
			switch( map[i][j] )
			{
				case 0:
				case 2:
					RenderEmptyTile( i, j );
					break;
				case 1:
					RenderWallTile( i, j );
					break;
				case 4:
					RenderGhostWallTile( i, j );
					break;
			}
		}
	}
    SDL_SetRenderTarget( mSDLRenderer, nullptr );
	return texture;
}
