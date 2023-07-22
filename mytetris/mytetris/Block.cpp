#include "Block.h"
#include<stdlib.h>
IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 33;
Block::Block()
{
	if (imgs[0] == NULL)
	{
		IMAGE imgTmp;
		loadimage(&imgTmp,"res/方块3.png");
		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++)
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i],i*33.4,1,size,size);
		}
		SetWorkingImage(); //恢复工作区
	}
	int blocks[7][4] = {
	1,3,5,7,// 1型
	2,4,5,7,// Z
	3,5,4,6,// Z'
	3,5,4,7,// T
	2,3,5,7,// L
	3,5,7,6,// J
	2,3,4,5,// 田
	};

	//随机生成一种方块
	blockType = 1+rand() % 7;
	//初始化 smallBlocks
	for (int i = 0; i<4; i++)
	{
		int value = blocks[blockType - 1][i];
		smallBlocks[i].row = value/2;
		smallBlocks[i].col = value%2;
	}
	img = imgs[blockType - 1];
}

void Block::drop()
{
	for (int i = 0; i < 4; i++)
	{
		smallBlocks[i].row++;
	}
}

void Block::moveLeftRight(int dx)
{
	for (int i = 0; i < 4; i++)
	{
		smallBlocks[i].col+=dx;
	}
}

void Block::revolve()
{
	Point p = smallBlocks[1];
	for (int i = 0; i < 4; i++)
	{
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}

void Block::draw(int leftMarign, int topMarign)
{
	for (int i = 0; i < 4; i++)
	{
		int x = leftMarign + smallBlocks[i].col*size;
		int y = topMarign+ smallBlocks[i].row * size;
		putimage(x, y, img);
	}
}

IMAGE** Block::getImage()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	// TODO: 在此处插入 return 语句
	if (this == &other)
	{
		return *this;
	}
	this->blockType = other.blockType;
	for (int i = 0; i < 4; i++)
	{
		this->smallBlocks[i] = other.smallBlocks[i];
	}
	return *this;
	
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols= map[0].size();
	for (int i = 0; i < 4; i++)
	{
		if (smallBlocks[i].col < 0 ||smallBlocks[i].col>=cols || 
			smallBlocks[i].row < 0 || smallBlocks[i].row >=rows||
			map[smallBlocks[i].row][smallBlocks[i].col]!=0)
		{
			return false;
		}
	}
	return true;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++)
	{
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType;
	}
}

int Block::getBlockType()
{
	return blockType;
}
