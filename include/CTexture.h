#ifndef C_TEXTURE__H
#define C_TEXTURE__H

#include <string>

class SDL_Texture;
class SDL_Renderer;

class CTexture
{
public:
	CTexture( const std::string& aPath, SDL_Renderer* aRenderer );

	SDL_Texture* SDLTexture();

private:
	std::string mPath;
	SDL_Texture* mSDLTexture;
};

#endif // C_TEXTURE__H
