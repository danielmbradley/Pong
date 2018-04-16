//*****************************************************************
// PICDEM HPC Explorer 18 LCD function
// Filename : messages.h
//
// By Simon Cotton
// 
// 2011-11-25
//*****************************************************************

#ifndef MESSAGES_H
#define	MESSAGES_H

	// Note the '\n' character here is used to indicate end of the line

	char PONG[] = {'P', 'O', 'N', 'G', '!','\n '};
	char Ready[] = {'R', 'E', 'A', 'D', 'Y', '\n '};
	char PlayerOne[] 	= {'R', 'E', 'A', 'D', 'Y', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', 'O', 'N', 'E', '\n'};
    char PlayerTwo[] 	= {'R', 'E', 'A', 'D', 'Y', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', ' ', 'T', 'W', 'O', '\n'};
    char lineClear[]	= {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    char playerTitle[] = {'P', 'L', 'A', 'Y', 'E', 'R', '2', ' ', ' ', 'P', 'L', 'A', 'Y', 'E', 'R', '1', '\n'};
    char scores[17 ]	= {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    char winnerTwo[]	= {'W', 'I', 'N', 'N', 'E', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    char winnerOne[]	= {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W', 'I', 'N', 'N', 'E', 'R', '\n'};

