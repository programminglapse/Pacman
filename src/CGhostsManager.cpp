#include "CGhostsManager.h"

#include "CGrid.h"
#include "CPlayer.h"
#include "Globals.h"

#include <stack>

bool IsWalkable( const CGrid& aGrid, const tile& aTile );
void InvertPath( std::queue<tile>& aPath );
tile FindRandomWalkableTile( const CGrid& aGrid, const tile& aTile );

CGhostsManager::CGhostsManager( const std::vector<CGhost>& aGhosts, CTexture aEatableTexture, const float& aEatableTime, const std::vector<float>& aGhostFindTime  ) :
	mGhosts( aGhosts ),
	mEatableTexture( aEatableTexture ),
	mEatableTime( aEatableTime ),
	mGhostsPath( mGhosts.size(), ghost_path{} ),
	mGhostFindTime( aGhostFindTime ),
	mCurrentGhostFindTime( mGhosts.size(), 0 )
{
}

std::vector<CGhost>& CGhostsManager::Ghosts()
{
	return mGhosts;
}

const std::vector<CGhost>& CGhostsManager::GetGhosts() const
{
	return mGhosts;
}

void CGhostsManager::SetGhostsEatables( bool aEatables )
{
	for( auto ghostIt = mGhosts.begin(); ghostIt != mGhosts.end(); ++ghostIt )
	{
		(*ghostIt).SetEatableTime( mEatableTime );
		ClearGhostPath( std::distance( mGhosts.begin(), ghostIt ) );
	}
}

void CGhostsManager::ClearGhostPath( int aGhostIdx )
{
	ghost_path p;
	p.push( mGhostsPath[aGhostIdx].front() );
	mGhostsPath[aGhostIdx] = p;
}

SDL_Texture* CGhostsManager::GetGhostTexture( int aGhostIdx )
{
	return mGhosts.at(aGhostIdx).IsEatable() ? mEatableTexture.SDLTexture() : mGhosts.at(aGhostIdx).Texture().SDLTexture();
}

void CGhostsManager::Update( const float& aDeltaTime, const CGrid& aGrid, const CPlayer& aPlayer )
{
	static const auto eatTimeDecrease = 0.005f;
	static const auto findTimeDecrease = 0.05f;
	for( auto& ghost : mGhosts )
	{
		if( ghost.IsEatable() )
			ghost.DecreaseEatableTime( eatTimeDecrease*aDeltaTime );
	}
	for( auto& currentGhostFindTime : mCurrentGhostFindTime )
		currentGhostFindTime -= findTimeDecrease*aDeltaTime;

	UpdateGhostsPosition( aDeltaTime, aGrid, aPlayer );
}

void CGhostsManager::UpdateGhostsPosition( const float& aDeltaTime, const CGrid& aGrid, const CPlayer& aPlayer )
{
	for( auto ghostIt = mGhosts.begin(); ghostIt != mGhosts.end(); ++ghostIt )
	{
		const tile ghostTile = { (*ghostIt).GetPos().x/TILE_SIZE, (*ghostIt).GetPos().y/TILE_SIZE };
		const auto ghostIdx = std::distance( mGhosts.begin(), ghostIt );

		if( mGhostsPath[ghostIdx].empty() )
		{
			const tile playerTile = { aPlayer.GetRect().x/TILE_SIZE, aPlayer.GetRect().y/TILE_SIZE };
			CreatePath( ghostIdx, aGrid, mGhosts[ghostIdx].IsEatable() ? FindRandomWalkableTile( aGrid, ghostTile ) : playerTile );
		}
		else if( mCurrentGhostFindTime[ghostIdx] < 0 )
		{
			ClearGhostPath( ghostIdx );
			mCurrentGhostFindTime[ghostIdx] = mGhostFindTime[ghostIdx];
		}
		else
		{
			const auto targetTile = mGhostsPath[ghostIdx].front();
			if( targetTile != ghostTile )
			{
				if( targetTile.first < ghostTile.first )
					(*ghostIt).SetDirection( LEFT );
				else if( targetTile.first > ghostTile.first )
					(*ghostIt).SetDirection( RIGHT );
				else if( targetTile.second > ghostTile.second )
					(*ghostIt).SetDirection( DOWN );
				else
					(*ghostIt).SetDirection( UP );
			}

			if( (*ghostIt).MoveToTile( aDeltaTime, targetTile ) )
				mGhostsPath[ghostIdx].pop();
		}
	}
}

