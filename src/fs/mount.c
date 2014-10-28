#include "mount.h"

struct mnt_namespace mnt_namespace;

__attribute__((constructor)) void vfs_init(){
    INIT_LIST_HEAD(&mnt_namespace.mount_points);
    static struct vfsmount root = {
        .mnt_parent = NULL,
        .mnt_mountpoint = NULL,
        .mnt_root = NULL,
        .mnt_sb = NULL,
        .mnt_list = LIST_HEAD_INIT(root.mnt_list),
    };
    mnt_namespace.root = &root;
}


