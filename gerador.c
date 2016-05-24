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
#include <fcntl.h>
#include <semaphore.h>
#include "structs.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <pthread.h>

#define DEBUG           1
static unsigned t_geracao;
static int u_clock;
clock_t start;
unsigned convert_string_to_int (char * str);
void * t_vehicle(void * arg);
Vehicle * create_vehicle( int  id , Entry entry_point, unsigned time_park);

FullVehicle* create_full_vehicle(int id,  Entry e, unsigned park_time,clock_t start, clock_t start_gen ,pthread_mutex_t *mutex_fifo, pthread_mutex_t *mutex_log );
int main(int argc, char **argv){
	
	struct tms start_tms, now_tms;
	start=times(&start_tms);
	if(argc != 3)
	{
		fprintf(stderr, "Usage:./gerador <T_GERACAO> <U_RELOGIO>" );
		exit(1);	
	}
	if((t_geracao =convert_string_to_int(argv[1])) ==UINT_MAX){
		fprintf(stderr, "INVALID ENTRY FOR T_GERACAO \n" );
		exit(1);
	}
	if((u_clock =convert_string_to_int(argv[2])) ==UINT_MAX){
		fprintf(stderr, "INVALID ENTRY FOR U_CLOCK \n" );
		exit(1);	
	}
	long ticks_seg = sysconf(_SC_CLK_TCK);

	long t_geracao_ticks = t_geracao * ticks_seg;
	printf("%u\n",t_geracao );
	printf("%ld\n",ticks_seg );
	printf("%ld\n",t_geracao_ticks );
	srand((unsigned)time(NULL));
	unsigned id = 0, time_gap;
	pthread_mutex_t mutex_fifo = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;
	if(mkdir(FIFO_DIR,0777) == -1 && errno != EEXIST){
		printf(" error creating %s directory: %s \n",FIFO_DIR,strerror(errno) );
		exit(2);
	}
	//log TODO

	while((times(&now_tms)-start) < t_geracao_ticks){
		int nr = (rand ()%10);
		int a = rand () %123;
		printf("   %d ",nr );
		int time_gap = 0;
		if( nr < 5){
			time_gap= 0;
		}
		else if (nr <8){
			time_gap = u_clock;
		}
		else time_gap =2*u_clock;
		mysleep(time_gap);
		//create vehicle
		Entry e = rand () % 4;
		unsigned tempo = (rand()% 10 + 1) * u_clock;
		FullVehicle *f = create_full_vehicle(id,e,tempo,times(&now_tms),start ,&mutex_fifo,&mutex_log);
		//printf("viatura: id %u , t %u ,a %u\n",f->id,f->time_park,f->entry_point);
		//thread TODO
		pthread_t tid;
		if(pthread_create(&tid,NULL,t_vehicle,f) !=0){
			printf("error creating vehicle thread");
			exit(4);
		}

		//id increment for private fifo
		id++;
	}
	//unlink(LOCK_FILE);
	rmdir(FIFO_DIR);
	pthread_exit(NULL); 
}


void * t_vehicle(void * arg){
 if(pthread_detach(pthread_self()) != 0) {
        perror("error detaching vehicle thread\n");
        exit(5);
    }
    FullVehicle * f = (FullVehicle *) arg;
    
    Vehicle * v = create_vehicle(f->id,f->time_park,f->entry_point); 
 	//printf("viatura: id %u , t %u ,a %u\n",v->id,v->time_park,v->entry_point);
	if(mkfifo(v->fifo,S_IRWXU) !=0){
		perror();
	}
	pthread_mutex_lock(f->mutex_fifo);
	//write to controoler
	pthread_mutex_unlock(f->mutex_fifo);

	pthread_mutex_lock(f->mutex_log);
	//write to log dfile
	pthread_mutex_unlock(f->mutex_log);
 return NULL;   
}

FullVehicle* create_full_vehicle(int id, Entry e, unsigned tempo,clock_t start, clock_t start_gen ,pthread_mutex_t *mutex_fifo, pthread_mutex_t *mutex_log ){
	
	//srand((unsigned) time(NULL));
	//	printf("%d\n",tempo );
	FullVehicle *  v= malloc(sizeof(FullVehicle));
	v->id=id;
	v->entry_point=e;
	v->start=start;
	v->start_gen=start_gen;
	v->mutex_fifo = mutex_fifo;
	v->mutex_log = mutex_log;
	v->time_park = tempo;
	return v ;
}
Vehicle * create_vehicle( int  id , Entry entry_point, unsigned time_park){
	Vehicle * v = malloc(sizeof(Vehicle));
	v->id=id;
	v->entry_point = entry_point;
	v->time_park=time_park;
	snprintf(v->fifo,19,"%s%u",FIFO,id);
	return v;

}
unsigned convert_string_to_int (char * str){
    errno = 0;
    char *endptr;
    unsigned val;
    val = (unsigned) strtoul(str, &endptr, 10);
    if ((errno == ERANGE && val == UINT_MAX) || (errno != 0 && val == 0)) {
        perror("strtoul");
        return UINT_MAX;
    }
    if (endptr == str) {
        printf("parse_uint: no digits were found in %s \n", str);
        return UINT_MAX;
    }
    return val;
}

