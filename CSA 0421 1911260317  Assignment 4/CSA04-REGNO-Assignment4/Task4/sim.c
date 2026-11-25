/* Disk-scheduling simulator: FCFS SSTF SCAN C-SCAN LOOK C-LOOK.
   Service time = 0.1 ms per cylinder + 4 ms rotation/transfer (ASSUMED HDD model). */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXCYL 199
enum {FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK, NALG};
static const char *nm[] = {"FCFS","SSTF","SCAN","C-SCAN","LOOK","C-LOOK"};
/* arr[] ascending (ms). Fills wait[]; returns total head movement in cylinders. */
static double run(int alg, int n, const double *arr, const int *cyl, int head, double *wait)
{
    int *pend = malloc(n * sizeof(int)); int np = 0, nxt = 0, served = 0, dir = -1;
    double t = 0, move = 0;
    if (alg == CSCAN || alg == CLOOK) dir = 1;
    while (served < n) {
        int pick = -1;
        for (;;) {
            if (np == 0 && nxt < n && arr[nxt] > t) t = arr[nxt];
            while (nxt < n && arr[nxt] <= t) pend[np++] = nxt++;
            if (np == 0) continue;
            int bi = -1;
            for (int k = 0; k < np; k++) {
                int r = pend[k], c = cyl[r];
                if (alg == FCFS) { if (bi < 0 || r < pend[bi]) bi = k; }
                else if (alg == SSTF) { if (bi < 0 || abs(c-head) < abs(cyl[pend[bi]]-head) ||
                         (abs(c-head)==abs(cyl[pend[bi]]-head) && r < pend[bi])) bi = k; }
                else if (dir > 0) { if (c >= head && (bi < 0 || c < cyl[pend[bi]] || (c==cyl[pend[bi]] && r<pend[bi]))) bi = k; }
                else { if (c <= head && (bi < 0 || c > cyl[pend[bi]] || (c==cyl[pend[bi]] && r<pend[bi]))) bi = k; }
            }
            if (bi >= 0) { pick = bi; break; }
            /* nothing in the current direction: reposition */
            int d;
            if (alg == SCAN)       { int e = dir > 0 ? MAXCYL : 0; d = abs(e-head); head = e; dir = -dir; }
            else if (alg == LOOK)  { dir = -dir; d = 0; }
            else if (alg == CSCAN) { d = (MAXCYL-head) + MAXCYL; head = 0; }
            else { int lo = MAXCYL+1; for (int k=0;k<np;k++) if (cyl[pend[k]]<lo) lo = cyl[pend[k]]; d = abs(head-lo); head = lo; }
            move += d; t += 0.1*d;
        }
        int r = pend[pick]; pend[pick] = pend[--np];
        /* keep FCFS order irrelevant: FCFS picks min index every time */
        int seek = abs(cyl[r]-head);
        wait[r] = t - arr[r];
        move += seek; t += 0.1*seek + 4.0; head = cyl[r]; served++;
    }
    free(pend); return move;
}
static double expo(double mean){ return -mean*log(1.0 - (rand()+0.5)/((double)RAND_MAX+1.0)); }
int main(void)
{
    const int n = 10000; const char *lname[] = {"light","medium","heavy"}; double mean[] = {40, 12, 7};
    double *arr = malloc(n*sizeof(double)), *wait = malloc(n*sizeof(double)); int *cyl = malloc(n*sizeof(int));
    printf("=== Task 4.2: 10,000 random requests, head starts at 100 ===\n");
    for (int l = 0; l < 3; l++) {
        srand(42 + l); double t = 0;
        for (int i = 0; i < n; i++) { t += expo(mean[l]); arr[i] = t; cyl[i] = rand() % (MAXCYL+1); }
        printf("\n%s load (mean inter-arrival %.0f ms)\n%-7s %14s %14s %14s\n", lname[l], mean[l], "algo", "avg move/req", "avg wait (ms)", "max wait (ms)");
        for (int a = 0; a < NALG; a++) {
            double mv = run(a, n, arr, cyl, 100, wait), sum = 0, mx = 0;
            for (int i = 0; i < n; i++) { sum += wait[i]; if (wait[i] > mx) mx = wait[i]; }
            printf("%-7s %14.1f %14.1f %14.1f\n", nm[a], mv/n, sum/n, mx);
            if (l == 1 && (a == SCAN || a == CSCAN)) {
                double bs[4]={0},bc[4]={0};
                for (int i=0;i<n;i++){ int b=cyl[i]/50; bs[b]+=wait[i]; bc[b]++; }
                printf("        [uniformity, avg wait by cylinder band 0-49/50-99/100-149/150-199: %.1f %.1f %.1f %.1f ms]\n", bs[0]/bc[0],bs[1]/bc[1],bs[2]/bc[2],bs[3]/bc[3]);
            }
        }
    }
    /* Starvation test: request at cylinder 183 at t=0, then a request near 60 every 1 ms for 5 s */
    printf("\n=== Task 4.3: starvation test (head 53, request at 183, then one near cylinder 60 every 1 ms for 5000 ms) ===\n");
    int m = 5001; double *a2 = malloc(m*sizeof(double)), *w2 = malloc(m*sizeof(double)); int *c2 = malloc(m*sizeof(int));
    /* index 0: first stream request at t=0; index 1: the request at 183 arriving at t=0.5 ms; then 1 per ms */
    a2[0] = 0; c2[0] = 60; a2[1] = 0.5; c2[1] = 183;
    for (int i = 2; i < m; i++) { a2[i] = i - 1; c2[i] = 58 + (i % 5); }
    for (int a = 0; a < NALG; a++) { run(a, m, a2, c2, 53, w2); printf("%-7s request at 183 waited %.1f ms\n", nm[a], w2[1]); }
    return 0;
}
