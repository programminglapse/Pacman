#ifndef C_MODULE_RENDERER__H
#define C_MODULE_RENDERER__H

#include "AModule.h"

class SDL_Texture;
class SDL_Renderer;
class SDL_Color;
class SDL_Rect;
class CGrid;

class CModuleRenderer : public AModule
{
public:
	CModuleRenderer();

	bool Init() override;
	bool PreUpdate() override;
	bool Update() override;
	bool Clear() override;

	SDL_Renderer* SDLRenderer();
	void RenderTexture( SDL_Texture* aTexture, const SDL_Rect& aRect, int aRotationAngle = 0 );

private:
	SDL_Texture* CreateMapTexture( const CGrid& aGrid );
	void RenderSpecialTiles();
	void RenderEmptyTile( int aX, int aY );
	void RenderWallTile( int aX, int aY );
	void RenderCoinTile( int aX, int aY );
	void RenderTile( int aX, int aY, int aWidth, int aHeight, const SDL_Color& aColor );
	void RenderGhostWallTile( int aX, int aY );

	SDL_Texture* mMap = nullptr;
	SDL_Renderer* mSDLRenderer;
};

#endif // C_MODULE_RENDERER__H
