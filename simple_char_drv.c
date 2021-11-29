#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/ktime.h>

u64 get_jiffies_64(void);
struct _vchar_drv
{
	unsigned int open_cnt;
	unsigned long start_time;
	unsigned long time_read;
}vchar_drv;
//đăng ký entry point open
int ex01_open(struct inode *pinode, struct file *pfile)
{
	vchar_drv.start_time= jiffies;
	vchar_drv.open_cnt++;
	printk(KERN_INFO "Inside the %s function (%d)\n",__FUNCTION__,vchar_drv.open_cnt);
	return 0;
}

//đăng ký entry point read
ssize_t ex01_read(struct file *pfile, char __user *buffer,size_t length,loff_t *offset)
{
	printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
	return 0;	
}

//đăng ký entry point write
ssize_t ex01_write(struct file *pfile, const char __user *buffer,size_t length,loff_t *offset)
{

	printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
	return length;	
}

//đăng ký entry point release
int ex01_close(struct inode *pinode, struct file *pfile)
{
	
	struct timespec64 ts;
	jiffies_to_timespec64(jiffies-vchar_drv.start_time, &ts);
	printk(KERN_INFO "The driver is used in %lld.%ld seconds\n",ts.tv_sec,ts.tv_nsec/1000000);
	printk(KERN_INFO"Inside the %s function\n",__FUNCTION__);
	return 0;
}

// khai báo các hàm
struct file_operations ex01_file_operations=
{
	.owner =THIS_MODULE,
	.open  =ex01_open,
	.read  =ex01_read,
	.write =ex01_write,
	.release=ex01_close,
};

int ex01_simple_module_init(void)
{
	printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__);
	// đăng ký character device driver với kernel với major number là 236 
	register_chrdev(236,"Simple Char Drv",&ex01_file_operations);
	return 0;	
}


void ex01_simple_module_exit(void)
{
	printk(KERN_ALERT "Inside the %s function\n",__FUNCTION__);
	// hủy đăng ký character device driver
	unregister_chrdev(236,"Simple Char Drv");	
}

module_init(ex01_simple_module_init);
module_exit(ex01_simple_module_exit);
MODULE_LICENSE("GPL"); 





























