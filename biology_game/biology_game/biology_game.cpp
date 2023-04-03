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

SDL_Window* window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "images/background.png");
SDL_Texture* bloodeCellTexture = IMG_LoadTexture(renderer, "images/bloodCell.png");
SDL_Texture* loadingTexture = IMG_LoadTexture(renderer, "images/loading.png");
SDL_Texture* playButtonSprite = IMG_LoadTexture(renderer, "images/PlayButton.png");
SDL_Texture* quitButtonSprite = IMG_LoadTexture(renderer, "images/QuitButton.png");
SDL_Texture* groundTextureSprite = IMG_LoadTexture(renderer, "images/ground.png");
SDL_Texture* playerTextureSprite = IMG_LoadTexture(renderer, "images/player.png");
SDL_Texture* laserTexture = IMG_LoadTexture(renderer, "images/laser.png");
SDL_Texture* heartTexture = IMG_LoadTexture(renderer, "images/heart.png");
SDL_Texture* attspeedTexture = IMG_LoadTexture(renderer, "images/upgrades/attspeed.png");
SDL_Texture* hpupTexture = IMG_LoadTexture(renderer, "images/upgrades/hpup.png");
SDL_Texture* levelupTexture = IMG_LoadTexture(renderer, "images/upgrades/levelup.png");
SDL_Texture* walkSpeedTexture = IMG_LoadTexture(renderer, "images/upgrades/walkSpeed.png");
SDL_Texture* damageUpTexture = IMG_LoadTexture(renderer, "images/upgrades/damageUp.png");
SDL_Texture* fireRateTexture = IMG_LoadTexture(renderer, "images/upgrades/firerate.png");
SDL_Texture* entitySpriteTexture = IMG_LoadTexture(renderer, "images/entitySprite.png");

SDL_Rect background = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

SDL_Rect bloodCellOrigin[40];
SDL_Rect bloodCellCurrent[40];

SDL_Rect playButtonOrigin = { SCREEN_WIDTH / 2 - 550 / 2, SCREEN_HEIGHT / 2, 550, 300 };
SDL_Rect playButtonCurrent = { 0, 0, 250, 300 };

SDL_Rect quitButtonOrigin = { SCREEN_WIDTH / 2 - 200 / 2, SCREEN_HEIGHT / 2 + 200, 200, 150 };
SDL_Rect quitButtonCurrent = { 0, 0, 125, 300 };

SDL_Rect playerRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,100,100 };

SDL_Rect levelUpRect = { 400, 400,SCREEN_WIDTH / 2, SCREEN_WIDTH / 4 };

SDL_Rect upgradeRect = { SCREEN_WIDTH / 2 - 50, 630,200,100 };

vector<SDL_Rect> floorRectOrigin;
vector<SDL_Rect> floorRectCurrent;
vector<SDL_Rect> lasers;
vector<SDL_Rect> heartsRect;
vector<SDL_Rect> entityRect;
vector<SDL_Rect> entityCurrentRect;
vector<int> laserDirection;
vector<int> entityGravity;
vector<bool> entityAlive;
vector<bool> entityDirection;

bool isPlaying = 1;
bool onMenuCheck = 1;
bool blackoutCheck = 0;
bool keyPressed = false;
bool onGround = 0;
bool gravityDebounce = 0;
bool levelDebounce = 0;
bool mapGenerateDebounce = 1;

int buttonNumber = 1;
int lavelUpTransparency = 0;
int invisRate = 0;
int loading = 255;
int playerX, playerY;
int speed = 5;
int laserSpeed = 20;
int gravity = 0;
int laserSize = 100;
int fireRate = 20;
int cooldown = 0;
int walkspeed = 10;
int jumpStrength = 24;
int hearts = 3;
int hurtCooldownCheck = 30;
int hurtCooldown = 0;
int kills = 0;
int killsPerLevelUp = 3;
int stats = 0;
int respawn[3] = { 0,0,0 };

SDL_Event event;

