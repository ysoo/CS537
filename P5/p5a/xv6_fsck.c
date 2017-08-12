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
  if(fd == 0) {
      perror("Image not found\n");
      exit(1);
  }
  fstat(fd,&buf);
  void *imgptr = (void*)mmap(NULL, buf.st_size, PROT_READ,MAP_PRIVATE,fd,0);
  struct superblock *sb =(struct superblock*)(imgptr+BSIZE);
  struct dinode *dip = (struct dinode*)(imgptr+BSIZE*2);
//  uchar *bitmap = (uchar*)(imgptr+BSIZE*28);

  int const ninodes = (int)sb->ninodes;
  int const nblks = (int)sb->nblocks;
  int const size = (int)sb->size;
  uchar *bitmap = (uchar*)(imgptr+BSIZE*(ninodes)/8+4);


  for(int i=0;i<ninodes;i++){
    //TODO:
    // Each inode is either unallocated or one
    // of the valid types (T_FILE, T_DIR, T_DEV). ERROR: bad inode.
    //for loop 200 times to theck
    if(!(dip[i].type == 0 ||dip[i].type == 1 || dip[i].type == 2 || dip[i].type == 3)){
      perror("ERROR: bad inode.\n");
      exit(1);
    }
    //try to set up two arrays to record usage of data blks

      //TODO:
      //Root directory exists, and it is inode number 1. ERROR: root directory does not exist.
      //check the first dirent of first inode, inum == 1, and  if the name is "."
      if(i == 1 && dip[i].type != 2){
          perror("ERROR: root directory does not exist.\n");
          exit(1);
      }


      //TODO:
      //For in-use inodes, each address that is used by inode is valid (points to a valid datablock address within the image).
      //If the direct block is used and is invalid, print ERROR: bad direct address in inode.
    for(int j=0;j<NDIRECT;j++){
        if(dip[i].addrs[j]==0){
        }
        else if(dip[i].addrs[j]>size||dip[i].addrs[j]<*(uint*)((uchar*)(imgptr+BSIZE*(ninodes)/8+5))) {
            //larger than size or smaller than the area of non-data blks
            perror("ERROR: bad direct address in inode.\n");
            exit(1);
        }

        else if(dip[i].type == 2){
            //TODO:
            //For in-use inodes, direct address in use is only used once. ERROR: direct address used more than once.


        }
    }
    //TODO:
    // if the indirect block is in use and is invalid,print ERROR: bad indirect address in inode.
    if(dip[i].addrs[NINDIRECT]==0){
    } else if(dip[i].addrs[NINDIRECT]>size||dip[i].addrs[NINDIRECT]<*(uint*)((uchar*)(imgptr+BSIZE*(ninodes)/8+5))) {
        //larger than size or smaller than the area of non-data blks
        perror("ERROR: bad indirect address in inode.\n");
        exit(1);
    }

  }





  //TODO:
  //Each directory contains . and .. entries. ERROR: directory not properly formatted.
  //all inodes, first two dirent are "." and ".."

  //TODO:
  //Each .. entry in directory refers to the proper parent inode, and parent inode points back to it. ERROR: parent directory mismatch.

  //TODO:
  //For in-use inodes, each address in use is also marked in use in the bitmap. ERROR: address used by inode but marked free in bitmap.

  //TODO:
  //For blocks marked in-use in bitmap, actually is in-use in an inode or indirect block somewhere. ERROR: bitmap marks block in use but it is not in use.




  //TODO:
  //For in-use inodes, indirect address in use is only used once. ERROR: indirect address used more than once.

  //TODO:
  //For all inodes marked in use, must be referred to in at least one directory. ERROR: inode marked use but not found in a directory.

  //TODO:
  //For each inode number that is referred to in a valid directory, it is actually marked in use. ERROR: inode referred to in directory but marked free.

  //TODO:
  //Reference counts (number of links) for regular files match the number of times file is referred to in directories (i.e., hard links work correctly)
  //ERROR: bad reference count for file.

  //TODO:
  //No extra links allowed for directories (each directory only appears in one other directory). ERROR: directory appears more than once in file system.


  exit(0);
}
