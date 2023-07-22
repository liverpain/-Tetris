#include"Tetris.h"
#include <time.h>
#include<string.h>
int main(void)
{
	Tetris game(20, 10, 124, 123, 33);
	game.play();
	return 0;
}