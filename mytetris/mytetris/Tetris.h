#pragma once
#include<vector>
#include<iostream>
#include<graphics.h>
#include <time.h>
#include<string.h>
#include"Block.h"
using namespace std;
class Tetris
{
public:
	Tetris(int rows,int cols,int left,int top,int blocksize );
	void init(); 
	void play();
private:
	void keyEvent();
	void updateWindow();//更新画面
	int getdelay(); //距离上一次间隔多少时间
	void drop();
	void clearLine();
	void moveLeftRight(int dx);
	void revolve();
	void drawScore();
	void drawTimeCnt();
	void drawLevel();
	void drawLines();
	void drawHighestScore();
	void checkOver();
	void saveScore();
	void gameOverWindow();
	bool isIn(ExMessage* msg, int x, int y, int w, int h);
private:
	int delay;
	//是否更新
	bool update; 
	//0代表没有方块
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMarign;
	int topMarign;
	int blockSize;
	IMAGE imgBg;
	IMAGE imgGameOver;
	IMAGE imgGameStart;
	Block* curBlock;
	Block* nextBlock; //预告方块
	Block bakBlock;   //备用方块
	int score; //分数
	int highestScore;
	int level; //级别
	int linecnt; //总共消行数
	double  timecnt; //所用时间
	clock_t starttime;//最初时间
	bool gameOver;
};

