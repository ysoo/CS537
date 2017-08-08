#include <stdlib.h>
#include <stdio.h>
#include "fs.h"
#include "types.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[]){
  struct stat buf;
  int fd=open("fs.img",O_RDWR);
  fstat(fd,&buf);
  void *imgptr = (void*)mmap(NULL, buf.st_size, PROT_READ,MAP_PRIVATE,fd,0);
  struct superblock *sb =(struct superblock*)(imgptr+BSIZE);
  struct dinode *dip = (struct dinode*)(imgptr+BSIZE*2);
//  uchar *bitmap = (uchar*)(imgptr+BSIZE*28);

  int const ninodes = (int)sb->ninodes;
  int const nblks = (int)sb->nblocks;
  int const size = (int)sb->size;
  uchar *bitmap = (uchar*)(imgptr+BSIZE*(ninodes)/8+4);

  //TODO:
  // Each inode is either unallocated or one
  // of the valid types (T_FILE, T_DIR, T_DEV). ERROR: bad inode.
  //for loop 200 times to theck
  for(int i=0;i<ninodes;i++){
    if(!(dip[i].type == 0 ||dip[i].type == 1 || dip[i].type == 2 || dip[i].type == 3)){
      perror("ERROR: bad inode.\n");
      exit(1);
    }
    //try to set up two arrays to record usage of data blks
  }
  //TODO:
  //if a inode is used, check all of its address whether their val is a valid add
  //if direct addr, check if it is a valid addr
  //if indirect addr, check if it is a valid, then in that addr if all addrs are valid

  //TODO:
  //check the first dirent of first inode, inum == 1, and  if the name is "."

  //TODO:
  //all inodes, first two dirent are "." and ".."

  //TODO:
  exit(0);
}
