#include"Header.h"

int main() {
	MazeCreate* maze = new MazeCreate;
	RandState* rand_state = new RandState;

	if (maze->ConditionCheck(width, height) == false) {
		delete(maze);
		delete(rand_state);
		return -1;
	}

	// 移動量を記憶するための vector
	std::vector<int> move_x;
	std::vector<int> move_y;

	int _maze[height][width] = {};

	//外壁、壁伸ばし開始点を設定
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
				maze->Create(&_maze[y][x],WALL);
			}
			else if (x % 2 == 0 && y % 2 == 0) {
				maze->Create(&_maze[y][x], SEED);
				maze->PlusSeedCount();
			}
		}
	}

	maze->MazeCout(_maze);

	do{
		maze->GetRandSeed(_maze,rand_state);
		maze->Create(&_maze[rand_state->rand_y][rand_state->rand_x], WALL);
		maze->ExtendWall(rand_state->rand_x, rand_state->rand_y, _maze, move_x, move_y);
	} while (maze->GetSeedCount() != 0);

	maze->MazeCout(_maze);

	delete(maze);
	delete(rand_state);

	return 0;
}

int MazeCreate::GetSeedCount() {
	return m_seed_count;
}

int MazeCreate::GetCreateCount() {
	return m_create_count;
}

/**
* @brief 種の位置をランダムに選び出す関数
* @brief 種の数が減るほど選び出すための時間が増大する欠点あり
*/
void MazeCreate::GetRandSeed(int _maze[height][width],RandState *rand_state) {
	do {
		std::srand(time(NULL));
		rand_state->rand_y = rand() % (height - 3) + 1;
		rand_state->rand_x = rand() % (width - 3) + 1;
		if (rand_state->rand_y % 2 != 0)
			rand_state->rand_y++;
		if (rand_state->rand_x % 2 != 0)
			rand_state->rand_x++;
	} while (_maze[rand_state->rand_y][rand_state->rand_x] != SEED);
}

void MazeCreate::PlusSeedCount() {
	m_seed_count++;
}

void MazeCreate::MinusSeedCount() {
	if (GetSeedCount() > 0)
		m_seed_count--;
}

void MazeCreate::MinusCreateCount() {
	if (GetCreateCount() > 0)
		m_create_count--;
}

/**
* @brief 配列の中身を変更する
* @brief もし、変更前が SEED であれば SeedCount を減らす
* @param (maze_wall) 中身を何に変更したいかを指定する
*/
 void MazeCreate::Create(int *_maze,MazeWall maze_wall){
	switch (maze_wall)
	{
	case NO:
		*_maze = MazeWall::NO;
		break;
	case WALL:
		if (*_maze == SEED)
			MinusSeedCount();
		*_maze = MazeWall::WALL;
		break;
	case ME_WALL:
		if (*_maze == SEED)
			MinusSeedCount();
		*_maze = MazeWall::ME_WALL;
		break;
	case SEED:
		*_maze = MazeWall::SEED;
		break;
	default:
		break;
	}
}

 /**
 * @brief 種から壁を伸ばす関数
 * @brief 壁に接続するまで再起し続ける
 * @brief 四方が自身が生成途中の壁の場合 vector を用いて壁を伸ばせる座標まで戻る
 */
