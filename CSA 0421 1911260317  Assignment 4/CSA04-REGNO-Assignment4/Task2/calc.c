#include <stdio.h>
int main(void){
  long long B=4096, fsz=4LL<<30, nb=fsz/B, ptr=B/4;
  printf("4 GB file, 4 KB blocks: %lld blocks (last block number %lld)\n", nb, nb-1);
  printf("Contiguous: 1 read (start+offset)\nLinked: %lld reads (follow %lld pointers)\nFAT in RAM: %lld table look-ups + 1 disk read\n", nb, nb-1, nb-1);
  long long d=12, s=ptr, dd=ptr*ptr, t=ptr*ptr*ptr;
  printf("Indexed: direct 12, single covers %lld, double covers %lld; block %lld lies in the %s region -> 3 reads (double idx, single idx, data)\n", s, dd, nb-1, (nb-1)<d? "direct":(nb-1)<d+s?"single":(nb-1)<d+s+dd?"double":"triple");
  long long tot=(d+s+dd+t)*B;
  printf("Max file size = (12 + %lld + %lld + %lld) x 4096 = %lld bytes = %.3f TiB\n", s,dd,t,tot,tot/1099511627776.0);
  long long disk=4LL<<40, blocks=disk/B;
  printf("4 TB disk: %lld blocks; bit vector = %lld bits = %lld MB\n", blocks, blocks, blocks/8/1048576);
  printf("FAT for the same disk: %lld entries x 4 B = %lld GB\n", blocks, blocks*4/1073741824);
  printf("Linked free list: 1 pointer (4 B) kept inside every free block, 0 extra table\n");
  return 0; }
