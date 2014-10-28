#ifndef __MOUNT_H__
#define __MOUNT_H__

#include <stdint.h>
#include "link_list.h"

struct vfsmount{
    struct vfsmount *mnt_parent;
    struct dentry *mnt_mountpoint;
    struct dentry *mnt_root;
    struct super_block *mnt_sb;
    struct list_head mnt_list;
};

struct mnt_namespace{
    struct list_head mount_points;
    struct vfsmount *root;
};

extern struct mnt_namespace mnt_namespace;

int mount(const char *dir);

#endif