void laserAttacks()
{
	int j = 0;
	for (auto i = lasers.begin(); i != lasers.end(); i++, j++)
	{
		if (laserDirection[j] == 1)
		{
			lasers[j].x -= laserSpeed;
		}
		else if (laserDirection[j] == 2)
		{
			lasers[j].x += laserSpeed;
		}
	}
}

void fadeIn()
{
	SDL_SetTextureAlphaMod(loadingTexture, invisRate);
	SDL_RenderCopy(renderer, loadingTexture, 0, &background);
	if (invisRate <= 255)
		invisRate += 17;
	if (invisRate == 255)
		blackoutCheck = 0;
}
void fadeOut()
{
	SDL_SetTextureAlphaMod(loadingTexture, invisRate);
	SDL_RenderCopy(renderer, loadingTexture, 0, &background);
	if (invisRate > 0)
		invisRate -= 17;
}

void returnHighLightToNormal()
{
	playButtonCurrent.x = 0;
	quitButtonCurrent.x = 0;
}

void fixKeys()
{
	if (onMenuCheck == 1)
	{
		if (buttonNumber < 1)
			buttonNumber = 1;
		else if (buttonNumber > 2)
			buttonNumber = 2;
	}
}

void boubleMove()
{
	loop(0, 5)
	{
		bloodCellOrigin[i].x += 15;
		bloodCellCurrent[i].x -= 15;

		bloodCellCurrent[i].x += bloodCellsSize * 2;

		if (bloodCellOrigin[i].x > SCREEN_WIDTH)
		{
			bloodCellOrigin[i].x = -bloodCellsSize - (rand() % (bloodCellsSize * 2));
			bloodCellOrigin[i].y = (rand() % SCREEN_HEIGHT);
		}
		if (bloodCellCurrent[i].x > bloodCellsSize * 18)
		{
			bloodCellCurrent[i].x = 0;
		}

	}

	loop(5, 10)
	{

		bloodCellOrigin[i].x += 10;
		bloodCellCurrent[i].x -= 15;

		bloodCellCurrent[i].x += bloodCellsSize * 2;


		if (bloodCellOrigin[i].x > SCREEN_WIDTH)
		{
			bloodCellOrigin[i].x = -bloodCellsSize - (rand() % (bloodCellsSize * 2));
			bloodCellOrigin[i].y = (rand() % SCREEN_HEIGHT);
		}
		if (bloodCellCurrent[i].x > bloodCellsSize * 18)
		{
			bloodCellCurrent[i].x = 0;
		}

	}

	loop(10, 20)
	{

		bloodCellOrigin[i].x += 5;
		bloodCellCurrent[i].x -= 15;

		bloodCellCurrent[i].x += bloodCellsSize * 2;

		if (bloodCellOrigin[i].x > SCREEN_WIDTH)
		{
			bloodCellOrigin[i].x = -bloodCellsSize - (rand() % (bloodCellsSize * 2));
			bloodCellOrigin[i].y = (rand() % SCREEN_HEIGHT);
		}
		if (bloodCellCurrent[i].x > bloodCellsSize * 18)
		{
			bloodCellCurrent[i].x = 0;
		}

	}

	loop(20, 40)
	{

		bloodCellOrigin[i].x += 2;
		bloodCellCurrent[i].x -= 15;

		bloodCellCurrent[i].x += bloodCellsSize * 2;


		if (bloodCellOrigin[i].x > SCREEN_WIDTH)
		{
			bloodCellOrigin[i].x = -bloodCellsSize - (rand() % (bloodCellsSize * 2));
			bloodCellOrigin[i].y = (rand() % SCREEN_HEIGHT);
		}
		if (bloodCellCurrent[i].x > bloodCellsSize * 18)
		{
			bloodCellCurrent[i].x = 0;
		}

	}
}

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

