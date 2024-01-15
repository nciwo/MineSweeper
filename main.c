#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

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
	/**** Inspect first index(rows) ****/
	if(!(strcmp(level.level, EASY.level)))
	{
		if((input[0] >= 'A') && (input[0] <= 'I')) ;
		else if((input[0] >= 'a') && (input[0] <= 'i')) ;
		else return false;
	}
	else
	{
		if((input[0] >= 'A') && (input[0] <= 'P')) ;
		else if((input[0] >= 'a') && (input[0] <= 'p')) ;
		else return false;
	}
	
	int index;
	int col_cnt, i, j;
	if(input[1] == ' ') index = 2;
	else index = 1;

	if((input[index] >= '1') && (input[index] <= '9')) col_cnt = input[index] - '0';
	else return false;

	if((input[index + 1] >= '0') && (input[index + 1] <= '9'))
	{
		col_cnt *= 10;
		col_cnt += input[index + 1] - '0';
	}
	printf("col_cnt = %d\n", col_cnt);

	if(!(strcmp(level.level, EASY.level)))
	{
		if((col_cnt >= 1) && (col_cnt <= 9)) return true;
		else return false;
	}
	else if(!(strcmp(level.level, INTERMEDIATE.level)))
	{
		if((col_cnt >= 1) && (col_cnt <= 16)) return true;
		else return false;
	}
	else if(!(strcmp(level.level, EXPERT.level)))
	{
		if((col_cnt >= 1) && (col_cnt <= 40)) return true;
		else return false;
	}

	printf("Error occured in comparing level.\n");
	exit(-1);
}

void getInput(Level level)
{
	char input[5];
	printf("\n>>> ");
	scanf("%s", input);
	printf("Input : %s\n", input);
	
	while(!(isValidInput(level, input)))
	{
		printf("Invalid input! Try again.\n>>> ");
		scanf("%s", input);
	}
	printf("Your Input is %s\n", input);
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
