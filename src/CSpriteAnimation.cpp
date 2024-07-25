#include "CSpriteAnimation.h"

CSpriteAnimation::CSpriteAnimation( const std::vector<CTexture>& aTextures, int aFramesPerSprite ) :
	mTextures( aTextures ),
	mFramesPerSprite( aFramesPerSprite )
{
}

CTexture& CSpriteAnimation::Texture()
{
	return mTextures[mCurrentSprite];
}

void CSpriteAnimation::CountFrame()
{
	mCurrentFrameCount++;
	if( mCurrentFrameCount > mFramesPerSprite )
	{
		mCurrentFrameCount = 0;
		mCurrentSprite = (mCurrentSprite+1)%mTextures.size();
	}
}
