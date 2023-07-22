#pragma once
#include<graphics.h>
#include <vector>
using namespace std;
struct Point {
	int row;
	int col;
};
class Block
{
public:
	Block();
	void drop();
	void moveLeftRight(int dx);
	void revolve();
	void draw(int leftMarign,int topMarign);
	static IMAGE** getImage();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();
	/*Point* getSmallBlocks();*/
private:
	int blockType; //∑ΩøÈ¿‡–Õ
	Point smallBlocks[4];
	IMAGE *img;
	static IMAGE* imgs[7];
	static int size;
};

