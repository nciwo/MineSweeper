#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


typedef enum _Board
{
	WALL = -2,
	MINE,
	AIR
} Board;

Board **ppBoard;
bool **ppClicked;

/**** Level Structure ****/
typedef struct _Level
{
	const int width;
	const int height;
	const int mines;
	char* const level;
} Level;

typedef struct _COORD
{
	int x;
	int y;
} COORD;

/**** Global Level Variables ****/
const Level EASY = {9, 9, 10, "EASY"};
const Level INTERMEDIATE = {16, 16, 40, "INTERMEDIATE"};
const Level EXPERT = {30, 16, 99, "EXPERT"};

void boardReveal(Level level, COORD pos);



void plantMine(Level level)
{
	srand(time(NULL));
	int i, j;
	const int size = level.width * level.height;
	Board *pRandArr = (Board*)malloc(sizeof(Board) * size);
	
	for(i = 0; i < size; ++i) pRandArr[i] = AIR;
	for(i = 0; i < level.mines; ++i)
	{
		unsigned int index = rand() % size;
		if(pRandArr[index] == MINE)
		{
			--i;
			continue;
		}
		else pRandArr[index] = MINE;
	}
	for(i = 0; i < level.height; ++i)
		for(j = 0; j < level.width; ++j)
			ppBoard[i][j] = pRandArr[(i * level.width) + j];
}

void initBoard(Level level)
{
	int i, j;

	ppBoard = (Board**)malloc(sizeof(Board*) * level.height);
	ppClicked = (bool**)malloc(sizeof(bool*) * level.height);
	for(i = 0; i < level.height; ++i)
	{
		ppBoard[i] = (Board*)malloc(sizeof(Board) * level.width);
		ppClicked[i] = (bool*)malloc(sizeof(bool) * level.width);
		for(j = 0; j < level.width; ++j) ppClicked[i][j] = false;
	}
	plantMine(level);
}

/**** This function prints all blocks at once. ****/
void __SECRET__printBoard(Level level)
{
	int i, j;
	for(i = 0; i < level.height; ++i)
	{
		for(j = 0; j < level.width; ++j)
		{
			switch(ppBoard[i][j])
			{
			case AIR:
				printf(" ");
				break;
			case MINE:
				printf("X");
				break;
			case WALL:
				break;
			default:
				printf("%d", ppBoard[i][j]);
				break;
			}
		}
		printf("\n");
	}
}

void writeNumber(Level level)
{
	int i, j;
	for(i = 0; i < level.height; ++i)
	{
		for(j = 0; j < level.width; ++j)
		{
			if(ppBoard[i][j] == AIR)
			{
				int count = 0;
				if((i != 0) && (j != 0) && (ppBoard[i - 1][j - 1] == MINE)) ++count;
				if((i != 0) && (ppBoard[i - 1][j] == MINE)) ++count;
				if((i != 0) && (j != level.width - 1) && (ppBoard[i - 1][j + 1] == MINE)) ++count;
				
				if((j != 0) && (ppBoard[i][j - 1] == MINE)) ++count;
				if((j != level.width - 1) && (ppBoard[i][j + 1] == MINE)) ++count;

				if((i != level.height - 1) && (j != 0) && (ppBoard[i + 1][j - 1] == MINE)) ++count;
				if((i != level.height - 1) && (ppBoard[i + 1][j] == MINE)) ++count;
				if((i != level.height - 1) && (j != level.width - 1) && (ppBoard[i + 1][j + 1] == MINE)) ++count;
				if(count != 0) ppBoard[i][j] = count;
			}
		}
	}
}

void freeBoard(Level level)
{
	int i;
	for(i = 0; i < level.height; ++i)
	{
		free(ppBoard[i]);
		free(ppClicked[i]);
	}
	free(ppBoard);
	free(ppClicked);
}


void printBoard(Level level)
{
	int i, j;
	for(i = 0; i < level.height; ++i)
	{
		for(j = 0; j < level.width; ++j)
		{
			printf("[");
			if(ppClicked[i][j] == true)
			{
				switch(ppBoard[i][j])
				{
				case AIR:
					printf(" ");
					break;
				case MINE:
					printf("X");
					break;
				case WALL:
					break;
				default:
					printf("%d", ppBoard[i][j]);
					break;
				}	
			}
			else printf("*");
			printf("]");
		}
		printf("\n");
	}
}

