#include <thread>
#include <unistd.h>
#include "simple_work_queue.hpp"

using namespace std;

struct {
    work_queue work_q;
} shared;

void do_work() {
    for (;;) {
        long w;
        if (shared.work_q.remove_job(&w)) {
            if (w < 0) break;

            printf("%% Thinking for a bit(%lds)\n", w); fflush(stdout);
            sleep(w);
            printf("%% Done thinking\n"); fflush(stdout);
        }
        else {
            // NO JOB CURRENTLY
            continue;
        }
    }
}

int main(int argc, char* argv[]) {
    // instead of pthread_create
    thread worker(do_work);

    for (;;) {
        long w;
        printf(">> "); fflush(stdout);
        scanf("%ld", &w);

        shared.work_q.add_job(w);
        if (w < 0) { printf("Exiting...\n"); return 0; }
    }
}
