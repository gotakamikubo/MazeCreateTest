#ifndef HEADER_H_
#define HEADER_H_

#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<Windows.h>

enum MazeWall {
	NO,			//! 何もない（通路）
	WALL,		//! 壁
	ME_WALL,	//! 現在生成中の壁
	SEED,		//! 壁を伸ばすための種
};

enum Direction {
	 UP,
	 RIGHT,
	 DOWN,
	 LEFT,
};

const int width = 15;
const int height = 9;

typedef struct {
	int rand_x = 0;
	int rand_y = 0;
}RandState;

class MazeCreate {
private:
	int m_seed_count = 0;
	int m_create_count = 0;
public:
	int GetSeedCount();
	int GetCreateCount();
	void MinusCreateCount();
	void GetRandSeed(int _maze[height][width],RandState *rand_state);
	void PlusSeedCount();
	void MinusSeedCount();
	void Create(int *_maze,MazeWall maze_wall);
	void MazeCout(int _maze[height][width]);
	int ExtendWall(int x, int y, int _maze[height][width],std::vector<int>&move_x, std::vector<int>& move_y);
	bool ConditionCheck(int width, int height);
	bool MeWallCheck(int x,int y,int _maze[height][width],std::vector<int>&direction);
	bool WallCheck(int x, int y, int _maze[height][width]);
};

#endif // !HEADER_H_
