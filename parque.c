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
int convert_string_to_int(char * str, int * str); //safely converts a string to int 

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
int convert_string_to_int (char * str, int * num ){
	int i  = 0 ;
	while(str[i]!='\0'){
		if(str[i] >= '1' && str[i]<= '9') //making sure the arg passed 
			break;						  //doesnt start with 0
		i++;
	}
	errno = 0;
	*num = strtol(str,NULL,10);
	if(errno != 0)
		return-1;
	return 0 ;

}