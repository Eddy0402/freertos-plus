#ifndef __DENTRY_H__
#define __DENTRY_H__

struct dentry{
    struct inode *d_inode;
    struct super_block *d_sb;
    struct dentry *d_parent;
    uint32_t d_count;
    uint32_t d_mounted; // Is it mount point?
    char d_name[32];
};

struct dentry_operations{
    int (*d_hash) (struct dentry *,char *);
    int (*d_compare) (struct dentry *, char *, char *);
    int (*d_delete) (struct dentry *dentry);
};

void d_instantiate(struct dentry *,struct inode *);
void d_drop(struct dentry *);
struct dentry * d_alloc(struct dentry *, const char *);
void d_set_d_op(struct dentry *, const struct dentry_operations *);
struct dentry *d_lookup(const struct dentry *, const char *);

#endif
