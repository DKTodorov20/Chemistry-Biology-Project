#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


using namespace std;
int main(int argc, char* args[]) 
{
	srand(time(0));

	inputPositions();


	const int FPS = 60;
	float frameTime = 0;
	int prevTime = 0;
	int currentTime = 0;
	float deltaTime = 0;
	float moveSpeed = 200.0f;
	const Uint8* keyState;

	int b[3] = { 0,0,0 };

	SDL_SetTextureColorMod(entitySpriteTexture, 41, 0, 0);

	while (isPlaying)
	{

		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 1000.0f;

		SDL_SetTextureColorMod(entitySpriteTexture, 125, 0, 0);
		SDL_SetTextureColorMod(laserTexture, 125, 0, 0);

		if (onMenuCheck == 1)
			onMenu();
		else
			onLevels();
		if (loading > 0)
		{
			SDL_SetTextureAlphaMod(loadingTexture, loading);
			SDL_RenderCopy(renderer, loadingTexture, 0, &background);
			if (loading > 0)
				loading -= 17;
		}

		if (blackoutCheck == 1)
			fadeIn();
		else
			fadeOut();

		if (hurtCooldown <= hurtCooldownCheck and hurtCooldown != 0)
			hurtCooldown--;

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		SDL_Delay(50);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}