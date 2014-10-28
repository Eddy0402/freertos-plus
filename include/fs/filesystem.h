#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <stdint.h>
#include <hash-djb2.h>

#define MAX_FS 16
#define OPENFAIL (-1)

#define OPENDIR_NOTFOUNDFS (-2)
#define OPENDIR_NOTFOUND (-1)

typedef int (*fs_open_t)(void * opaque, const char * fname, int flags, int mode);
typedef int (*fs_open_dir_t)(void * opaque, const char * fname);

struct file_system_type {
    struct super_block *(*get_sb) (int flags, char *name, void *data);
    void (*kill_sb) (struct super_block *);
};

struct dentry_operations;

struct super_block{
    struct dentry    *s_root;
    struct super_operations *s_op;
    struct dentry_operations *s_d_op; /* default dentry operations */
    void *opaque;
};

struct super_operations{
    struct inode *(*alloc_inode)(struct super_block *sb);
    void (*destroy_inode)(struct inode*);
    void (*write_inode)(struct inode*);
    void (*write_super)(struct super_block *sb);
};

#define DT_REG 1
#define DT_DIR 2

struct inode{
    uint32_t i_no;
    uint32_t i_type;
    uint32_t i_mode;
    uint32_t i_nlink;
    uint32_t i_size;
    struct inode_operations *i_op;
    struct super_block *i_sb;
};

struct inode_operations{
    int (*create)(struct inode *,struct dentry *,int);
    struct dentry *(*lookup)(struct inode *,struct dentry *);
    int (*link)(struct dentry *,struct inode *,struct dentry);
    int (*unlink) (struct inode *, struct dentry *);
    int (*mkdir) (struct inode *, struct dentry *, int);
    int (*rmdir) (struct inode *, struct dentry *);
};


/* Need to be called before using any other fs functions */
__attribute__((constructor)) void fs_init();

int register_fs(const char * mountpoint, fs_open_t callback, fs_open_dir_t dir_callback, void * opaque);
int fs_open(const char * path, int flags, int mode);
int fs_opendir(const char * path);

#endif
