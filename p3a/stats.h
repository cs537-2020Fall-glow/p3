
typedef struct {
    int pid;
    char birth[25];
    char clientString[10];
    int elapsed_sec;
    double elapsed_msec;
    clock_t start_time;
} stats_t;

union seg_t {
    stats_t stats;
    char padding[64];
};