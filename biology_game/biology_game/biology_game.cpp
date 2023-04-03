#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


#define loop(a, b) for (int i = a; i < b; i++)
#define invisRateCheck if (invisRate == 255)


using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int bloodCellsSize = 200;
const int levelSize = 1800;
const int groundPiece = 200;


void inputPositions()
{
	loop(0, 5)
	{
		bloodCellCurrent[i] = { bloodCellsSize * (rand() % 10), 0, bloodCellsSize * 2,bloodCellsSize * 2 };
		bloodCellOrigin[i] = { rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT ,bloodCellsSize, bloodCellsSize };
	}
	loop(5, 10)
	{
		bloodCellCurrent[i] = { bloodCellsSize * (rand() % 10), 0, bloodCellsSize * 2,bloodCellsSize * 2 };
		bloodCellOrigin[i] = { rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT ,bloodCellsSize - 75, bloodCellsSize - 75 };
	}
	loop(10, 20)
	{
		bloodCellCurrent[i] = { bloodCellsSize * (rand() % 10), 0, bloodCellsSize * 2,bloodCellsSize * 2 };
		bloodCellOrigin[i] = { rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT ,bloodCellsSize - 160, bloodCellsSize - 160 };
	}
	loop(20, 40)
	{
		bloodCellCurrent[i] = { bloodCellsSize * (rand() % 10), 0, bloodCellsSize * 2,bloodCellsSize * 2 };
		bloodCellOrigin[i] = { rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT ,bloodCellsSize - 180, bloodCellsSize - 180 };
	}
}

void upgradePrint()
{
	if (stats == 0)
	{
		SDL_SetTextureAlphaMod(attspeedTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, attspeedTexture, 0, &upgradeRect);
	}
	else if (stats == 1) {

		SDL_SetTextureAlphaMod(damageUpTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, damageUpTexture, 0, &upgradeRect);
	}
	else if (stats == 2) {
		SDL_SetTextureAlphaMod(hpupTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, hpupTexture, 0, &upgradeRect);
	}
	else if (stats == 3) {
		SDL_SetTextureAlphaMod(walkSpeedTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, walkSpeedTexture, 0, &upgradeRect);
	}
	else if (stats == 4) {
		SDL_SetTextureAlphaMod(fireRateTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, fireRateTexture, 0, &upgradeRect);
	}
}

void gravitiyFunc()
{
	playerRect.y -= gravity;
	gravity -= 1;
}


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

	SDL_SetTextureColorMod(entitySpriteTexture, 41, 0, 0);

	SDL_SetTextureColorMod(playerTextureSprite, 41, 0, 0);

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