// msgdriver.c - simple char device for virtual message passing (Stage 1)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "msgdriver"
#define CLASS_NAME  "msgdrv"
#define MAX_BUF_SZ  1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jainish Parmar");
MODULE_DESCRIPTION("Simple message-passing char driver (Stage 1)");
MODULE_VERSION("0.1");

static dev_t dev_number;
static struct cdev msg_cdev;
static struct class *msg_class;
static struct device *msg_device;

static char kernel_buffer[MAX_BUF_SZ];
static size_t buffer_size = 0; // number of valid bytes in buffer

/* file ops prototypes */
static int msg_open(struct inode *inode, struct file *file);
static int msg_release(struct inode *inode, struct file *file);
static ssize_t msg_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t msg_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = msg_open,
    .release = msg_release,
    .read    = msg_read,
    .write   = msg_write,
};

static int msg_open(struct inode *inode, struct file *file)
{
    pr_info("msgdriver: open()\n");
    return 0;
}

static int msg_release(struct inode *inode, struct file *file)
{
    pr_info("msgdriver: release()\n");
    return 0;
}

static ssize_t msg_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    size_t to_copy;

    if (buffer_size == 0) {
        /* nothing to read */
        pr_info("msgdriver: read() - buffer empty\n");
        return 0; // EOF-style: return 0 to indicate no more data
    }

    to_copy = min(len, buffer_size);

    if (copy_to_user(buf, kernel_buffer, to_copy)) {
        pr_err("msgdriver: copy_to_user failed\n");
        return -EFAULT;
    }

    /* simple behavior: after read, clear buffer (consumes message) */
    buffer_size = 0;
    memset(kernel_buffer, 0, MAX_BUF_SZ);

    pr_info("msgdriver: read() - returned %zu bytes\n", to_copy);
    return to_copy;
}

static ssize_t msg_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    size_t to_copy = min(len, (size_t)(MAX_BUF_SZ - 1)); // keep space for null if needed

    if (copy_from_user(kernel_buffer, buf, to_copy)) {
        pr_err("msgdriver: copy_from_user failed\n");
        return -EFAULT;
    }

    kernel_buffer[to_copy] = '\0';
    buffer_size = to_copy;

    pr_info("msgdriver: write() - received %zu bytes\n", to_copy);
    return to_copy;
}

static int __init msgdrv_init(void)
{
    int ret;

    /* Allocate major/minor dynamically */
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret) {
        pr_err("msgdriver: failed to alloc chrdev region\n");
        return ret;
    }

    cdev_init(&msg_cdev, &fops);
    msg_cdev.owner = THIS_MODULE;

    ret = cdev_add(&msg_cdev, dev_number, 1);
    if (ret) {
        pr_err("msgdriver: failed to add cdev\n");
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    msg_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(msg_class)) {
        pr_err("msgdriver: failed to create class\n");
        cdev_del(&msg_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(msg_class);
    }

    msg_device = device_create(msg_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(msg_device)) {
        pr_err("msgdriver: failed to create device\n");
        class_destroy(msg_class);
        cdev_del(&msg_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(msg_device);
    }

    pr_info("msgdriver: registered device %s with major:%d minor:%d\n",
            DEVICE_NAME, MAJOR(dev_number), MINOR(dev_number));
    return 0;
}

static void __exit msgdrv_exit(void)
{
    device_destroy(msg_class, dev_number);
    class_destroy(msg_class);
    cdev_del(&msg_cdev);
    unregister_chrdev_region(dev_number, 1);
    pr_info("msgdriver: unloaded\n");
}

module_init(msgdrv_init);
module_exit(msgdrv_exit);
