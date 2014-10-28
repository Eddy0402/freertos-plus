#ifndef __FIO_H__
#define __FIO_H__

#include <stdio.h>
#include <stdint.h>

enum open_flags {
    /* Base flags */
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR = 2,
    /* Addtional flags to be `or` */
    O_DIRECTORY = 4,
    O_CREAT = 8,
    O_TRUNC = 16,
    O_APPEND = 32,
};

enum mode_t {
    DUMMY
    /* Not implement file creation yet */
};

#define EBADF  -1 /* Not a valid file descriptor */
#define ENFILE -2
#define EACCES -3

struct dirent;
struct inode;

struct file{
    struct inode *f_inode;
    struct file_operations *f_op;
    uint32_t f_flags;
    uint32_t f_mode;
    uint32_t f_pos;
};

struct file_operations{
    uint32_t (*llseek)(struct file *,uint32_t off,int origin);
    uint32_t (*read)(struct file *,char *buf, uint32_t count);
    uint32_t (*write)(struct file *,const char *buf, uint32_t count);
    int (*readdir)(struct file *, struct dirent* entry, struct dirent ** result);
    int (*open)(struct inode *inode,struct file *file);
    int (*close)(struct file *file);
};

/* File IO interface */
int fio_open(const char *name,int flags,mode_t mode);
ssize_t fio_read(int fd,void *buf, size_t count);
ssize_t fio_write(int fd,const void *buf,size_t count);
size_t fio_lseek(int fd, size_t pos,int origin);
int fio_close(int fd);

int file_alloc(struct file *,struct file_operations *);

#endif
