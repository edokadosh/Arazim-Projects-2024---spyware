#include "message_slot.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Avner Spira");
MODULE_DESCRIPTION("message slot device driver");


void init_node(node_t *);
void init_channel(channel_t *);
node_t * query_list(device_node_t * device, uint32_t id);
device_node_t * query_list_list(uint minor);
int alloc_node(node_t * new_node);

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
    
    if (register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops) == -1)
    {
        printk(KERN_ERR "message slot device registration failed\n");
    }

    return 0;
}

static void __exit message_slot_exit(void) {

    device_node_t * device = list_list;
    device_node_t * next_device = NULL;

    while(device != NULL) {
        node_t * node = device->first;
        while (node != NULL) {
            node_t * next_node = node->next;
            if (node->channel.msg != NULL) {
                kfree(node->channel.msg);
            }
            kfree(node);
            node = next_node;
        }
        device = device->next;
    }

    device = list_list;
    while(device != NULL) {
        next_device = device->next;
        kfree(device);
        next_device = device;
    }
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}

module_init(message_slot_init);
module_exit(message_slot_exit);

static int device_open(struct inode * inode, struct file * file)
{

    uint minor = iminor(inode);
    device_node_t * device_node = query_list_list(minor);
    if (device_node == NULL) {
        if (!(device_node = kmalloc(sizeof(device_node), GFP_KERNEL))) {
            printk(KERN_ALERT "my_device: Failed to allocate memory\n");
            return -ENOMEM;            
        }
        spin_lock_init(&device_node->lock);
        device_node->minor = minor;
        device_node->first = NULL;
        device_node->next = list_list;
        spin_lock(&lock);
        list_list = device_node;
        spin_unlock(&lock);
    }

    file_data_t * file_data;

    if (file->private_data == NULL)
    {
        if (!(file_data = kmalloc(sizeof(file_data_t), GFP_KERNEL)))
        {
            printk(KERN_ALERT "my_device: Failed to allocate memory\n");
            return -ENOMEM;
        }
        file_data->current_channel = NULL;
        file_data->list_head = device_node;
        file->private_data = (void *)file_data;
    }

    return 0;
}

static int device_release(struct inode * inode, struct file * file)
{
    if (file->private_data != NULL) {
        *(file_data_t*)(file->private_data) = (file_data_t){0};
        kfree(file->private_data);
    }
    return 0;
}

static long device_ioctl(struct file * file, uint ioctl_cmd, ulong ioctl_param)
{
    switch(ioctl_cmd) {
    
    case MSG_SLOT_CHANNEL:
        if (file->private_data == NULL || ioctl_param == 0) {
            return -EINVAL;
        }
        file_data_t * file_data = (file_data_t *)file->private_data;
        node_t * query_res = query_list(file_data->list_head, ioctl_param);
        if (query_res != NULL) {
            file_data->current_channel = &query_res->channel;
            return 0;
        }
        else {
            node_t * new_node;
            int res;
            if ((res = alloc_node(new_node)) != 0) return res;
            new_node->channel.id = ioctl_param;
            spin_lock(&file_data->list_head->lock);
            new_node->next = file_data->list_head->first;
            file_data->list_head->first = new_node;
            spin_unlock(&file_data->list_head->lock);
            file_data->current_channel = &new_node->channel;
            return 0;
        }
    default:
        return -EINVAL;
    }

    return 0;
}

static ssize_t device_read(struct file * file, char * buffer, size_t size, loff_t * offset)
{
    ulong ret = 0;
    file_data_t * file_data = (file_data_t *)file->private_data;
    channel_t * channel = file_data->current_channel;
    loff_t off = 0;
    if (channel->id == 0) return -EINVAL;
    if (channel->len == 0) return -EWOULDBLOCK;
    if (channel->len > size) return -ENOSPC;

    if (!(ret = copy_from_user(&off, offset, sizeof(*offset)))) {
        return -EFAULT;
    }

    spin_lock(&channel->lock);
    if (!(ret = copy_to_user(buffer + off, channel->msg, channel->len))) {
        spin_unlock(&channel->lock);
        return -EFAULT;
    }
    spin_unlock(&channel->lock);
    return ret;
}

static ssize_t device_write(struct file * file, const char * buffer, size_t size, loff_t * offset)
{
    
    // TODO validate and handle offset
    ulong ret = 0;
    file_data_t * file_data = (file_data_t *)file->private_data;
    channel_t * channel = file_data->current_channel;
    loff_t off = 0;
    if (channel->id == 0) return -EINVAL;
    if (size <= 0 || size > MAX_MSG_SIZE) return -EMSGSIZE;

    if (!(ret = copy_from_user(&off, offset, sizeof(*offset)))) {
        return -EFAULT;
    }



    if (channel->msg != NULL) {
        kfree(channel->msg);
    }
    if (!(channel->msg = kmalloc(size, GFP_KERNEL))) {
        return -ENOMEM;
    }
    spin_lock(&channel->lock);
    if ((ret = copy_from_user(channel->msg, buffer + off, size))) {
        spin_unlock(&channel->lock);
        return -EFAULT;
    }
    spin_unlock(&channel->lock);
    return ret;
}



void init_channel(channel_t * channel) {
    *channel = (channel_t){.id = 0, .len = 0, .msg = NULL};
    spin_lock_init(&channel->lock);
}

// void init_node(node_t * node) {
//     init_channel(&node->channel);
//     INIT_LIST_HEAD(&node->list);
// }


// void release_node(node_t * node) {
//     list_del(&node->list);
//     kfree(node);
// }

int alloc_node(node_t * new_node) {
    if (!(new_node = kmalloc(sizeof(new_node), GFP_KERNEL))) {
        printk(KERN_ALERT "my_device: Failed to allocate memory\n");
        return -ENOMEM;
    }
    init_channel(&new_node->channel);
    new_node->next = NULL;
    return 0;
}  

node_t * query_list(device_node_t * device, uint32_t id) {
    spin_lock(&device->lock);
    node_t * node = device->first;
    while (node != NULL) {
        if (node->channel.id == id) {
            spin_unlock(&device->lock);
            return node;
        }
    }
    spin_unlock(&device->lock);
    return NULL;
}


device_node_t * query_list_list(uint minor) {
    spin_lock(&lock);
    device_node_t * node = list_list;
    while (node != NULL) {
        if (node->minor == minor) {
            spin_unlock(&lock);
            return node;
        }
    }
    spin_unlock(&lock);
    return NULL;
}
