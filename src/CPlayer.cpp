#include "CPlayer.h"

#include "CGrid.h"

CPlayer::CPlayer( const CSpriteAnimation& aSpriteAnimation, const SDL_Rect& aRect, const float& aVelocity ) :
	mSpriteAnimation( aSpriteAnimation ),
	mRect( aRect ),
	mPos( { float(aRect.x), float(aRect.y) } ),
	mVelocity( {aVelocity,aVelocity} )
{
}

CSpriteAnimation& CPlayer::SpriteAnimation()
{
	return mSpriteAnimation;
}

const SDL_Rect& CPlayer::GetRect() const
{
	return mRect;
}

const EDirection& CPlayer::GetDirection() const
{
	return mDirection;
}

const double& CPlayer::GetRotationAngle() const
{
	return mRotationAngle;
}

void CPlayer::SetVelocity( const pos& aVelocity )
{
	mVelocity = aVelocity;
}

void CPlayer::SetDirection( const EDirection& aDirection )
{
	mTempDirection = aDirection;
}

void CPlayer::UpdatePosition( const float& aDeltaTime, const CGrid& aGrid )
{
	if( CanChangeDirection( aGrid ) )
	{
		mDirection = mTempDirection;
		mTempDirection = NONE;
		SetVelocity();
	}

	const auto& map = aGrid.GetMap();
	auto xIncrement = mVelocity.x*aDeltaTime;
	auto yIncrement = mVelocity.y*aDeltaTime;
	const int currentXTile = mPos.x/TILE_SIZE;
	const int currentYTile = mPos.y/TILE_SIZE;

	if( mDirection == RIGHT )
	{
		if( mPos.x+xIncrement+PLAYER_SIZE > (currentXTile+1)*TILE_SIZE )
		{
			if( currentXTile+1 >= GRID_SIZE/TILE_SIZE )
				xIncrement = -mPos.x;
			else if( map[currentXTile+1][currentYTile] == 1 )
				xIncrement = (currentXTile+1)*TILE_SIZE-mPos.x-PLAYER_SIZE;
		}
	}
	else if( mDirection == LEFT )
	{
		if( mPos.x+xIncrement < currentXTile*TILE_SIZE )
		{
			if( currentXTile == 0 )
				xIncrement = -mPos.x + GRID_SIZE - TILE_SIZE;
			else if( map[currentXTile-1][currentYTile] == 1 )
				xIncrement = currentXTile*TILE_SIZE - mPos.x;
		}
	}
	else if( mDirection == UP )
	{
		if( mPos.y+yIncrement < currentYTile*TILE_SIZE && map[currentXTile][currentYTile-1] == 1 )
			yIncrement = currentYTile*TILE_SIZE - mPos.y;
	}
	else if( mDirection == DOWN )
	{
		if( mPos.y+yIncrement+PLAYER_SIZE > (currentYTile+1)*TILE_SIZE && map[currentXTile][currentYTile+1] == 1 )
			yIncrement = (currentYTile+1)*TILE_SIZE-mPos.y-PLAYER_SIZE;
	}

	mPos.x += xIncrement;
	mPos.y += yIncrement;
	mRect.x = mPos.x;
	mRect.y = mPos.y;
}

bool CPlayer::CanChangeDirection( const CGrid& aGrid )
{
	if( mTempDirection == NONE || mDirection == mTempDirection )
		return false;

	const auto& map = aGrid.GetMap();
	const int currentXTile = mPos.x/TILE_SIZE;
	const int currentYTile = mPos.y/TILE_SIZE;

	if( mTempDirection == RIGHT )
	{
		if( map[currentXTile+1][currentYTile] == 1 ||
				( (mPos.y+PLAYER_SIZE) > (currentYTile+1)*TILE_SIZE && map[currentXTile+1][currentYTile+1] == 1 ) )
			return false;
	}
	else if( mTempDirection == LEFT )
	{
		if( currentXTile != 0 && ( map[currentXTile-1][currentYTile] == 1 ||
				( (mPos.y+PLAYER_SIZE) > (currentYTile+1)*TILE_SIZE && map[currentXTile-1][currentYTile+1] == 1 ) ) )
			return false;
	}
	else if( mTempDirection == UP )
	{
		if( map[currentXTile][currentYTile-1] == 1 ||
				( mPos.x+PLAYER_SIZE > (currentXTile+1)*TILE_SIZE && map[currentXTile+1][currentYTile-1] == 1 ) )
			return false;
	}
	else if( mTempDirection == DOWN )
	{
		if( map[currentXTile][currentYTile+1] == 1 || map[currentXTile][currentYTile+1] == 4 ||
				( mPos.x+PLAYER_SIZE > (currentXTile+1)*TILE_SIZE && ( map[currentXTile+1][currentYTile+1] == 1 || map[currentXTile+1][currentYTile+1] == 4 ) ) )
			return false;
	}
	return true;
}

void CPlayer::SetVelocity()
{
	if( mDirection == LEFT )
	{
		mVelocity.x = -PLAYER_VELOCITY;
		mVelocity.y = 0.0f;
		mRotationAngle = 180;
	}
	else if( mDirection == RIGHT )
	{
		mVelocity.x = PLAYER_VELOCITY;
		mVelocity.y = 0.0f;
		mRotationAngle = 0;
	}
	else if( mDirection == UP )
	{
		mVelocity.y = -PLAYER_VELOCITY;
		mVelocity.x = 0.0f;
		mRotationAngle = 270;
	}
	else if( mDirection == DOWN )
	{
		mVelocity.y = PLAYER_VELOCITY;
		mVelocity.x = 0.0f;
		mRotationAngle = 90;
	}
}

void CPlayer::ResetToInitialPosition()
{
	mPos = {TILE_SIZE,TILE_SIZE};
	mRect.x = mPos.x;
	mRect.y = mPos.y;
	mVelocity = {0,0};
	mDirection = NONE;
	mTempDirection = NONE;
	mRotationAngle = 0.0f;
}