int MazeCreate::ExtendWall(int x, int y, int _maze[height][width],std::vector<int>& move_x, std::vector<int>& move_y)
{
	//移動可能な方向を記憶するための vector
	std::vector<int>direction;

	if(MeWallCheck(x, y, _maze, direction)==true){

		if (WallCheck(x, y, _maze) == true) {

			int rand_d;

			std::srand(time(NULL));
			rand_d = rand() % direction.size();

			switch (direction[rand_d])
			{
			case UP:
				Create(&_maze[y - 1][x], ME_WALL);
				if (_maze[y - 2][x] == WALL) {
					break;
				}
				Create(&_maze[y - 2][x], ME_WALL);
				y -= 2;
				move_x.push_back(0);
				move_y.push_back(-2);
				ExtendWall(x, y, _maze, move_x, move_y);
				break;
			case RIGHT:
				Create(&_maze[y][x + 1], ME_WALL);
				if (_maze[y][x + 2] == WALL) {
					break;
				}
				Create(&_maze[y][x + 2], ME_WALL);
				x += 2;
				move_x.push_back(2);
				move_y.push_back(0);
				ExtendWall(x, y, _maze, move_x, move_y);
				break;
			case LEFT:
				Create(&_maze[y][x - 1], ME_WALL);
				if (_maze[y][x - 2] == WALL) {
					break;
				}
				Create(&_maze[y][x - 2], ME_WALL);
				x -= 2;
				move_x.push_back(-2);
				move_y.push_back(0);
				ExtendWall(x, y, _maze, move_x, move_y);
				break;
			case DOWN:
				Create(&_maze[y + 1][x], ME_WALL);
				if (_maze[y + 2][x] == WALL) {
					break;
				}
				Create(&_maze[y + 2][x], ME_WALL);
				y += 2;
				move_x.push_back(0);
				move_y.push_back(2);
				ExtendWall(x, y, _maze, move_x, move_y);
				break;
			default:
				break;
			}
		}
		
	}
	else {
		//ここで座標を戻す
		x -= move_x.back();
		y -= move_y.back();
		move_x.pop_back();
		move_y.pop_back();
		ExtendWall(x, y, _maze, move_x, move_y);
	}
	
	// 現在生成中の壁を壁に変える
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (_maze[i][j] == ME_WALL) {
				Create(&_maze[i][j], WALL);
			}
		}

	}

	//m_create_direction = NO_D;
		
	MazeCout(_maze);

	return 0;
}

/**
* @brief 画面を更新してから、迷路をコンソール画面に出力する関数
*/
void MazeCreate::MazeCout(int _maze[height][width]) {

	system("cls");

	for (int y = 0; y < height; y++) {
		std::cout << std::endl;
		for (int x = 0; x < width; x++) {
			switch (_maze[y][x])
			{
			case MazeWall::NO:
				std::cout << "　";
				break;
			case MazeWall::WALL:
				std::cout << "■";
				break;
			case MazeWall::ME_WALL:
				std::cout << "■";
				break;
			default:
				std::cout << "　";
				break;
			}
		}
	}
}

/**
* @brief 迷路生成の条件を満たしているか確認する関数
* @brief width、height が偶数であれば1足す
*/
bool MazeCreate::ConditionCheck(int width, int height) {
	if (width < 5 || height < 5) {
		return false;
	}
	if (width % 2 == 0)width++;
	if (height % 2 == 0)height++;

	return true;
}

/**
* @brief ニマス先が現在伸ばしている壁でなかった場合 vector にその方向を入れる関数
* @return vector の中身が空であれば false を返す
*/
bool MazeCreate::MeWallCheck(int x, int y, int _maze[height][width],std::vector<int>& direction) {
	if (_maze[y - 2][x] != ME_WALL) {
		direction.push_back(UP);
	}
	if (_maze[y][x + 2] != ME_WALL) {
		direction.push_back(RIGHT);
	}
	if (_maze[y + 2][x] != ME_WALL) {
		direction.push_back(DOWN);
	}
	if (_maze[y][x - 2] != ME_WALL) {
		direction.push_back(LEFT);
	}

	if (direction.empty() == false) {
		return true;
	}
	return false;
}

/**
* @brief 一マス先をみて現在伸ばしている壁の先頭が既存の壁と接続しているかを確認する関数
* @return 一つでも接続していれば false を返す
*/
bool MazeCreate::WallCheck(int x, int y, int _maze[height][width]) {
	if (_maze[y - 1][x] == WALL) {
		return false;
	}
	else if (_maze[y][x + 1] == WALL) {
		return false;
	}
	else if (_maze[y + 1][x] == WALL) {
		return false;
	}
	else if (_maze[y][x - 1] == WALL) {
		return false;
	}
	else {
		return true;
	}
}