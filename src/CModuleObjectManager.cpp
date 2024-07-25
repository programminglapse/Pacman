#include "CModuleObjectManager.h"

#include "CManager.h"
#include "CModuleRenderer.h"
#include "CModuleTimer.h"
#include "Globals.h"

#include <algorithm>

CGrid::map CreateMap();
CPlayer CreatePlayer();
CGhostsManager CreateGhostsManager();
bool CheckCollision( const SDL_Rect& aLeft, const SDL_Rect& aRight );

CModuleObjectManager::CModuleObjectManager() :
	mGrid( CreateMap() ),
	mPlayer( CreatePlayer() ),
	mFont( FC_CreateFont() ),
	mGhostsManager( CreateGhostsManager() )
{
	FC_LoadFont( mFont, manager->mModuleRenderer->SDLRenderer(), "../assets/fonts/font.ttf", 30, FC_MakeColor( 255,255,255,255), TTF_STYLE_NORMAL );
}

bool CModuleObjectManager::PreUpdate()
{
	if( mGameState == PLAYING )
	{
		mPlayer.SpriteAnimation().CountFrame();
		mPlayer.UpdatePosition( manager->mModuleTimer->GetDeltaTime(), mGrid );
		mGhostsManager.Update( manager->mModuleTimer->GetDeltaTime(), mGrid, mPlayer );

		if( CheckGhostCollision() || ( CheckCollectedObject() && CheckFinishedGame() ) )
		{
			mGameState = MENU;
			ResetGame();
		}
	}

	return true;
}

bool CModuleObjectManager::Update()
{
	return true;
}

const CGrid& CModuleObjectManager::GetGrid() const
{
	return mGrid;
}

CPlayer& CModuleObjectManager::Player()
{
	return mPlayer;
}

const int& CModuleObjectManager::GetScore() const
{
	return mScore;
}

FC_Font* CModuleObjectManager::Font()
{
	return mFont;
}

const EGameState& CModuleObjectManager::GetGameState() const
{
	return mGameState;
}

void CModuleObjectManager::SetGameState( const EGameState& aGameState )
{
	mGameState = aGameState;
}

CGhostsManager& CModuleObjectManager::GhostManager()
{
	return mGhostsManager;
}

bool CModuleObjectManager::CheckGhostCollision()
{
	for( auto ghostIt = mGhostsManager.Ghosts().begin(); ghostIt != mGhostsManager.Ghosts().end(); ++ghostIt )
		if( CheckCollision( mPlayer.GetRect(), (*ghostIt).GetRect() ) )
		{
			if( !(*ghostIt).IsEatable() )
				return true;
			else
			{
				mScore += GHOST_EAT_POINTS;
				mGhostsManager.ResetGhost( std::distance( mGhostsManager.Ghosts().begin(), ghostIt ) );
			}
		}
	return false;
}

bool CModuleObjectManager::CheckCollectedObject()
{
	auto& map = mGrid.Map();
	const int playerXPos = mPlayer.GetRect().x/TILE_SIZE;
	const int playerYPos = mPlayer.GetRect().y/TILE_SIZE;

	if( map[playerXPos][playerYPos] == 2 )
	{
		mScore += POINTS_PER_COIN;
		map[playerXPos][playerYPos] = 0;
		return true;
	}
	else if( map[playerXPos][playerYPos] == 3 )
	{
		mGhostsManager.SetGhostsEatables();
		map[playerXPos][playerYPos] = 0;
	}

	return false;
}

bool CModuleObjectManager::CheckFinishedGame()
{
	const auto& map = mGrid.GetMap();
	return std::all_of( map.begin(), map.end(), [&]( const std::vector<int> aVect ){ return std::all_of( aVect.begin(), aVect.end(), [&]( int i ){ return i != 2; } ); } );
}

void CModuleObjectManager::ResetGame()
{
	mGrid = CGrid( CreateMap() );
	mPlayer.ResetToInitialPosition();
	mScore = 0;
	mGhostsManager.Reset();
}