void mapGenerating()
{
	entityRect.clear();
	entityDirection.clear();
	entityGravity.clear();
	entityCurrentRect.clear();
	loop(0, 3)
	{
		entityDirection.push_back(0);
		entityGravity.push_back(0);
		entityAlive.push_back(1);
		entityCurrentRect.push_back({ 0,0,100,130 });
	}

	entityRect.push_back({ 50,200,120, 120 });
	entityRect.push_back({ SCREEN_WIDTH - 100,200,120, 120 });
	entityRect.push_back({ SCREEN_WIDTH / 2,50,120, 120 });

	for (int j = 0; j < 10; j++)
	{
		floorRectOrigin.push_back({ (j - 1) * groundPiece,SCREEN_HEIGHT - groundPiece, groundPiece, groundPiece });
		if (j == 0)
			floorRectCurrent.push_back({ 0,0, groundPiece, groundPiece });
		else if (j == 10)
			floorRectCurrent.push_back({ groundPiece,0, groundPiece, groundPiece });
		else
			floorRectCurrent.push_back({ groundPiece / 2,0, groundPiece, groundPiece });
	}

	for (int j = 0; j < 4; j++)
	{
		floorRectOrigin.push_back({ (j + 1) * groundPiece,200, groundPiece, groundPiece });
		if (j == 0)
			floorRectCurrent.push_back({ 0,0, groundPiece, groundPiece });
		else if (j == 3)
			floorRectCurrent.push_back({ groundPiece,0, groundPiece, groundPiece });
		else
			floorRectCurrent.push_back({ groundPiece / 2,0, groundPiece, groundPiece });
	}

	floorRectOrigin.push_back({ 0,500, groundPiece, groundPiece });
	floorRectCurrent.push_back({ groundPiece,0, groundPiece, groundPiece });

	floorRectOrigin.push_back({ SCREEN_WIDTH - groundPiece,500, groundPiece, groundPiece });
	floorRectCurrent.push_back({ 0,0, groundPiece, groundPiece });
}
void gravitiyFunc()
{
	playerRect.y -= gravity;
	gravity -= 1;
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

void entetyRespawn()
{
	loop(0, 3)
	{
		if (respawn[i] == 3)
		{
			entityRect[i].y -= 500;
			entityAlive[i] = 1;
			respawn[i] = 0;
		}
	}

}

void levelUp()
{
	if (kills == killsPerLevelUp)
	{
		levelDebounce = 1;
		stats = rand() % 5;

		if (stats == 0)
		{
			laserSpeed += 5;
		}
		if (stats == 1)
		{
			laserSize += 50;
		}
		else if (stats == 2) {
			hearts++;
			playerRect.w += 10;
			playerRect.h += 10;
		}
		else if (stats == 3) {
			walkspeed += 5;
		}
		else if (stats == 4) {
			if (fireRate >= 0)
				fireRate -= 4;
			cooldown = fireRate;
		}
		kills = 0;
	}
	if (levelDebounce == 1)
	{
		if (lavelUpTransparency <= 255)
			lavelUpTransparency += 17;
		if (lavelUpTransparency == 255)
			levelDebounce = 0;
		SDL_SetTextureAlphaMod(levelupTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, levelupTexture, 0, &levelUpRect);

		upgradePrint();
	}
	else
	{
		if (lavelUpTransparency > 0)
			lavelUpTransparency -= 17;
		SDL_SetTextureAlphaMod(levelupTexture, lavelUpTransparency);
		SDL_RenderCopy(renderer, levelupTexture, 0, &levelUpRect);

		upgradePrint();
	}
}

void hitBox()
{
	gravityDebounce = 0;
	loop(0, floorRectOrigin.size())
	{
		if ((playerRect.y + playerRect.h > floorRectOrigin[i].y + groundPiece * 0.15 and playerRect.y < floorRectOrigin[i].y + floorRectOrigin[i].h - groundPiece * 0.15 and playerRect.x + playerRect.w > floorRectOrigin[i].x and playerRect.x < floorRectOrigin[i].x + floorRectOrigin[i].w))
		{
			gravityDebounce = 1;
			gravity = 0;
			onGround = 1;
			if ((playerRect.y + playerRect.h - 1 > floorRectOrigin[i].y + groundPiece * 0.15 and playerRect.y < floorRectOrigin[i].y + floorRectOrigin[i].h - groundPiece * 0.15 and playerRect.x + playerRect.w > floorRectOrigin[i].x and playerRect.x < floorRectOrigin[i].x + floorRectOrigin[i].w))
				gravity = 1;
		}
		else
		{
			if (gravityDebounce == 0)
				onGround = 0;
		}

		if ((playerRect.y + playerRect.h - 20 > floorRectOrigin[i].y + groundPiece * 0.15 and playerRect.y < floorRectOrigin[i].y + floorRectOrigin[i].h - groundPiece * 0.15 and playerRect.x + playerRect.w + walkspeed > floorRectOrigin[i].x and playerRect.x < floorRectOrigin[i].x + floorRectOrigin[i].w))
		{
			playerRect.x -= walkspeed;
			onGround = 1;
		}

		if ((playerRect.y + playerRect.h - 20 > floorRectOrigin[i].y + groundPiece * 0.15 and playerRect.y < floorRectOrigin[i].y + floorRectOrigin[i].h - groundPiece * 0.15 and playerRect.x + playerRect.w > floorRectOrigin[i].x and playerRect.x - walkspeed < floorRectOrigin[i].x + floorRectOrigin[i].w))
		{
			playerRect.x += walkspeed;
			onGround = 1;
		}
	}
}
void hurt()
{
	hearts--;
	gravity = 10;
}

void entityUpdate()
{
	loop(0, entityRect.size())
	{
		if (entityAlive[i] == 1)
		{
			if (entityRect[i].x < 0)
				entityDirection[i] = 1;
			if (entityRect[i].x + entityRect[i].w > SCREEN_WIDTH)
				entityDirection[i] = 0;
			if (entityDirection[i] == 1)
				entityRect[i].x += 5;
			else
				entityRect[i].x -= 5;

			entityRect[i].y -= entityGravity[i];
			entityGravity[i] -= 1;
			for (int j = 0; j < floorRectOrigin.size(); j++)
			{
				if ((entityRect[i].y + entityRect[i].h > floorRectOrigin[j].y + groundPiece * 0.15 and entityRect[i].y < floorRectOrigin[j].y + floorRectOrigin[j].h - groundPiece * 0.15 and entityRect[i].x + entityRect[i].w > floorRectOrigin[j].x and entityRect[i].x < floorRectOrigin[j].x + floorRectOrigin[j].w))
				{
					entityGravity[i] = 0;
					entityGravity[i] += 1;
					break;
				}
			}
			int l = 0;
			for (auto j = lasers.begin(); j != lasers.end(); j++, l++)
			{
				if (entityRect[i].x <= lasers[l].x + lasers[l].w and entityRect[i].x + entityRect[i].w >= lasers[l].x and lasers[l].y <= entityRect[i].y + entityRect[i].h and lasers[l].y + lasers[l].h >= entityRect[i].y)
				{
					entityAlive[i] = 0;
					kills++;
					respawn[i] = 30;
				}
			}
			if (entityRect[i].x <= playerRect.x + playerRect.w and entityRect[i].x + entityRect[i].w >= playerRect.x and playerRect.y <= entityRect[i].y + entityRect[i].h and playerRect.y + playerRect.h >= entityRect[i].y and hurtCooldown == 0)
			{
				hurtCooldown = hurtCooldownCheck;
				hurt();
			}

		}
		else if (respawn[i] >= 0)
			respawn[i] -= 1;
		entityCurrentRect[i].x += 128;
		if (entityCurrentRect[i].x >= 128 * 8)
			entityCurrentRect[i].x = 0;
	}
}

void buttonHighLight()
{
	returnHighLightToNormal();
	switch (buttonNumber) {
	case 1:
		playButtonCurrent.x = 250;
		break;
	case 2:
		quitButtonCurrent.x = 125;
		break;
	default:
		break;
	}
}

void enterEvent()
{
	blackoutCheck = 1;
	if (onMenuCheck == 1)
	{
		switch (buttonNumber) {
		case 1:
			onMenuCheck = 0;
			break;
		case 2:
			isPlaying = 0;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (buttonNumber) {
		case 1:
			onMenuCheck = 0;
			break;
		case 2:
			isPlaying = 0;
			break;
		default:
			break;
		}
	}
	buttonNumber = 1;
}

void cooldownCheck()
{
	if (cooldown < fireRate)
	{
		cooldown++;
	}
}

void keys()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (onMenuCheck == 1)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_UP)
					buttonNumber--;
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
					buttonNumber++;
				else if (event.key.keysym.sym == SDLK_RETURN)
					enterEvent();
			}
			else
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					onMenuCheck = 1;
					blackoutCheck = 1;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP and SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] and onGround == 1)
				{
					gravity += jumpStrength;
					playerRect.x -= walkspeed;

				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP and SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] and onGround == 1)
				{
					gravity += jumpStrength;
					playerRect.x += walkspeed;

				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_UP and onGround == 1)
					gravity += jumpStrength;
				else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
					playerRect.x -= walkspeed;
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
					playerRect.x += walkspeed;
				else if (event.key.keysym.scancode == SDL_SCANCODE_A and cooldown == fireRate)
				{
					cooldown = 0;
					lasers.push_back({ playerRect.x - playerRect.w / 2, playerRect.y,laserSize,laserSize });
					laserDirection.push_back(1);
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_D and cooldown == fireRate)
				{
					cooldown = 0;
					lasers.push_back({ playerRect.x + playerRect.w / 2, playerRect.y,laserSize,laserSize });
					laserDirection.push_back(2);
				}
		case SDL_KEYUP:
			keyPressed = false;
			break;
			}
		}


	}
	fixKeys();
}

