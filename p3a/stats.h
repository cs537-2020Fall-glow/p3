
typedef struct {
    int pid; // 4
    char birth[25]; // 25
    char clientString[10]; // 10
    int elapsed_sec; // 4
    double elapsed_msec; // 8
    clock_t start_time; // 8
    struct timespec start_elapsed_time; // https://en.cppreference.com/w/c/chrono/timespec 16
} stats_t;

union seg_t {
    stats_t stats;
    char padding[64];
};