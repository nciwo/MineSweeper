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

/**** Global Level Variables ****/
const Level EASY = {9, 9, 10, "EASY"};
const Level INTERMEDIATE = {16, 16, 40, "INTERMEDIATE"};
const Level EXPERT = {30, 16, 99, "EXPERT"};

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

bool isValidInput(Level level, const char *input)
{
	char key[4];
	int i, checker;
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
		checker = (key[1] - '0') * 10 + (key[2] - '0');
		if(key[3] != '\0') return false;
	}
	else checker = -1;

	if(level.level == EASY.level)
	{
		if(key[0] > 'I' && key[0] > 'i') return false;
		if(key[2] != '\0') return false;
		if((key[1] >= '1') && (key[1] <= '9')) return true;
		else return false;
	}
	else if(level.level == INTERMEDIATE.level)
	{
		if(key[0] > 'P' && key[0] > 'p') return false;
		if(checker == -1)
		{
			if(key[2] != '\0') return false;
			if((key[1] >= '1') && (key[1] <= '9')) return false;
		}
		else
		{
			if((checker >= 1) && (checker <= 16)) return true;
			else return false;
		}

	}
	else if(level.level == EXPERT.level)
	{
		if(key[0] > 'P' && key[0] > 'p') return false;
		if(checker == -1)
		{
			if(key[2] != '\0') return false;
			if((key[1] >= '1') && (key[1] <= '9')) return false;
		}
		else
		{
			if((checker >= 1) && (checker <= 40)) return true;
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
	char input[5];
	printf("\n>>> ");
	scanf_s(input, 5);
	
	// printf("input : %s\n", input);
	while(!(isValidInput(level, input)))
	{
		printf("Invalid input! Try again.\n>>> ");
		scanf_s(input, 5);
		printf("input : %s\n", input);
	}
	// printf("Your Input is %s\n", input);
}

int main()
{
	Level level = EASY;

	initBoard(level);
	writeNumber(level);

	printBoard(level);
	getInput(level);


	freeBoard(level);
	return 0;
}
