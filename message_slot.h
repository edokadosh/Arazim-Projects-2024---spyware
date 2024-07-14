

#define __KERNEL__
#define MODULE
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/list.h>


#define DEVICE_NAME "message_slot"



#define MAJOR_NUM (235)

#define MAX_MSG_SIZE (128)

#define MSG_SLOT_CHANNEL (3)

typedef struct channel {
    spinlock_t lock;
    uint id;
    int len;
    char * msg;
} channel_t;

typedef struct node_s {
    struct  channel channel;
    struct node_s * next;
} node_t;

typedef struct device_node_s {
    spinlock_t lock;
    uint32_t minor;
    node_t * first;
    struct device_node_s * next;
}  device_node_t;


typedef struct file_private_data_s {
    channel_t * current_channel;
    device_node_t * list_head;
} file_data_t;

// typedef struct inode_private_data_s {
//     struct list_head list;
// } inode_data_t;


static int device_open(struct inode * ip, struct file * file);
static int device_release(struct inode * ip, struct file * file);
static long device_ioctl(struct file * file, uint ioctl_num, ulong ioctl_param);
static ssize_t device_read(struct file * file, char * buffer, size_t size, loff_t * offset);
static ssize_t device_write(struct file * file, const char * buffer, size_t size, loff_t * offset);

