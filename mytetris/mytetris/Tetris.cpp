#include "Tetris.h"
#include <stdlib.h>
#include<iostream>
#include <time.h>
#include<string.h>
#include<fstream>
#include<conio.h>
#include"Block.h"
#include<mmsystem.h>
#include<graphics.h>
#define MAX_LEVEL 5
#define RECORDER_FILE "recoeder.txt"
const int SPEED_NORMAL[5] = { 500,400,300,150,80 };
const int SPEED_QUICK = 30;//30毫秒
Tetris::Tetris(int rows, int cols, int left, int top, int blocksize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMarign = left;
	topMarign = top;
	this->blockSize = blocksize;
	for (int i = 0; i < rows; i++)
	{
		vector<int> mapRow;
		for (int j = 0; j < cols; j++)
		{
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	delay = SPEED_NORMAL[0];
	//配置随机种子
	srand(time(NULL));
	//创建游戏窗口
	initgraph(696, 927);
	//加载图片
	loadimage(&imgBg,"res/bg2.png");
	loadimage(&imgGameOver, "res/gameOver.png");
	loadimage(&imgGameStart, "res/gameStart.png");
	putimage(0, 0, &imgGameStart);
	//初始化游戏区中数据
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;
		}
	}
	score = 0;
	linecnt = 0;
	level = 1;
	timecnt = 0;
	starttime = clock();

	//初始化最高分
	ifstream file(RECORDER_FILE);
	if (!file.is_open())
	{
		highestScore = 0;
	}
	else
	{
		file >> highestScore;
	}
	file.close();

	gameOver = false;
}

void Tetris::play()
{
	init();
	ExMessage msg;
	while (1)
	{
		if (peekmessage(&msg,EM_MOUSE)&&isIn(&msg,248,388,212,90)&&msg.message==WM_LBUTTONDOWN)
		{
			break;
		}
	}
	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;
	int timer = 0;
	while (1)
	{
		keyEvent();
		timer += getdelay();
		if(timer> delay)
		{
			timer = 0;
			drop();
			update = true;
		}
		if (update)
		{
			update = false;
			//更新游戏画面
			updateWindow();
			//更新游戏数据
			clearLine();
			if (gameOver)
			{
				//保存分数
				saveScore();
				//游戏结束界面
				gameOverWindow();
				system("pause");
				//重新开始游戏
				init(); 
			}
		}
		
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool revolveFlag=false;
	int dx=0;
	if (_kbhit())
	{
		ch=_getch();
		if (ch == 224)
		{
			ch = _getch();
			switch (ch)
			{
			case 72:
				revolveFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}
	if (dx != 0)
	{
		//左右移动
		moveLeftRight(dx);
		update = true;
		dx = 0;
	}
	if (revolveFlag)
	{
		revolve();
		update = true;
		revolveFlag = false;
	}
}

void Tetris::updateWindow()
{
	putimage(0,0,&imgBg);
	IMAGE** imgs = Block::getImage();
	BeginBatchDraw();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0)continue;
			int x = j * blockSize + leftMarign;
			int y= i * blockSize + topMarign;
			putimage(x, y,imgs[map[i][j]-1]);
		}
	}
	curBlock->draw(leftMarign, topMarign);
	nextBlock->draw(leftMarign + 33 * 11, topMarign);
	drawScore();
	drawTimeCnt();
	drawLevel();
	drawLines();
	drawHighestScore();
	EndBatchDraw();
	////测试
	//Block block;
	//block.draw(124,123);
}

int Tetris::getdelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0)
	{
		lastTime = currentTime;
		return 0;
	}
	else
	{
		unsigned long long ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}

}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();
	if (!curBlock->blockInMap(map))
	{
		//把方块固定不动
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
		delay = SPEED_NORMAL[level-1];
		//检查游戏是否已经结束
		checkOver();
	}

}

void Tetris::clearLine()
{
	int lines = 0;
	//不消行的行数
	int k = rows - 1; 
	int cnt;
	for (int i = k; i >= 0; i--)
	{
		//检查第i行是否满行
		cnt = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				cnt++;
			}
			map[k][j] = map[i][j];
		}
		if ( cnt < cols)
		{//不是满行
			k--;
		}
		else
		{
			//是满行
			lines++;
		}
	}
	if (lines > 0)
	{   //计算得分
		int addScore[4] = { 100,300,600,1000 };
		score += addScore[lines - 1];
		linecnt += lines;
		update = true;
	}
}

void Tetris::moveLeftRight(int dx)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(dx);
	if (!curBlock->blockInMap(map))
	{
		*curBlock = bakBlock;
	}
}

void Tetris::revolve()
{
	if (curBlock->getBlockType() == 7)
	{
		return;
	}
	bakBlock = *curBlock;
	curBlock->revolve();
	if (!curBlock->blockInMap(map))
	{
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	LOGFONT f;
	gettextstyle(&f);
	//字体颜色
	setcolor(RGB(0,0,0));
	//字体背景透明
	setbkmode(TRANSPARENT);
	f.lfWeight = 15;
	f.lfHeight = 30;
	//字体抗锯齿
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy_s(f.lfFaceName,sizeof(f.lfFaceName),_T("Microsoft YaHei"));
	settextstyle(&f);
	outtextxy(470,572, scoreText);
	f.lfWeight = 30;
	f.lfHeight = 60;
	settextstyle(&f);
	outtextxy(470,492, "分数");
	outtextxy(20, 492, "时间");
	outtextxy(20, 350, "级别");
	outtextxy(470, 350, "行数");
	outtextxy(180, 790, "历史最高分");
}

void Tetris::drawTimeCnt()
{
	clock_t EndTime = clock();
	timecnt = (double)(EndTime- starttime) / CLOCKS_PER_SEC;
	char timecntText[32];
	sprintf_s(timecntText, sizeof(timecntText), "%.1lf", timecnt);
	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 15;
	f.lfHeight = 30;
	settextstyle(&f);
	outtextxy(20, 572, timecntText);
}

void Tetris::drawLevel()
{
	level = (int)(timecnt / 60)+1;
	char levelText[32];
	sprintf_s(levelText, sizeof(levelText), "%d", level);
	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 30;
	f.lfHeight = 60;
	settextstyle(&f);
	outtextxy(40, 425, levelText);
}

void Tetris::drawLines()
{
	char lineText[32];
	sprintf_s(lineText, sizeof(lineText), "%d", linecnt);
	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 30;
	f.lfHeight = 60;
	settextstyle(&f);
	outtextxy(490, 425, lineText);
}

void Tetris::drawHighestScore()
{
	char highestscoreText[32];
	sprintf_s(highestscoreText, sizeof(highestscoreText), "%d", highestScore);
	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 30;
	f.lfHeight = 60;
	settextstyle(&f);
	outtextxy(180, 860, highestscoreText);
}

void Tetris::checkOver()
{
	gameOver =(curBlock->blockInMap(map) == false);
}

void Tetris::saveScore()
{
	if (score > highestScore)
	{
		highestScore = score;
		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::gameOverWindow()
{
	putimage(0, 0, &imgGameOver,SRCAND);
}

bool Tetris::isIn(ExMessage* msg, int x, int y, int w, int h)
{
	if (msg->x > x && msg->x<x + w && msg->y>y && msg->y < y + h)
	{
		return true;
	}
	return false;
}
