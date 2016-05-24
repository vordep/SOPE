#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <error.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "structs.h"
/**
DEFINES AND ASSETS
**/
#define NUM_CONTROLLERS         4
#define LOGGER_NAME             "parque.log"


clock_t start;
/**
GLOBAL VARS
**/
static int num_lugares;
static int t_Abertura;
static int num_lugares_ocupados = 0;
static FILE * fd_log;



/**
FUNCTIONS DECLARATIONS
**/
//controlador 
//arrumador

int main(int argc, char **argv){

	if(argc != 3)
	{
		fprintf(stderr, "Usage:./parque <NUM LUGARES> <T_ABERTURA>" );
		exit(1);	
	}
	if(convert_string_to_int(argv[1],&num_lugares) !=0)
	{
		fprintf(stderr, "INVALID ENTRY FOR NUM_LUGARES \n" );
		exit(1);
	}

	if(convert_string_to_int(argv[2],&t_Abertura) !=0)
	{
		fprintf(stderr, "INVALID ENTRY FOR TEMPO_ABERTURA \n" );
		exit(1);
	}
	/*
	if () //open logger 
	{
			
	}
	*/
	/*
	
	BEGGINING OF THE APPLICATION
	*/


}
