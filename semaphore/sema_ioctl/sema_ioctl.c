#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

#define IOCTL_MAGIC_NUMBER      'J'

#define IOCTL_CMD_READ      _IOR(IOCTL_MAGIC_NUMBER,1, int)
#define IOCTL_CMD_WRITE     _IOW(IOCTL_MAGIC_NUMBER,2 ,int)


char number[1024];
//struct semaphore sem;
struct rw_semaphore rwsem;

int sysprog_device_open(struct inode* inode, struct file*flip)
{
    //printk(KERN_INFO "HOLDING SEMAPHORE\n");
    printk(KERN_INFO "OPEN FUNCTION CALL\n");
    return 0;
}

int sysprog_device_release(struct inode* inode, struct file*flip)
{
    //printk(KERN_INFO "RELEASING SEMAPHORE\n");
    printk(KERN_INFO "RELEASE FUNCTION CALL\n");
    return 0;
}


long sysprog_ioctl(struct file*flip, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case IOCTL_CMD_READ:
            down_read(&rwsem);
            printk(KERN_INFO "INSIDE READ\n");        
            copy_to_user((void*)arg, &number, 1024);
	    ssleep(10);
            up_read(&rwsem);
            break;

        case IOCTL_CMD_WRITE:
            down_write(&rwsem);
            printk(KERN_INFO "INSIDE WRITE\n");
            copy_from_user(&number, (const void*)arg, 1024);
            up_write(&rwsem);
            break;
    }
    return 0;
}

struct file_operations sys_fops ={
    .open = sysprog_device_open,
    .release = sysprog_device_release,
    .unlocked_ioctl = sysprog_ioctl
};

int __init sysprog_device_init(void)
{
    if(register_chrdev(240, "syprog_device", &sys_fops)<0)
    {
        printk(KERN_INFO "DRIVER INIT FAILED\n");
    }
    else
    {
        printk(KERN_INFO "DRIVER INIT SUCCESS\n");
    }

    //sema_init(&sem, 1);
    init_rwsem(&rwsem);
    return 0;
}

void __exit sysprog_device_exit(void)
{
    unregister_chrdev(240, "sysprog_device");
    printk(KERN_INFO "DRIVER CLEANUP\n");
}

module_init(sysprog_device_init);
module_exit(sysprog_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("LEE");
