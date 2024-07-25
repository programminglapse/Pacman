#include "CModuleTimer.h"

#include "SDL.h"

bool CModuleTimer::Update()
{
	const auto currentTime = SDL_GetTicks();

	if( mLastTime == 0 )
	{
		mLastTime = currentTime;
		return true;
	}

	mDeltaTime = std::max( 1.0f/60.0f, (float)((currentTime-mLastTime)/1000.0f) );
	mLastTime = currentTime;

	return true;
}

const float& CModuleTimer::GetDeltaTime() const
{
	return mDeltaTime;
}
