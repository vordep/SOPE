#ifndef AUXSTRUCT_H
#define AUXSTRUCT_H

#define TPS sysconf(_SC_CLK_TCK)	//ticks per second

#define NUM_CONTROLLERS 	4

#define SEC2NANO        1000000000
#define FIFO_DIR        "tmp/"
#define FIFO_N          "tmp/fifoN"
#define FIFO_E          "tmp/fifoE"
#define FIFO_S          "tmp/fifoS"
#define FIFO_O          "tmp/fifoO"
#define FIFO            "tmp/fifo"
#define FIFO_NAME_SIZE  19

typedef enum {
    N, E, S, O
} Entry;


typedef struct {
    int id;                                                                                                  // unique identifier for vehicle
    unsigned time_park;                                                                                                     // parking time for vehicle
    Entry entry_point;                                                                                                      // park access for vehicle
    clock_t start;                                                                                                      // start time of vehicle
    clock_t start_gen;                                                                                                  // start time of generator
    pthread_mutex_t *mutex_fifo;                                                                                        // generator mutex for fifo
    pthread_mutex_t *mutex_log;                                                                                         // generator mutex for log
} FullVehicle;

typedef struct {
    int id;                                                                                                  // unique identifier for vehicle
    unsigned time_park;                                                                                                     // parking time for vehicle
	Entry entry_point;                                                                                                      // park access for vehicle
    char fifo[FIFO_NAME_SIZE];                                                                                          // vehicle fifo
} Vehicle;

void mysleep(unsigned ticks) {                                                                        // sleep for desired number of clock ticks, with number of ticks per second given
    double seconds = (double)ticks / TPS;
    struct timespec *req;
    req = malloc(sizeof(struct timespec));
    req->tv_sec = (time_t) (seconds);
    req->tv_nsec = (long) (seconds * SEC2NANO - req->tv_sec * SEC2NANO);

    printf("sleep: %u ticks, %d sec, %ld nsec\n", ticks, (int)req->tv_sec, req->tv_nsec);

    nanosleep(req, NULL);
    free(req);
}/*
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
}*/
#endif