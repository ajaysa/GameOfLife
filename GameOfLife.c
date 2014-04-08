#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int i, j;
	char **board, **board_copy;
	char body = '*';
	char ch;
	int MaxX, MaxY;

	initscr();			/* Start curses mode 		*/
	curs_set(0);
	raw();				/* Line buffering disabled	*/
	noecho();

	keypad(stdscr, TRUE);

	getmaxyx(stdscr, MaxX, MaxY);	

	// board defines the screen pixels
	board = (char **)malloc(sizeof(char*)*MaxX);
	for(i=0; i<MaxX; i++)
		board[i] = (char *)malloc(sizeof(char)*MaxY);

	// randomly initialize the board
	srand ( time(NULL) );
	for(i=0; i<MaxX; i++)
	{
		for(j=0; j<MaxY; j++)
		{
			if ( rand()%13 == 0 )
				board[i][j] = '1';
			else
				board[i][j] = '0';
		}
	}
/*
	board[MaxX/2][MaxY/2] = '1';
	board[MaxX/2+1][MaxY/2] = '1';
	board[MaxX/2][MaxY/2+1] = '1';
	board[MaxX/2-1][MaxY/2] = '1';
	board[MaxX/2][MaxY/2-1] = '1';
	board[MaxX/2+1][MaxY/2+1] = '1';
	board[MaxX/2-1][MaxY/2+1] = '1';
	board[MaxX/2-1][MaxY/2-1] = '1';
	board[MaxX/2+1][MaxY/2-1] = '1';
*/
	// board_copy represents a copy of the board
	board_copy = (char**)malloc(sizeof(char *)*MaxX);
	for(i=0; i<MaxX; i++)
		board_copy[i] = (char *)malloc(sizeof(char)*MaxY);

	// clear board_copy	
	for(i=0; i<MaxX; i++)
	{
		for(j=0; j<MaxY; j++)
			board_copy[i][j] = '0';
	}

	for(i=0; i<MaxX; i++)
	{
		for(j=0; j<MaxY; j++)
		{
			if ( board[i][j] == '1' )
	    			mvprintw(i, j, "%c",body);
		}
	}

	int FLAG;
	while(1)
	{
		FLAG = 0;

		refresh();
		fflush(stdout);
		// sleep for 15 sec
		usleep(150000);

		int count_live;

		for(i=0; i<MaxX; i++)
		{
			for(j=0; j<MaxY; j++)
			{
				count_live=0;

				// Check the rules
				if ( i-1 >= 0 && board[i-1][j] == '1' )
					count_live++;
				if ( i+1 < MaxX && board[i+1][j] == '1' )
					count_live++;
				if ( j-1 >= 0 && board[i][j-1] == '1' )
					count_live++;
				if ( j+1 < MaxY && board[i][j+1] == '1' )
					count_live++;
				if ( (i+1 < MaxX && j+1 < MaxY) && board[i+1][j+1] == '1' )
					count_live++;
				if ( (i-1 >= 0 && j+1 < MaxY ) && board[i-1][j+1] == '1' )
					count_live++;
				if ( (i+1 < MaxX && j-1 >= 0) && board[i+1][j-1] == '1' )
					count_live++;
				if ( (i-1 >= 0 && j-1 >= 0) && board[i-1][j-1] == '1' )
					count_live++;

				if ( board[i][j] == '1' )
				{
					if ( count_live == 2 || count_live == 3 )
						board_copy[i][j] = '1';
					if ( count_live > 3 || count_live < 2 )
					{
						board_copy[i][j] = '0';
						FLAG = 1;
					}
				}
				else
				{
					if ( count_live == 3 )
					{
						board_copy[i][j] = '1';
						FLAG = 1;
					}
				}
			}
		}

		// reconfigure the board
		for(i=0; i<MaxX; i++)
		{
			for(j=0; j<MaxY; j++)
			{
				board[i][j] = board_copy[i][j];
				if ( board[i][j] == '1' )
					mvprintw(i, j, "%c",body);
			}
		}

		// clear the board_copy
		for(i=0; i<MaxX; i++)
		{
			for(j=0; j<MaxY; j++)
				board_copy[i][j] = '0';
		}

		if ( FLAG == 0 )
			break;
	}

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	mvprintw(MaxX/2, MaxY/2-30, "LIFE ENDED, TOO SHORT!! ENJOY YOUR LIFE AS LONG AS YOU'RE ALIVE");
	while(1)
	{
		ch = wgetch(stdscr);
		if ( ch == 'q' || ch == 'Q' )
			break;
	}

	refresh();
	endwin();	/* End curses mode */

	return 0;
}
