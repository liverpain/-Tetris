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
	void updateWindow();//���»���
	int getdelay(); //������һ�μ������ʱ��
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
	//�Ƿ����
	bool update; 
	//0����û�з���
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
	Block* nextBlock; //Ԥ�淽��
	Block bakBlock;   //���÷���
	int score; //����
	int highestScore;
	int level; //����
	int linecnt; //�ܹ�������
	double  timecnt; //����ʱ��
	clock_t starttime;//���ʱ��
	bool gameOver;
};

