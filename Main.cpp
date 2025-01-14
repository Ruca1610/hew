#include "Main.h"
#include "DirectX.h"
#include "WinUtil.h"
#include "DirectXTex/Texture.h"
#include "Input.h"
#include "Game3D.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Sound.h"

//--- 定数定義
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//-- グローバル変数
Game3D* g_pGame;

unsigned int GetAppWidth()
{
	return SCREEN_WIDTH;
}
unsigned int GetAppHeight()
{
	return SCREEN_HEIGHT;
}

void Init()
{
	if (FAILED(InitDX(GetHWND(), SCREEN_WIDTH, SCREEN_HEIGHT, false)))
	{
		Error("directx initialize failed.");
	}
	if (FAILED(InitTexture(GetDevice())))
	{
		Error("texture initialize failed.");
	}
	if (FAILED(InitInput()))
	{
		Error("input initialize failed.");
	}
	if (FAILED(InitGeometory()))
	{
		Error("geometory initialize failed.");
	}
	if (FAILED(InitSound()))
	{
		Error("sound initialize failed.");
	}

	Sprite::Init();
	CGauge::Init();

	g_pGame = new Game3D();
}
void Uninit()
{
	delete g_pGame;
	CGauge::Uninit();
	Sprite::Uninit();
	UninitSound();
	UninitGeometory();
	UninitInput();
	UninitTexture();
	UninitDX();
}
void Update(float deltaTime)
{
	UpdateInput();
	g_pGame->Update();
}
void Draw()
{
	BeginDrawDX();
	g_pGame->Draw();
	EndDrawDX();
}
