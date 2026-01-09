#include <stdio.h>
#include <time.h>

#include "PLANES.h"
#include "PLANES_ERROR.h"
#include "SETUP.h"

static inline long get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int main(void) {
    struct planes_t *planes;

    printf("=== Planes Update Benchmark ===\n");
    printf("Allocating %zu planes...\n", N_PLANES);

    if (instantiate_planes(N_PLANES, &planes) != SUCCESS) {
        printf("Could not instantiate\n");
        planes_error();
        return -1;
    }

    printf("✓ Allocation done\n\n");

    // Initialize some planes as used
    for (size_t i = 0; i < N_PLANES; i++) {
        planes->distance[i] = 1000;
        planes->speed[i] = 500 + (i % 100);
        planes->used[i] = (i % 2 == 0);  // 50% used
    }

    const int delta_t = 1;
    const int iterations = 1000;

    printf("Running %d iterations of update on %zu planes\n", iterations, N_PLANES);
    printf("50%% of planes are active (used = true)\n\n");

    // Test 1: With multiplication by used
    printf("Test 1: Multiplication approach\n");
    printf("  distance[i] -= speed[i] * delta_t * used[i]\n");

    long update_start = get_time_ns();
    for (int iter = 0; iter < iterations; iter++) {
        for (size_t i = 0; i < N_PLANES; i++) {
            planes->distance[i] -= planes->speed[i] * delta_t * planes->used[i];
        }
    }
    long update_end = get_time_ns();
    double update_time = (update_end - update_start) / 1e9;

    double total_ops = (double)N_PLANES * iterations;
    printf("  Time: %.4f seconds\n", update_time);
    printf("  Updates/sec: %.2f M updates/sec\n", total_ops / update_time / 1e6);
    printf("  Throughput: %.2f B ops/sec\n\n", total_ops / update_time / 1e9);

    // Reset distances
    for (size_t i = 0; i < N_PLANES; i++) {
        planes->distance[i] = 1000;
    }

    // Test 2: With bitwise AND
    printf("Test 2: Bitwise AND approach\n");
    printf("  distance[i] -= speed[i] * delta_t & -(int)used[i]\n");

    long bitwise_start = get_time_ns();
    for (int iter = 0; iter < iterations; iter++) {
        for (size_t i = 0; i < N_PLANES; i++) {
            planes->distance[i] -= (planes->speed[i] * delta_t) & -(int)planes->used[i];
        }
    }
    long bitwise_end = get_time_ns();
    double bitwise_time = (bitwise_end - bitwise_start) / 1e9;

    printf("  Time: %.4f seconds\n", bitwise_time);
    printf("  Updates/sec: %.2f M updates/sec\n", total_ops / bitwise_time / 1e6);
    printf("  Throughput: %.2f B ops/sec\n\n", total_ops / bitwise_time / 1e9);

    // Comparison
    double speedup = update_time / bitwise_time;
    printf("=== Comparison ===\n");
    printf("Speedup (MUL vs AND): %.2f%%\n", (speedup - 1.0) * 100);
    printf("Time difference: %.4f seconds\n\n", update_time - bitwise_time);

    // Verify some data
    printf("=== Verification ===\n");
    printf("Sample distance[0]: %d\n", planes->distance[0]);
    printf("Sample distance[1]: %d\n", planes->distance[1]);
    printf("Sample used[0]: %s\n", planes->used[0] ? "true" : "false");
    printf("Sample used[1]: %s\n", planes->used[1] ? "true" : "false");

    destroy_planes(&planes);
    printf("\n✓ Cleanup done\n");
    return 0;
}