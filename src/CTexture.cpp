#include "CTexture.h"

#include "SDL_image.h"

SDL_Texture* CreateSDLTexture( const std::string& aPath, SDL_Renderer* aRenderer );

CTexture::CTexture( const std::string& aPath, SDL_Renderer* aRenderer ) :
	mPath( aPath ),
	mSDLTexture( CreateSDLTexture( aPath, aRenderer ) )
{
}

SDL_Texture* CTexture::SDLTexture()
{
	return mSDLTexture;
}

SDL_Texture* CreateSDLTexture( const std::string& aPath, SDL_Renderer* aRenderer )
{
	SDL_Surface* loadedSurface = IMG_Load( aPath.c_str() );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( aRenderer, loadedSurface );
	SDL_FreeSurface( loadedSurface );

	return texture;
}