void onMenu()
{
	keys();
	buttonHighLight();
	boubleMove();
	SDL_RenderCopy(renderer, backgroundTexture, 0, &background);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 15);
	loop(20, 40)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 25);
	loop(10, 20)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 90);
	loop(5, 10)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 150);
	loop(0, 5)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 255);
	SDL_RenderCopy(renderer, playButtonSprite, &playButtonCurrent, &playButtonOrigin);
	SDL_RenderCopy(renderer, quitButtonSprite, &quitButtonCurrent, &quitButtonOrigin);

}

void printHearts()
{
	heartsRect.clear();

	loop(0, hearts)
		heartsRect.push_back({ 50 * i, 0, 50,50 });
}

void onLevels()
{
	if (mapGenerateDebounce == 1)
	{
		mapGenerating();
		mapGenerateDebounce = 0;
	}


	keys();
	boubleMove();
	cooldownCheck();
	printHearts();

	SDL_RenderCopy(renderer, backgroundTexture, 0, &background);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 15);
	loop(20, 40)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 25);
	loop(10, 20)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 90);
	loop(5, 10)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 150);
	loop(0, 5)
		SDL_RenderCopy(renderer, bloodeCellTexture, &bloodCellCurrent[i], &bloodCellOrigin[i]);

	SDL_SetTextureAlphaMod(bloodeCellTexture, 255);

	gravitiyFunc();
	hitBox();

	laserAttacks();

	int j = 0;

	for (auto i = lasers.begin(); i != lasers.end(); i++, j++)
		SDL_RenderCopy(renderer, laserTexture, 0, &lasers[j]);

	SDL_RenderCopy(renderer, playerTextureSprite, 0, &playerRect);

	loop(0, hearts)
		SDL_RenderCopy(renderer, heartTexture, 0, &heartsRect[i]);

	entityUpdate();

	entetyRespawn();

	loop(0, floorRectOrigin.size())
		SDL_RenderCopy(renderer, groundTextureSprite, &floorRectCurrent[i], &floorRectOrigin[i]);

	loop(0, entityRect.size())
		if (entityAlive[i] == 1)
			SDL_RenderCopy(renderer, entitySpriteTexture, &entityCurrentRect[i], &entityRect[i]);
	levelUp();
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