// libmemdrv.h

#define DEVICE "/dev/memdrv"
#define BLOCK_SIZE 64 //0x1000
#define MAX_BID 78 //0x100
#define DEVICE_SIZE MAX_BID * BLOCK_SIZE

void open_device(void);
void read_block(int, char *);
void write_block(int, char *);
void close_device(void);
