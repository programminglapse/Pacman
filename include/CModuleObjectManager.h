#ifndef C_MODULE_OBJECT_MANAGER__H
#define C_MODULE_OBJECT_MANAGER__H

#include "AModule.h"
#include "CGrid.h"
#include "CPlayer.h"
#include "CGhostsManager.h"

#include "SDL_FontCache.h"

enum EGameState
{
	MENU = 0,
	PLAYING
};

class CModuleObjectManager : public AModule
{
public:
	CModuleObjectManager();

	bool PreUpdate() override;
	bool Update() override;

	const CGrid& GetGrid() const;
	CPlayer& Player();
	const int& GetScore() const;
	FC_Font* Font();
	const EGameState& GetGameState() const;
	void SetGameState( const EGameState& aGameState );
	CGhostsManager& GhostManager();

private:
	bool CheckGhostCollision();
	bool CheckCollectedObject();
	bool CheckFinishedGame();
	void ResetGame();

	CGrid mGrid;
	CPlayer mPlayer;
	int mScore = 0;
	FC_Font* mFont = nullptr;
	EGameState mGameState = MENU;
	CGhostsManager mGhostsManager;
};

#endif // C_MODULE_OBJECT_MANAGER__H
