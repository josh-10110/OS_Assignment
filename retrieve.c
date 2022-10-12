//Program that retrieves file

// including header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libmemdrv.h"

// defining consdtants

#define NDIRECT 12
#define NINDIRECT 1
#define MAXFILE 4864

// inode struct

typedef struct fs_inode {
    short type; // File type
    short major; // Major device number
    short minor; // Minor device number
    short nlink; // Number of links to inode in file system
    int size; // Size of file (bytes)
    int addrs[NDIRECT + 1]; // Data block addresses
    } Inode;

// main method

int main(void) {
    int fd;

    // open ramdisk
    if ((fd = open("/dev/memdrv", O_RDWR)) < 0) {
        fprintf(stderr, "open /dev/memdrv failed ");
        exit(1);
    }

    // read inode from the ramdisk
    //Inode inode;

    char buf[BLOCK_SIZE];

    read_block(fd, buf);


    //read the data blocks from the ramdisk
    int i;
    for (i = 0; i < NDIRECT; i++) {
        read_block(fd, buf);

        //write the data blocks to the file
        write_block(1, buf);
    }

    // read the indirect block from ramdisk
    //int indirect[NINDIRECT];
    read_block(fd, buf);

    // read the data blocks from the indirect block
    for (i = 0; i < NINDIRECT; i++) {
        read_block(fd, buf);

        // write data blocks to file
        write_block(1, buf);

    }
}
