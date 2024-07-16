#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif


#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/list.h>
#include "message_slot.h"

#define DEVICE_NAME ("message_slot")

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Avner Spira");
MODULE_DESCRIPTION("message slot device driver");


typedef struct channel {
    uint id;
    int len;
    char * msg;
} channel_t;

typedef struct node_s {
    struct  channel channel;
    struct node_s * next;
} node_t;

typedef struct device_node_s {
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
void init_node(node_t *);
void init_channel(channel_t *);
node_t * query_list(device_node_t * device, uint32_t id);
device_node_t * query_list_list(uint minor);
int alloc_node(node_t ** new_node);

static device_node_t * list_list = NULL;
static DEFINE_SPINLOCK(lock);

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl
};

static int __init message_slot_init(void) {
    
    int ret = 0;
    if ((ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops)) == -1)
    {
        printk(KERN_ERR "message slot device registration failed, error: %d\n", ret);
        return -1;
    }
    
    printk(KERN_DEFAULT "message slot device registration succsess\n");
    return 0;
}

static void __exit message_slot_exit(void) {

    printk(KERN_DEBUG "starting unregistration\n");

    printk(KERN_DEBUG "starting unregister_chardev\n"); // TODO change
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk(KERN_DEBUG "finnsih unregister_chardev\n");
    
    device_node_t * device = list_list;
    device_node_t * next_device = NULL;

    while(device != NULL) {
        node_t * node = device->first;

        while (node != NULL) {
            node_t * next_node = node->next;
            if (node->channel.msg != NULL) {
                printk(KERN_DEBUG "starting kfree channel.msg\n");
                kfree(node->channel.msg);
            }
            printk(KERN_DEBUG "starting kfree node\n");
            kfree(node);
            node = next_node;
        }
        device = device->next;
    }


    device = list_list;
    while(device != NULL) {
        next_device = device->next;
        printk(KERN_DEBUG "starting kfree device: %p\n", device);
        kfree(device);
        device = next_device;
    }

}

module_init(message_slot_init);
module_exit(message_slot_exit);

static int device_open(struct inode * inode, struct file * file)
{

    spin_lock(&lock);
    printk(KERN_DEBUG "starting %s\n", __func__);
    
    uint minor = iminor(inode);
    device_node_t * device_node = query_list_list(minor);
    if (device_node == NULL) {
        if (!(device_node = kmalloc(sizeof(device_node), GFP_KERNEL))) {
            printk(KERN_ALERT "my_device: Failed to allocate memory\n");
            spin_unlock(&lock);
            return -ENOMEM;            
        }
        device_node->minor = minor;
        device_node->first = NULL;
        device_node->next = list_list;
        list_list = device_node;
    }

    file_data_t * file_data = NULL;

    if (file->private_data == NULL)
    {
        if (!(file_data = kmalloc(sizeof(file_data_t), GFP_KERNEL)))
        {
            printk(KERN_ALERT "my_device: Failed to allocate memory\n");
            spin_unlock(&lock);
            return -ENOMEM;
        }
        file_data->current_channel = NULL;
        file_data->list_head = device_node;
        file->private_data = (void *)file_data;
    }
    printk(KERN_DEBUG "finnishing %s\n", __func__);
    spin_unlock(&lock);
    return 0;
}

static int device_release(struct inode * inode, struct file * file)
{
    printk(KERN_DEBUG "starting %s\n", __func__);
    if (file->private_data != NULL) {
        *(file_data_t*)(file->private_data) = (file_data_t){0};
        printk(KERN_DEBUG "kfree file->private_data, unfound node\n");
        kfree(file->private_data);
    }
    printk(KERN_DEBUG "finnishing %s\n", __func__);
    return 0;
}

static long device_ioctl(struct file * file, uint ioctl_cmd, ulong ioctl_param)
{
    spin_lock(&lock);
    printk(KERN_DEBUG "starting %s\n", __func__);
    
    switch(ioctl_cmd) {
    
    case MSG_SLOT_CHANNEL:
        if (file->private_data == NULL || ioctl_param == 0) {
            spin_unlock(&lock);
            return -EINVAL;
        }
        file_data_t * file_data = (file_data_t *)file->private_data;
        node_t * query_res = query_list(file_data->list_head, ioctl_param);
        if (query_res != NULL) {
            file_data->current_channel = &query_res->channel;
            printk(KERN_DEBUG "finnishing %s, channel already existed\n", __func__);
            spin_unlock(&lock);
            return 0;
        }
        else {
            node_t * new_node = NULL;
            int res;
            printk(KERN_DEBUG "1\n");
            if ((res = alloc_node(&new_node)) != 0) {
                printk(KERN_ERR "error alloc node\n");
                printk(KERN_DEBUG "finnishing %s\n", __func__);
                spin_unlock(&lock);
                return res;
            }
            printk(KERN_DEBUG "2\n");
            new_node->channel.id = ioctl_param;
            printk(KERN_DEBUG "3\n");
            printk(KERN_DEBUG "4\n");
            new_node->next = file_data->list_head->first;
            printk(KERN_DEBUG "5\n");
            file_data->list_head->first = new_node;
            printk(KERN_DEBUG "6\n");
            printk(KERN_DEBUG "7\n");
            file_data->current_channel = &new_node->channel;
            printk(KERN_DEBUG "finnishing %s, alloced new channel\n", __func__);
            spin_unlock(&lock);
            return 0;
        }
    default:
        spin_unlock(&lock);
        return -EINVAL;
    }
    printk(KERN_DEBUG "finnishing %s\n", __func__);
    spin_unlock(&lock);
    return 0;
}

