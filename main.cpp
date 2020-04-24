#define OLC_PGE_APPLICATION
#include "Game.h"
#include "olcPixelGameEngine_2.0.h"

int main() {

	Game game;
	if (game.Construct(894,672,1,1))
		game.Start();

	system("pause");
	return 0;
}