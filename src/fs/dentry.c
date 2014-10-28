#include <string.h>
#include "filesystem.h"
#include "dentry.h"
#include "FreeRTOS.h"

void d_instantiate(struct dentry *dentry,struct inode *inode){
    dentry->d_inode = inode;
}

void d_drop(struct dentry *dentry){
    vPortFree(dentry);
}

struct dentry *__d_alloc(struct super_block *sb, const char *name){
    struct dentry *r;
    r = pvPortMalloc(sizeof(struct dentry));
    if(!r)
        return NULL;
    r->d_inode = NULL;
    r->d_sb = sb;
    r->d_op= sb->s_d_op;
    r->d_parent = r;
    r->d_count = 1;
    r->d_mounted = 0;
    /* Use fix size name at this time */
    memcpy(r->d_name,name,32);
    r->d_name[31] = '\0';
    return r;
}

struct dentry *d_alloc(struct dentry *parent, const char *name){
    struct dentry *r = __d_alloc(parent ->d_sb, name);
    if(!r)
        return NULL;
    return r;
}

struct dentry *d_alloc_root(struct inode *root_inode){
    struct dentry *r = NULL;
    if(root_inode){
        static const char *name = "/";
        r = __d_alloc(root_inode->i_sb,name);
        if(r)
            d_instantiate(r,root_inode);
    }
    return r;
}

void d_set_d_op(struct dentry *dentry, const struct dentry_operations *d_op){
    dentry->d_op = d_op;
}
