#include <string.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "fio.h"
#include "filesystem.h"
#include "dentry.h"
#include "mount.h"
#include "hash-djb2.h"

#define MAX_FDS 32

static struct file fds[MAX_FDS];
static xSemaphoreHandle fio_sem = NULL;
enum KeyName{ESC=27, BACKSPACE=127};

/* Static functions */

static inline void __lock(){
    xSemaphoreTake(fio_sem, portMAX_DELAY);
}

static inline void __unlock(){
    xSemaphoreGive(fio_sem);
}

static inline int fio_is_open(int fd){
    if ((fd < 0) || (fd >= MAX_FDS))
        return 0;
    if( NULL == fds[fd].f_inode )
        return 0;
    return 1;
}

static inline int fio_is_open_int(int fd){
    __lock();
    int res = fio_is_open(fd);
    __unlock();
    return res;
}

static inline int fio_findfd() {
    int i;
    for (i = 0; i < MAX_FDS; i++) {
        if (!fio_is_open(i))
            return i;
    }
    return ENFILE;
}

static inline struct file *getfile(int fd) {
    if ((fd < 0) || (fd >= MAX_FDS))
        return NULL;
    return &fds[fd];
}


/* File IO interface */

int fio_open(const char *name,int flags,mode_t mode){
    /* Find a valid fd */
    __lock();
    int fd = fio_findfd();
    if (fd >= 0) {
        fds[fd].f_inode = (struct inode *)~0x0; /* claim fd */
    }else{
        return fd;
    }
    __unlock();

    /* Get dentry */
    struct dentry *root = mnt_namespace.root->mnt_root;
    struct dentry *target = root->d_op->d_lookup(root,name);
    struct inode *inode = target->d_inode;
    if( inode != NULL ){
        fds[fd] = (struct file){
            .f_inode = inode,
            .f_op = inode->i_fop,
            .f_flags = flags,
            .f_mode = mode,
            .f_pos = 0,
        };
        /* Flags and mode are pass to concrete file system */
        int err = fds[fd].f_op->open(inode,&fds[fd]);
        if(err == 0){
            return fd;
        }else{
            __lock();
            fds[fd].f_inode = NULL;
            __unlock();
            return err;
        }
    }else{
        __lock();
        fds[fd].f_inode = NULL;
        __unlock();
        return EACCES;
    }
}

ssize_t fio_read(int fd,void *buf, size_t count){
    if (fio_is_open_int(fd)) {
        return fds[fd].f_op->read(getfile(fd),buf,count);
    } else {
        return EBADF;
    }
}

ssize_t fio_write(int fd,const void *buf,size_t count){
    if (fio_is_open_int(fd)) {
        return fds[fd].f_op->write(getfile(fd),buf,count);
    } else {
        return EBADF;
    }
}

size_t fio_lseek(int fd, size_t pos,int origin){
    if (fio_is_open_int(fd)) {
        return fds[fd].f_op->llseek(getfile(fd),pos,origin);
    } else {
        return EBADF;
    }
}

int fio_close(int fd){
    int res = 0;
    if(fds[fd].f_inode){
        res = fds[fd].f_op->close(&fds[fd]);
        __lock();
        memset(&fds[fd],0,sizeof(struct file));
        __unlock();
    }else{
        res = EBADF;
    }
    return res;
}


/* Need to be called before using any other fio functions */
__attribute__((constructor)) void fio_init() {
    memset(fds, 0, sizeof(fds));
    fio_sem = xSemaphoreCreateMutex();
}

