#ifndef C_SPRITE_ANIMATION__H
#define C_SPRITE_ANIMATION__H

#include "CTexture.h"

#include <vector>

class CSpriteAnimation
{
public:
	CSpriteAnimation( const std::vector<CTexture>& aTextures, int aFramesPerSprite );

	CTexture& Texture();

	void CountFrame();

private:
	std::vector<CTexture> mTextures;
	int mFramesPerSprite;
	int mCurrentSprite = 0; 
	int mCurrentFrameCount = 0;
};

#endif
