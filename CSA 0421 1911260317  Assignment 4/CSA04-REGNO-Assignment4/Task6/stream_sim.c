/* Task 6: HDD-only (Design A) vs HDD + SSD video cache (Design B). Zipf popularity, video-level LRU.
   ASSUMED devices: HDD 8 ms + 160 MB/s, SSD 0.1 ms + 1500 MB/s; chunk 4 MB = 6.4 s of 5 Mb/s video. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define V 20000
#define REQ 200000
int main(void)
{
    double hdd = 8.0 + 4.0/160*1000, ssd = 0.1 + 4.0/1500*1000, cum[V], z = 0;
    for (int i = 0; i < V; i++) { z += 1.0/(i+1); cum[i] = z; }
    printf("=== Task 6 stage 5: simulation (%d chunk requests, %d videos, Zipf s=1) ===\n", REQ, V);
    printf("HDD chunk time %.1f ms, SSD chunk time %.2f ms\n", hdd, ssd);
    printf("%-22s %9s %14s %20s\n", "design", "hit ratio", "mean HDD ms", "streams per HDD(80%)");
    printf("%-22s %8.1f%% %14.1f %20.0f\n", "A: HDD only", 0.0, hdd, 0.8*6400/hdd);
    int sizes[] = {200, 500, 1000, 2000, 4000};
    for (int c = 0; c < 5; c++) {
        int N = sizes[c]; static long last[V]; static char in[V]; for (int i=0;i<V;i++){last[i]=0;in[i]=0;}
        int cnt = 0; long hits = 0; srand(7);
        for (long r = 1; r <= REQ; r++) {
            double u = (rand()+0.5)/((double)RAND_MAX+1.0)*z; int lo=0, hi=V-1;
            while (lo<hi){int m=(lo+hi)/2; if (cum[m]<u) lo=m+1; else hi=m;}
            int v = lo;
            if (in[v]) { hits++; last[v] = r; }
            else { if (cnt >= N) { int ev=-1; long mn=1L<<60; for (int i=0;i<V;i++) if (in[i] && last[i]<mn){mn=last[i];ev=i;} in[ev]=0; cnt--; }
                   in[v]=1; last[v]=r; cnt++; }
        }
        double h = (double)hits/REQ, hm = (1-h)*hdd;
        char name[40]; snprintf(name, 40, "B: HDD + SSD cache %d", N);
        printf("%-22s %8.1f%% %14.1f %20.0f\n", name, 100*h, hm, 0.8*6400/hm);
    }
    printf("\n=== Task 6 stage 4: design calculations (peak week 1) ===\n");
    double peak_streams = 10000, bw = peak_streams*5.0/8;           /* MB/s */
    printf("Peak: %.0f streams x 5 Mb/s = %.0f MB/s = %.0f Gb/s\n", peak_streams, bw, bw*8/1000);
    printf("Library: 20,000 videos x 1 GB = 20 TB; 2 copies = 40 TB; 5x growth = 200 TB -> %d x 16 TB HDDs now\n", (int)ceil(40.0/16/0.8));
    printf("HDD with 4 MB chunks: %.0f MB/s effective; no cache needs %.0f HDDs\n", 4.0/(hdd/1000), ceil(peak_streams/(0.8*6400/hdd)));
    double hr[] = {0.8, 0.9};
    for (int k = 0; k < 2; k++) printf("With %.0f%% cache hits: HDDs needed = %.0f; SSD traffic %.0f MB/s -> %.1f NVMe drives (3 GB/s each)\n", hr[k]*100, ceil(peak_streams/(0.8*6400/((1-hr[k])*hdd))), bw*hr[k], bw*hr[k]/3000);
    printf("Reads to start playing at minute m of a 1 GB video: extents = 1 extent-tree lookup (in inode) + 1 data read = 2; FAT/linked: up to %d look-ups/reads (mid-video: 1 GB/4 KB/2)\n", 1048576/4/2);
    return 0;
}
