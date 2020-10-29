
typedef struct {
    int pid; // 4
    char birth[25]; // 25
    char clientString[10]; // 10
    int elapsed_sec; // 4
    double elapsed_msec; // 8
    long birth_seconds; // 8
} stats_t;

union seg_t {
    stats_t stats;
    char padding[64];
};