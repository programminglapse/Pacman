#include "CGhost.h"

CGhost::CGhost( const direction_textures& aDirectionTextures, const SDL_Rect& aRect ) :
	mDirectionTextures( aDirectionTextures ),
	mRect( aRect ),
	mPos( { float(aRect.x), float(aRect.y) } )
{
}

CTexture& CGhost::Texture()
{
	return mDirectionTextures.at(mDirection);
}

const SDL_Rect& CGhost::GetRect() const
{
	return mRect;
}

const pos& CGhost::GetPos() const
{
	return mPos;
}

void CGhost::SetPos( const pos& aPos )
{
	mPos = aPos;
	mRect.x = mPos.x;
	mRect.y = mPos.y;
}

bool CGhost::IsEatable() const
{
	return mIsEatable;
}

void CGhost::SetEatableTime( const float& aEatableTime )
{
	mEatableTime = aEatableTime;
	mIsEatable = true;
}

const EDirection& CGhost::GetDirection() const
{
	return mDirection;
}

void CGhost::SetDirection( const EDirection& aDirection )
{
	mDirection = aDirection;
}

void CGhost::DecreaseEatableTime( const float& aDecreaseEatableTime )
{
	if( mEatableTime - aDecreaseEatableTime < 0.0f )
	{
		mEatableTime = 0.0f;
		mIsEatable = false;
	}
	else
		mEatableTime -= aDecreaseEatableTime;
}

bool CGhost::MoveToTile( const float& aDeltaTime, const tile& aTargetTile )
{
	const auto velocity = IsEatable() ? GHOST_EATABLE_VELOCITY : GHOST_VELOCITY;
	if( mDirection == LEFT )
		mPos.x = std::max<float>( mPos.x-velocity*aDeltaTime, aTargetTile.first*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2 );
	else if( mDirection == RIGHT )
		mPos.x = std::min<float>( mPos.x+velocity*aDeltaTime, aTargetTile.first*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2 );
	else if( mDirection == DOWN )
		mPos.y = std::min<float>( mPos.y+velocity*aDeltaTime, aTargetTile.second*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2 );
	else if( mDirection == UP )
		mPos.y = std::max<float>( mPos.y-velocity*aDeltaTime, aTargetTile.second*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2 );

	mRect.x = mPos.x;
	mRect.y = mPos.y;

	if( mDirection == LEFT || mDirection == RIGHT )
		return mPos.x == aTargetTile.first*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2;
	else
		return mPos.y == aTargetTile.second*TILE_SIZE+TILE_SIZE/2-GHOST_SIZE/2;
}
