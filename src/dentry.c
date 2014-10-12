#include "filesystem.h"
#include "dentry.h"

void d_instantiate(struct dentry *dentry,struct inode *inode){
    dentry->d_inode = inode;
    dentry->d_sb = inode->i_sb;
    dentry
    dentry->d_count = 1;
    dentry->d_mounted = 
    dentry->d
}

void d_drop(struct dentry *){

}

struct dentry * d_alloc(struct dentry *, const char *){

}

void d_set_d_op(struct dentry *, const struct dentry_operations *){

}

struct dentry *d_lookup(const struct dentry *, const char *){

}
