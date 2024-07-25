#ifndef C_GHOSTS_MANAGER__H
#define C_GHOSTS_MANAGER__H

#include "CGhost.h"

#include <vector>
#include <queue>

class CGrid;
class CPlayer;

class CGhostsManager
{
public:
	using ghost_path = std::queue<tile>;

	CGhostsManager( const std::vector<CGhost>& aGhosts, CTexture aEatableTexture, const float& aEatableTime, const std::vector<float>& aGhostFindTime );

	std::vector<CGhost>& Ghosts();
	const std::vector<CGhost>& GetGhosts() const;

	void SetGhostsEatables( bool aEatables = true );
	SDL_Texture* GetGhostTexture( int aGhostIdx );

	void Update( const float& aDeltaTime, const CGrid& aGrid, const CPlayer& aPlayer  );
	void ResetGhost( int aGhostIdx );
	void Reset();

private:
	void UpdateGhostsPosition( const float& aDeltaTime, const CGrid& aGrid, const CPlayer& aPlayer );
	void CreatePath( int aGhostIdx, const CGrid& aGrid, const tile& aTargetTile );
	void ClearGhostPath( int aGhostIdx );

	std::vector<CGhost> mGhosts;
	std::vector<ghost_path> mGhostsPath;
	std::vector<float> mGhostFindTime;
	std::vector<float> mCurrentGhostFindTime;
	 // Same for all ghosts
	CTexture mEatableTexture;
	float mEatableTime;

};

#endif // C_GHOSTS_MANAGER__H
