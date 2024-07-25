#ifndef C_GHOST__H
#define C_GHOST__H

#include "CTexture.h"
#include "Globals.h"

#include "SDL.h"

#include <map>

class CGhost
{
public:
	using direction_textures = std::map<EDirection,CTexture>;

	CGhost( const direction_textures& aDirectionTextures, const SDL_Rect& aRect );

	CTexture& Texture();
	const SDL_Rect& GetRect() const;
	const pos& GetPos() const;
	void SetPos( const pos& aPos );
	const EDirection& GetDirection() const;
	void SetDirection( const EDirection& aDirection );

	bool IsEatable() const;
	void SetEatableTime( const float& aEatableTime );
	void DecreaseEatableTime( const float& aDecreaseEatableTime );
	bool MoveToTile( const float& aDeltaTime, const tile& aTargetTile );

private:
	EDirection mDirection = LEFT;
	direction_textures mDirectionTextures;
	SDL_Rect mRect;
	bool mIsEatable = false;
	float mEatableTime = 0.0f;
	pos mPos;
};

#endif // C_GHOST__H
