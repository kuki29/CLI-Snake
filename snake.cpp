#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <deque>
#include <ctime>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_Q 113

#define BUF_WIDTH 58
#define BUF_HEIGHT 27

#define BLANK_SYMBOL ' '
#define WALL_SYMBOL '#'
#define SNAKE_SYMBOL '@'
#define FOOD_SYMBOL 'O'

typedef std::pair<int, int> ii;
typedef std::deque<ii> deqii;
typedef unsigned int uint;

enum DIRECTIONS
{
	UP, DOWN, LEFT, RIGHT
};

void game_over(int score)
{
	system("CLS");
	printf("\n\n\n\n\n\t\t\t\tGAME OVER\n\t\t\tYOU SCORE IS %d\n", score);
	system("PAUSE");
	exit(EXIT_SUCCESS);
}

void snake_in_buffer(char *buffer, deqii& snake)
{
	for (auto& coord : snake)
	{
		buffer[coord.first + coord.second * BUF_WIDTH] = SNAKE_SYMBOL;
	}
}

void spawn_food(char *buffer)
{
	int x, y;

	do
	{
		x = rand() % BUF_WIDTH + 1;
		y = rand() % BUF_HEIGHT + 1;
	} while (buffer[x + y * BUF_WIDTH] != BLANK_SYMBOL);

	buffer[x + y * BUF_WIDTH] = FOOD_SYMBOL;
}

void move_snake(deqii& snake, char *buffer, const int& direction)
{
	ii vec;
	switch (direction)
	{
	case UP:
		vec = std::make_pair(0, -1);
		break;

	case DOWN:
		vec = std::make_pair(0, 1);
		break;

	case LEFT:
		vec = std::make_pair(-1, 0);
		break;

	case RIGHT:
		vec = std::make_pair(1, 0);
		break;
	}

	snake.push_front(std::make_pair((snake[0].first + vec.first),
		(snake[0].second + vec.second)));

	if (snake[0].first < 1 || snake[0].second < 1 || \
		snake[0].first > (BUF_WIDTH - 2) || snake[0].second > (BUF_HEIGHT - 2))
	{
		game_over(snake.size());
	}
	else if (snake.size() > 1)
	{
		for (auto coord = snake.begin() + 1; coord != snake.end() - 1; coord++)
		{
			if (snake[0] == *coord)
			{
				game_over(snake.size());
			}
		}
	}

	if (buffer[snake[0].first + snake[0].second * BUF_WIDTH] != FOOD_SYMBOL)
	{
		buffer[snake.back().first + snake.back().second * BUF_WIDTH] = BLANK_SYMBOL;
		snake.pop_back();
	}
	else
	{
		spawn_food(buffer);
	}
}

int main()
{
//--- Game buffer creation and initialization
	char *buffer = new char[BUF_WIDTH * BUF_HEIGHT + 1];

	if (!buffer)
	{
		printf("Failed to create game buffer!\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	memset(buffer, BLANK_SYMBOL, (BUF_WIDTH * BUF_HEIGHT));
	buffer[BUF_WIDTH + (BUF_HEIGHT - 1) * BUF_WIDTH] = '\0';

	for (int i = 0; i < BUF_WIDTH; i++)
	{
		buffer[i + 0 * BUF_WIDTH] = \
			buffer[i + (BUF_HEIGHT - 1) * BUF_WIDTH] = WALL_SYMBOL;
	}

	for (int i = 0; i < BUF_HEIGHT; i++)
	{
		buffer[0 + i * BUF_WIDTH] = \
			buffer[(BUF_WIDTH - 2) + i * BUF_WIDTH] = WALL_SYMBOL;
		buffer[(BUF_WIDTH - 1) + i * BUF_WIDTH] = '\n';
	}
//---

	int direction = RIGHT;
	deqii snake;
	snake.push_back(std::make_pair(((BUF_WIDTH / 2) + 4), ((BUF_HEIGHT / 2) - 1)));

	srand((uint) time(NULL));
	spawn_food(buffer);

	bool run = true;

//--- game loop
	while (run)
	{
		do
		{
			snake_in_buffer(buffer, snake);

			int delay = 30000000;
			while (delay--);

			system("CLS");
			printf("%s", buffer);

			move_snake(snake, buffer, direction);
		} while (!_kbhit());

		char c;

		switch ((c = _getch()))
		{
		case KEY_UP:
			if (direction != DOWN)
			{
				direction = UP;
			}
			break;

		case KEY_DOWN:
			if (direction != UP)
			{
				direction = DOWN;
			}
			break;

		case KEY_LEFT:
			if (direction != RIGHT)
			{
				direction = LEFT;
			}
			break;

		case KEY_RIGHT:
			if (direction != LEFT)
			{
				direction = RIGHT;
			}
			break;

		case KEY_Q:
			run = false;
			break;

		default:
			break;
		}
	}

	system("CLS");
	printf("Bye!\n");

	delete buffer;
	return 0;
}