CGrid::map CreateMap()
{
	return
	{
			{1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,2, 1,0,0,0,1, 1,1,1,1,1, 1,1,1  },
			{1,2,2,2,2, 2,2,2,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,2,2,2,2, 2,3,1  },
			{1,2,1,1,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,1,1,1,1, 1,2,1  },
			{1,2,1,0,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,1,1,2,2, 2,2,1  },
			{1,2,1,0,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,2,2,2,1, 1,2,1  },
                                                                         
			{1,2,1,1,1, 2,1,1,2,1, 1,1,1,1,2, 1,1,1,1,1, 2,1,1,1,1, 1,2,1  },
			{1,2,2,2,2, 2,2,2,2,2, 2,2,3,2,2, 2,2,2,2,2, 2,2,2,2,2, 2,2,1  },
			{1,2,1,1,1, 2,1,1,1,1, 1,1,1,1,2, 1,1,1,1,2, 1,1,2,1,1, 1,1,1  },
			{1,2,1,0,1, 2,1,1,1,1, 1,1,1,1,2, 1,1,1,1,2, 1,1,2,1,1, 1,1,1  },
			{1,2,1,0,1, 2,2,2,2,1, 1,2,2,2,2, 2,2,2,2,2, 1,1,2,2,2, 2,2,1  },
                                                                         
			{1,2,1,0,1, 2,1,1,2,1, 1,2,1,1,1, 1,1,2,1,2, 1,1,2,1,1, 1,2,1  },
			{1,2,1,1,1, 2,1,1,2,1, 1,2,1,0,0, 0,1,2,1,2, 1,1,3,1,1, 1,2,1  },
			{1,2,2,2,2, 2,1,1,2,2, 2,2,1,0,0, 0,1,2,1,2, 2,2,2,2,2, 2,2,1  },
			{1,1,1,1,1, 2,1,1,1,1, 1,2,4,0,0, 0,1,2,1,1, 1,1,1,2,1, 1,1,1  },
			{1,1,1,1,1, 2,1,1,1,1, 1,2,4,0,0, 0,1,2,1,1, 1,1,1,2,1, 1,1,1  },
                                                                         
			{1,2,2,2,2, 2,1,1,2,2, 2,2,1,0,0, 0,1,2,1,2, 2,2,2,2,2, 2,2,1  },
			{1,2,1,1,1, 2,1,1,2,1, 1,2,1,0,0, 0,1,2,1,2, 1,1,2,1,1, 1,2,1  },
			{1,2,1,0,1, 2,1,1,2,1, 1,2,1,1,1, 1,1,2,1,2, 1,1,2,1,1, 1,2,1  },
			{1,2,1,0,1, 2,2,2,2,1, 1,2,2,2,2, 2,2,2,2,2, 1,1,2,2,2, 2,2,1  },
			{1,2,1,0,1, 2,1,1,1,1, 1,1,1,1,2, 1,1,1,1,2, 1,1,2,1,1, 1,1,1  },
                                                                         
			{1,2,1,1,1, 2,1,1,1,1, 1,1,1,1,2, 1,1,1,1,2, 1,1,2,1,1, 1,1,1  },
			{1,2,2,2,2, 2,2,2,2,2, 2,2,2,2,2, 2,2,2,2,2, 2,2,2,2,2, 2,2,1  },
			{1,2,1,1,1, 2,1,1,2,1, 1,1,1,1,2, 1,1,1,1,1, 2,1,1,1,1, 1,2,1  },
			{1,2,1,0,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,2,2,2,1, 1,2,1  },
			{1,2,1,0,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,1,1,2,2, 2,2,1  },
                                                                         
			{1,2,1,1,1, 2,1,1,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,1,1,1,1, 1,2,1  },
			{1,2,2,3,2, 2,2,2,2,1, 0,0,0,1,2, 1,0,0,0,1, 2,2,2,2,3, 2,2,1  },
			{1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,2, 1,0,0,0,1, 1,1,1,1,1, 1,1,1  },
	};
}

CPlayer CreatePlayer()
{
	return CPlayer( CSpriteAnimation(
			{ CTexture("../assets/player1.png", manager->mModuleRenderer->SDLRenderer() ),
			CTexture("../assets/player2.png", manager->mModuleRenderer->SDLRenderer() ),
			CTexture("../assets/player3.png", manager->mModuleRenderer->SDLRenderer() ) },
			PLAYER_FRAMES_PER_SPRITE ), {40,40,PLAYER_SIZE,PLAYER_SIZE}, 0.0f );
}

CGhostsManager CreateGhostsManager()
{
	return CGhostsManager( { CGhost( {
			{ NONE, CTexture("../assets/ghosts/ghost1-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ LEFT, CTexture("../assets/ghosts/ghost1-LEFT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ RIGHT, CTexture("../assets/ghosts/ghost1-RIGHT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ UP, CTexture("../assets/ghosts/ghost1-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ DOWN, CTexture("../assets/ghosts/ghost1-DOWN.png", manager->mModuleRenderer->SDLRenderer() ) },
			}, {GHOST1_INIT_X,GHOST1_INIT_Y,GHOST_SIZE, GHOST_SIZE} ), 
			CGhost( {
			{ NONE, CTexture("../assets/ghosts/ghost2-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ LEFT, CTexture("../assets/ghosts/ghost2-LEFT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ RIGHT, CTexture("../assets/ghosts/ghost2-RIGHT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ UP, CTexture("../assets/ghosts/ghost2-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ DOWN, CTexture("../assets/ghosts/ghost2-DOWN.png", manager->mModuleRenderer->SDLRenderer() ) },
			}, {GHOST2_INIT_X,GHOST2_INIT_Y,GHOST_SIZE, GHOST_SIZE} ),
			CGhost( {
			{ NONE, CTexture("../assets/ghosts/ghost3-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ LEFT, CTexture("../assets/ghosts/ghost3-LEFT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ RIGHT, CTexture("../assets/ghosts/ghost3-RIGHT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ UP, CTexture("../assets/ghosts/ghost3-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ DOWN, CTexture("../assets/ghosts/ghost3-DOWN.png", manager->mModuleRenderer->SDLRenderer() ) },
			}, {GHOST3_INIT_X,GHOST3_INIT_Y,GHOST_SIZE, GHOST_SIZE} ),
			CGhost( {
			{ NONE, CTexture("../assets/ghosts/ghost4-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ LEFT, CTexture("../assets/ghosts/ghost4-LEFT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ RIGHT, CTexture("../assets/ghosts/ghost4-RIGHT.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ UP, CTexture("../assets/ghosts/ghost4-UP.png", manager->mModuleRenderer->SDLRenderer() ) },
			{ DOWN, CTexture("../assets/ghosts/ghost4-DOWN.png", manager->mModuleRenderer->SDLRenderer() ) },
			}, {GHOST4_INIT_X,GHOST4_INIT_Y,GHOST_SIZE, GHOST_SIZE} )
			},
			CTexture( "../assets/ghosts/Eatable.png", manager->mModuleRenderer->SDLRenderer() ), 2.0f, { 1.0f, 20.0f, 50.0f, 100.0f } );
}

bool CheckCollision( const SDL_Rect& aLeft, const SDL_Rect& aRight )
{
	SDL_Rect result;
	return SDL_IntersectRect( &aLeft, &aRight, &result );
}
