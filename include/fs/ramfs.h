#ifndef __RAMFS_H__
#define __RAMFS_H__

#include "filesystem.h"
#include "dentry.h"
#include "fio.h"

extern struct file_system_type ramfs;

#define RAMFS_INODE_MAX 100

#endif