static ssize_t device_read(struct file * file, char * buffer, size_t size, loff_t * offset)
{
    spin_lock(&lock);
    printk(KERN_DEBUG "starting %s\n", __func__);
    
    ulong ret = 0;
    file_data_t * file_data = (file_data_t *)file->private_data;
    channel_t * channel = file_data->current_channel;
    loff_t off = 0;
    if (channel->id == 0) {
        spin_unlock(&lock);
        return -EINVAL;
    }
    if (channel->len == 0) {
        spin_unlock(&lock);
        return -EWOULDBLOCK;
    }
    if (channel->len > size) {
        spin_unlock(&lock);
        return -ENOSPC;
    }

    ret = copy_from_user(&off, offset, sizeof(off));
    if (ret != sizeof(off) && ret != 0) {
        printk(KERN_DEBUG "copy_fron user return value for offset: %lu\n", ret);
        spin_unlock(&lock);
        return -EFAULT;
    }
    if (ret == 0) {
        off = 0;
    }
    printk(KERN_DEBUG "copy_fron user return value for offset: %lu\n", ret);


    if ((ret = copy_to_user(buffer + off, channel->msg, channel->len)) != 0) {
        printk(KERN_DEBUG "copy_to_user return value for msg: %lu\n", ret);
        spin_unlock(&lock);
        return -EFAULT;
    }
    printk(KERN_DEBUG "copy_to_user return value for msg: %lu\n", ret);
    printk(KERN_DEBUG "finnishing %s\n", __func__);
    spin_unlock(&lock);
    return channel->len;
}

static ssize_t device_write(struct file * file, const char * buffer, size_t size, loff_t * offset)
{
    
    spin_lock(&lock);
    printk(KERN_DEBUG "starting %s\n", __func__);
    
    ulong ret = 0;
    file_data_t * file_data = (file_data_t *)file->private_data;
    channel_t * channel = file_data->current_channel;
    loff_t off = 0;
    if (channel->id == 0) {
        spin_unlock(&lock);
        return -EINVAL;
    }
    if (size <= 0 || size > MAX_MSG_SIZE) {
        spin_unlock(&lock);
        return -EMSGSIZE;
    }

    ret = copy_from_user(&off, offset, sizeof(off));
    if (ret != sizeof(off) && ret != 0) {
        printk(KERN_DEBUG "copy_fron user return value for offset: %lu\n", ret);
        spin_unlock(&lock);
        return -EFAULT;
    }
    if (ret == 0) {
        off = 0;
    }
    printk(KERN_DEBUG "copy_fron user return value for offset: %lu\n", ret);



    if (channel->msg != NULL) {
        kfree(channel->msg);
    }
    if (!(channel->msg = kmalloc(size, GFP_KERNEL))) {
        return -ENOMEM;
    }
    if ((ret = copy_from_user(channel->msg, buffer + off, size)) != 0) {
        printk(KERN_DEBUG "copy_fron user return value for msg: %lu\n", ret);
        channel->len = size - ret;
        spin_unlock(&lock);
        return -EFAULT;
    }
    printk(KERN_DEBUG "copy_fron user return value for msg: %lu\n", ret);
    printk(KERN_DEBUG "msg: %s\n", channel->msg);

    channel->len = size - ret;
    printk(KERN_DEBUG "finnishing %s\n", __func__);
    spin_unlock(&lock);
    return channel->len;
}



void init_channel(channel_t * channel) {
    *channel = (channel_t){.id = 0, .len = 0, .msg = NULL};
}

// void init_node(node_t * node) {
//     init_channel(&node->channel);
//     INIT_LIST_HEAD(&node->list);
// }


// void release_node(node_t * node) {
//     list_del(&node->list);
//     kfree(node);
// }

int alloc_node(node_t ** new_node) {
    if (!(*new_node = kmalloc(sizeof(new_node), GFP_KERNEL))) {
        printk(KERN_ALERT "my_device: Failed to allocate memory\n");
        return -ENOMEM;
    }
    init_channel(&(*new_node)->channel);
    (*new_node)->next = NULL;
    return 0;
}  

node_t * query_list(device_node_t * device, uint32_t id) {
    printk(KERN_DEBUG "start query_list\n");
    node_t * node = device->first;
    while (node != NULL) {
        if (node->channel.id == id) {
            printk(KERN_DEBUG "finnish query_list, found node\n");
            return node;
        }
    }
    printk(KERN_DEBUG "finnish query_list, unfound node\n");
    return NULL;
}


device_node_t * query_list_list(uint minor) {
    printk(KERN_DEBUG "start query_list_list\n");
    device_node_t * node = list_list;
    while (node != NULL) {
        if (node->minor == minor) {
            printk(KERN_DEBUG "finnish query_list_list, found node\n");
            return node;
        }
    }
    printk(KERN_DEBUG "finnish query_list_list, unfound node\n");

    return NULL;
}