void CGhostsManager::ResetGhost( int aGhostIdx )
{
	mGhosts[aGhostIdx].SetPos({ GHOST1_INIT_X+40.0f*aGhostIdx, GHOST1_INIT_Y });
	mGhostsPath[aGhostIdx] = {};
}

void CGhostsManager::CreatePath( int aGhostIdx, const CGrid& aGrid, const tile& aTargetTile )
{
	const tile ghostTile = { mGhosts[aGhostIdx].GetPos().x/TILE_SIZE, mGhosts[aGhostIdx].GetPos().y/TILE_SIZE };

	ghost_path result;
	ghost_path path;
	std::map<tile,bool> visited;
	std::map<tile,tile> cameFrom;

	path.push( ghostTile );
	visited[ghostTile] = true;
	cameFrom[ghostTile] = {-1,-1};

	while( !path.empty() )
	{
		auto currentTile = path.front();
		path.pop();

		if( currentTile == aTargetTile )
		{
			result.push( currentTile );
			while( cameFrom[currentTile].first != -1 )
			{
				currentTile = cameFrom[currentTile];
				result.push( currentTile );
			}
			break;
		}

		const static std::vector<tile> directions = { {0,-1}, {0,1}, {-1,0}, {1,0} };
		for( const auto& dir : directions )
		{
			const tile newTile = { currentTile.first+dir.first, currentTile.second+dir.second };
			if( !visited[newTile] && IsWalkable( aGrid, newTile ) )
			{
				visited[newTile] = true;
				path.push( newTile );
				cameFrom[newTile] = currentTile;
			}
		}
	}

	InvertPath( result );
	mGhostsPath[aGhostIdx] = result;
}

void CGhostsManager::Reset()
{
	for( auto ghostIt = mGhosts.begin(); ghostIt != mGhosts.end(); ++ghostIt )
	{
		ResetGhost( std::distance( mGhosts.begin(), ghostIt ) );
		(*ghostIt).DecreaseEatableTime( mEatableTime );
	}
}

bool IsWalkable( const CGrid& aGrid, const tile& aTile )
{
	if( aTile.first < 0 || aTile.second < 0 || aTile.first > GRID_SIZE/TILE_SIZE-1 || aTile.second > GRID_SIZE/TILE_SIZE-1 )
		return false;
	return aGrid.GetMap()[aTile.first][aTile.second] != 1;
}

void InvertPath( std::queue<tile>& aPath )
{
	std::stack<tile> s;
	while( !aPath.empty() )
	{
		s.push(aPath.front());
		aPath.pop();
	}
	while( !s.empty() )
	{
		aPath.push( s.top() );
		s.pop();
	}
}

tile FindRandomWalkableTile( const CGrid& aGrid, const tile& aTile )
{
	std::map<tile,bool> visited;
	visited[aTile] = true;
	tile farTile = aTile;
	const static std::vector<tile> directions = { {0,-1}, {0,1}, {-1,0}, {1,0} };
	auto tileCount = 0;
	while( tileCount < 15 )
	{
		tile nearestTile = {-1,-1};
		for( const auto& dir : directions )
		{
			const tile newTile = { farTile.first+dir.first, farTile.second+dir.second };
			if( !visited[newTile] && IsWalkable( aGrid, newTile ) )
			{
				nearestTile = newTile;
				visited[newTile] = true;
				farTile = newTile;
				++tileCount;
				break;
			}
		}
		if( nearestTile.first == -1 )
			return farTile;
	}
	
	return farTile;
}