bool isValidInput(Level level, const char *input, COORD *c)
{
	char key[4];
	int i;
	if(input[1] == ' ')
	{
		key[0] = input[0];
		key[1] = input[2];
		key[2] = input[3];
		key[3] = input[4];
	}
	else for(i = 0; i < 4; ++i) key[i] = input[i];

	if(key[0] < 'A' && key[0] < 'a') return false;
	if(((key[1] >= '1') && (key[1] <= '9')) && ((key[2] >= '0') && (key[2] <= '9')))
	{
		c->x = (key[1] - '0') * 10 + (key[2] - '0');
		if(key[3] != '\0') return false;
	}
	else c->x = -1;

	if(level.level == EASY.level)
	{
		if((key[0] >= 'a') && (key[0] <= 'i')) c->y = key[0] - 'a' + 1;
		else if((key[0] >= 'A') && (key[0] <= 'I')) c->y = key[0] - 'A' + 1;
		else return false;

		if(key[2] != '\0') return false;

		c->x = key[1] - '0';
		if((c->x >= 1) && (c->x <= 9)) return true;
		else return false;
	}
	else if(level.level == INTERMEDIATE.level)
	{
		if((key[0] >= 'a') && (key[0] <= 'p')) c->y = key[0] - 'a' + 1;
		else if((key[0] >= 'A') && (key[0] <= 'P')) c->y = key[0] - 'A' + 1;
		else return false;

		if(c->x == -1)
		{
			if(key[2] != '\0') return false;
			if((key[1] >= '1') && (key[1] <= '9')) { c->x = key[1] - '0'; return true; }
			else return false;
		}
		else
		{
			if((c->x >= 1) && (c->x <= 16)) return true;
			else return false;
		}

	}
	else if(level.level == EXPERT.level)
	{
		if((key[0] >= 'a') && (key[0] <= 'p')) c->y = key[0] - 'a' + 1;
		else if((key[0] >= 'A') && (key[0] <= 'P')) c->y = key[0] - 'A' + 1;
		else return false;

		if(c->x == -1)
		{
			if(key[2] != '\0') return false;
			if((key[1] >= '1') && (key[1] <= '9')) { c->x = key[1] - '0'; return true; }
		}
		else
		{
			if((c->x >= 1) && (c->x <= 40)) return true;
			else return false;
		}
	}
}

/**** In gcc, there's no scanf_s(). So it's required. ****/
void scanf_s(char *input, int size)
{
	if(size <= 0) return;
	int i;
	for(i = 0; i < size; ++i)
	{
		input[i] = getchar();
		if(input[i] == '\n')
		{
			input[i] = '\0';
			break;
		}
	}
	if(input[size - 1] != '\0') while(getchar() != '\n');
}

void getInput(Level level)
{
	COORD pos;
	COORD *pPos = &pos;
	char input[5];
	printf("\n>>> ");
	scanf_s(input, 5);
	
	// printf("input : %s\n", input);
	while(!(isValidInput(level, input, pPos)))
	{
		printf("Invalid input! Try again.\n>>> ");
		scanf_s(input, 5);
		printf("input : %s\n", input);
	}
	// printf("Your Input is %s\n", input);
	// printf("x : %d, y : %d\n", pPos->x, pPos->y);
	boardReveal(level, pos);
}

void GameOver()
{
	printf("You clicked mine!\n");
	exit(0);
}

void boardReveal(Level level, COORD pos)
{
	COORD temp;
	int x = pos.x - 1, y = pos.y - 1;
	
	ppClicked[y][x] = true;

	if(ppBoard[y][x] == MINE) GameOver();
	else if(ppBoard[y][x] == AIR)
	{
		if(x > 0)
		{
			temp.x = x - 1;
			temp.y = y;
			boardReveal(level, temp);
			
			if(y > 0)
			{
				temp.y = y - 1;
				boardReveal(level, temp);
			}
			if(y < (level.height - 1))
			{
				temp.y = y + 1;
				boardReveal(level, temp);
			}
		}
		if(x < (level.width - 1))
		{
			temp.x = x + 1;
			temp.y = y;
			boardReveal(level, temp);
			
			if(y > 0)
			{
				temp.y = y - 1;
				boardReveal(level, temp);
			}
			if(y < (level.height - 1))
			{
				temp.y = y + 1;
				boardReveal(level, temp);
			}
		}	
		if(y > 0)
		{
			temp.x = x;
			temp.y = y - 1;
			boardReveal(level, temp);
		}
		if(y < (level.height - 1))
		{
			temp.x = x;
			temp.y = y + 1;
			boardReveal(level, temp);
		}
	}
}

int main()
{
	Level level = EASY;

	initBoard(level);
	writeNumber(level);

	printBoard(level);
	getInput(level);

	printBoard(level);

	printf("\n=================================\n");
	__SECRET__printBoard(level);

	freeBoard(level);
	return 0;
}
