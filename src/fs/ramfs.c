#include "ramfs.h"
#include "FreeRTOS.h"
#include "string.h"

/* File system operations */
struct super_block *ramfs_get_sb (int flags, char *name, void *data);
void ramfs_kill_sb(struct super_block *);

struct file_system_type ramfs = {
    .get_sb = ramfs_get_sb,
    .kill_sb = ramfs_kill_sb
};

struct directory_file {
    uint32_t df_count;
    uint32_t df_sorted;
    struct{
        uint32_t *df_hashs;
        uint8_t *df_filenames;
    } df_files[1];
};

/* Super block operations */
struct inode *ramfs_s_alloc_inode(struct super_block *sb);
void ramfs_s_destroy_inode(struct inode*);
void ramfs_s_write_inode(struct inode*);
void ramfs_s_write_super(struct super_block *sb);

struct super_operations ramfs_s_op = {
    .alloc_inode = ramfs_s_alloc_inode,
    .destroy_inode = ramfs_s_destroy_inode,
    .write_inode = ramfs_s_write_inode,
    .write_super = ramfs_s_write_super
};

/* Inode operations */
int ramfs_i_create (struct inode *,struct dentry *,int);
struct dentry *ramfs_i_lookup (struct inode *,struct dentry *);
int ramfs_i_link (struct dentry *,struct inode *,struct dentry*);
int ramfs_i_unlink (struct inode *, struct dentry *);
int ramfs_i_mkdir (struct inode *, struct dentry *, int);
int ramfs_i_rmdir (struct inode *, struct dentry *);

struct inode_operations ramfs_i_op = {
    .create = ramfs_i_create,
    .lookup = ramfs_i_lookup,
    .link = ramfs_i_link,
    .unlink = ramfs_i_unlink,
    .mkdir = ramfs_i_mkdir,
    .rmdir = ramfs_i_rmdir
};

/* File operations */
uint32_t ramfs_f_llseek(struct file *,uint32_t off,int origin);
uint32_t ramfs_f_read(struct file *,char *buf, uint32_t count, uint32_t offset);
uint32_t ramfs_f_write(struct file *,char *buf, uint32_t count, uint32_t offset);
int ramfs_f_readdir(struct file *, struct dirent* entry, struct dirent ** result);
int ramfs_f_open(struct inode *inode,struct file *file);

struct file_operations ramfs_f_op = {
    .llseek = ramfs_f_llseek,
    .read = ramfs_f_read,
    .write = ramfs_f_write,
    .readdir = ramfs_f_readdir,
    .open = ramfs_f_open
};


/* Function definitions */

struct super_block *ramfs_get_sb (int flags, char *name, void *data)
{
    (void)flags;(void)name;(void)data;
    struct super_block *sb = pvPortMalloc( sizeof(struct super_block) );
    sb -> s_op = &ramfs_s_op;
    sb -> s_root = NULL;
    sb -> s_inodes = pvPortMalloc( sizeof(struct inode) * RAMFS_INODE_MAX );
    return sb;
}

void ramfs_kill_sb (struct super_block* sb)
{
    vPortFree(sb->s_inodes);
    vPortFree(sb);
}

struct inode *ramfs_s_alloc_inode(struct super_block *sb){
    struct inode *inodes = sb->s_inodes;
    for(int i = 0; i < RAMFS_INODE_MAX; ++i){
        if( inodes[i].i_nlink ){
            inodes[i].i_no = i;
            return &inodes[i];
        }
    }
    return NULL;
}

void ramfs_s_destroy_inode(struct inode *ino){
    memset( ino, 0, sizeof(struct inode) );
}

void ramfs_s_write_inode (struct inode *ino){return;}

void ramfs_s_write_super (struct super_block *sb){return;}

int ramfs_i_create (struct inode *inode,struct dentry *dentry,int mode){
    inode = dentry->d_sb->s_op->alloc_inode(dentry->d_sb);
    inode->i_mode = mode;
    return 0; //TODO: error handling
}

struct dentry *ramfs_i_lookup (struct inode *inode,struct dentry *dentry){
    return NULL;    
}

int ramfs_i_link (struct dentry *old_dentry,struct inode *dir,struct dentry *dentry){
    directory_file *dir_data = dir->data->i_data;
    return -1;
} 

int ramfs_i_unlink (struct inode *dir, struct dentry *dentry){
    return -1;
}

int ramfs_i_mkdir (struct inode *dir, struct dentry *dentry, int mode){

    return -1;
}

int ramfs_i_rmdir (struct inode *dir, struct dentry *dentry){
    return -1;
}

uint32_t ramfs_f_llseek(struct file *file,uint32_t off,int origin){
    return 0;
}

uint32_t ramfs_f_read(struct file *file,char *buf, uint32_t count, uint32_t offset){
    return 0;
}

uint32_t ramfs_f_write(struct file *file,char *buf, uint32_t count, uint32_t offset){
    return 0;
}

int ramfs_f_readdir(struct file *file, struct dirent* entry, struct dirent ** result){
    return -1;
}

int ramfs_f_open(struct inode *inode,struct file *file){
    file->f_inode = inode;
    file->f_pos = 0;
}
