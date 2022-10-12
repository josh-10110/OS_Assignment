// Program to store file in ramdisk

// inlcuding header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libmemdrv.h"


// defining constants

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof (uint))
#define MAXFILE 4864

// inode struct defined

typedef struct fs_inode {
    short type; // File type
    short major; // Major device number (T_DEV only)
    short minor; // Minor device number (T_DEV only)
    short nlink; // Number of links to inode in file system
    int size; // Size of file (bytes)
    int addrs[NDIRECT + 1]; // Data block addresses
} Inode;

Inode inode;
char buf[BLOCK_SIZE];


// function that stores file in RAMDISK
// takes filename and random int as parameters
// return value is void
void store(char *filename, int random) {
    int fd, i;
    int nblocks;
    int blockno;
    int offset;
    int n;
    int *indirect;
    int *random_index;
    int temp;
    int random_index_count = 0;
    int random_index_size = 0;

    // open file
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "store: cannot open %s\n", filename);
        exit(1);
    }

    // calculate the number of blocks needed to store the file
    nblocks = (inode.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (nblocks > MAXFILE) {
        fprintf(stderr, "store: file too large\n");
        exit(1);
    }

    // allocate memory for random index
    if (random) {
        random_index_size = nblocks;
        random_index = (int *) malloc(random_index_size * sizeof(int));
        if (random_index == NULL) {
            fprintf(stderr, "store: cannot allocate memory\n");
            exit(1);
        }
    }

    // store the file in ramdisk
    offset = 0;
    for (i = 0; i < nblocks; i++) {
        // calculate the block number
        if (i < NDIRECT) {
            blockno = inode.addrs[i];
        } else {
            read_block(inode.addrs[NDIRECT], (char *) buf);

            indirect = (int *) buf;
            blockno = indirect[i - NDIRECT];
        }

        // store block
        if (random) {
            // store the block in a random order
            random_index[random_index_count++] = blockno;
        } else {
            // Store the block in order
            write_block(blockno, (char *) (buf + offset));

        }

        offset += BLOCK_SIZE;
    }

    // store file in random order
    if (random) {
        // Shuffle the random index
        for (i = 0; i < random_index_size; i++) {
            n = rand() % random_index_size;
            temp = random_index[i];
            random_index[i] = random_index[n];
            random_index[n] = temp;
        }

        // store the file in a random order
        offset = 0;
        for (i = 0; i < nblocks; i++) {
            // calculate block number
            if (i < NDIRECT) {
                blockno = inode.addrs[i];
            } else {
                read_block(inode.addrs[NDIRECT], (char *) buf);

                indirect = (int *) buf;
                blockno = indirect[i - NDIRECT];
            }

            // store block
            (write_block(random_index[i], (char *) (buf + offset)));
            

            offset += BLOCK_SIZE;
        }

        // free memory allocated for the random index
        free(random_index);
    }

    // close file
    close(fd);
    close_device();
}

// main method
int main(int argc, char *argv[]) {
    int random = 0;

    // Check the command-line arguments
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: store [-r] filename\n");
        exit(1);
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-r") == 0) {
            random = 1;
        } else {
            fprintf(stderr, "Usage: store [-r] filename\n");
            exit(1);
        }
    }

    // Store the file in the RAMDISK
    store(argv[argc - 1], random);

    return 0;
}