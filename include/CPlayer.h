#ifndef C_PLAYER__H
#define C_PLAYER__H

#include "CSpriteAnimation.h"
#include "Globals.h"

#include "SDL.h"

class CGrid;

class CPlayer
{
public:
	CPlayer( const CSpriteAnimation& aSpriteAnimation, const SDL_Rect& aRect, const float& aVelocity );

	CSpriteAnimation& SpriteAnimation();
	const SDL_Rect& GetRect() const;
	const EDirection& GetDirection() const;
	const double& GetRotationAngle() const;

	void SetVelocity( const pos& aVelocity );
	void SetDirection( const EDirection& aDirection );

	void UpdatePosition( const float& aDeltaTime, const CGrid& aGrid );
	void ResetToInitialPosition();

private:
	bool CanChangeDirection( const CGrid& aGrid );
	void SetVelocity();

	CSpriteAnimation mSpriteAnimation;
	SDL_Rect mRect;
	pos mPos;
	pos mVelocity;
	EDirection mDirection = NONE;
	EDirection mTempDirection = NONE;
	double mRotationAngle = 0.0f;
};

#endif // C_PLAYER__H
