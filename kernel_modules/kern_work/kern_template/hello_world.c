#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/poll.h>
#include <linux/wait.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#define IOCTL_SET_AGE	1001
#define IOCTL_GET_AGE	1002
#define SUCCESS 0
#define ERROR -1
#define MSG_BUFFER_LEN 15

static int device_major = -1;
static const char *device_name = "hello-dev";

static int proc_show(struct seq_file *m, void *v);
static int proc_open(struct inode *inode, struct  file *file);
static ssize_t proc_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset);

static int dev_open(struct inode *inode, struct file *file);
static ssize_t dev_read(struct file *file, char __user *buffer, size_t length, loff_t *offset);
static ssize_t dev_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset);
static long dev_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static unsigned int dev_poll(struct file *file, poll_table *wait);
static int dev_release(struct inode *inode, struct file *file);

/*
	My Global Variables
*/
static int Age = 0;
static int device_open_count = 0;

static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr = NULL;

static DECLARE_WAIT_QUEUE_HEAD(fortune_wait);

static int new_data_is_ready = 0;

static const struct file_operations device_ops = {
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl,
	.poll = dev_poll,
	.release = dev_release,
};

static const struct file_operations proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_open,
	.write      = proc_write,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Hello proc! \n");
	return 0;
}

static int proc_open(struct inode *inode, struct  file *file)
{
	return single_open(file, proc_show, NULL);
}

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;
	printk(KERN_INFO "%s %d \n",buffer,(int)length);

	if(msg_ptr == NULL || *msg_ptr == 0){
		msg_ptr = msg_buffer;
	}
	//printk(KERN_INFO "%s\n",buffer);
	while(length && buffer[bytes_read]){
		copy_from_user(msg_ptr, (char *)(&(buffer[bytes_read])), sizeof(char));
		length--;
		bytes_read++;
	}
	printk(KERN_INFO "%d\n",bytes_read);
	new_data_is_ready++;
	wake_up_interruptible(&fortune_wait);
	printk(KERN_INFO "completed \n");	
	return bytes_read;
}

static int dev_open(struct inode *inode, struct file *file)
{
	if (device_open_count)
		return ERROR;
	
	device_open_count++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static ssize_t dev_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;

	if (msg_ptr == NULL || *msg_ptr == 0) {
		msg_ptr = msg_buffer;
	}

	while (length && *msg_ptr) {

		copy_to_user((msg_ptr++), buffer++,sizeof(char));
		length--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t dev_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset)
{
	int bytes_read = 0;
	if(msg_ptr == NULL && *msg_ptr == 0){
		msg_ptr = msg_buffer;
	}
	while(length && *msg_ptr){
		copy_from_user(msg_ptr, (char *)(&(buffer[bytes_read])), sizeof(char));
		length--;
		msg_ptr++;
		bytes_read++;
	}
	new_data_is_ready++;
	wake_up_interruptible(&fortune_wait);
	return bytes_read;
}

static long dev_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	switch(ioctl_num){
		case IOCTL_GET_AGE:
			copy_to_user((int *)ioctl_param,&Age,sizeof(int));
		break;
		case IOCTL_SET_AGE:
			copy_from_user(&Age, (int *)ioctl_param, sizeof(int));
		break;
		default:
			printk(KERN_INFO "dev_ioctl | Error \n");
	}
	return Age;
}

static unsigned int dev_poll(struct file *file, poll_table *wait)
{
	wait_event_interruptible(fortune_wait,new_data_is_ready > 0);
	new_data_is_ready--;
    return 0;
}


static int dev_release(struct inode *inode, struct file *file)
{
	device_open_count--;
 	module_put(THIS_MODULE);
 	return 0;
}

static int __init main_init(void)
{
	printk(KERN_INFO "Loading module ...\n");

	device_major = register_chrdev(0, device_name, &device_ops);
	proc_create("hello_proc", 0, NULL, &proc_fops);
	return 0;
}

static void __exit main_exit(void)
{
	printk(KERN_INFO "Exitting module hello \n");
	unregister_chrdev(device_major, device_name);
	remove_proc_entry("hello_proc", NULL);
}

module_init(main_init);
module_exit(main_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Emre BAYRAM");	/* Who wrote this module? */
MODULE_DESCRIPTION("Hello world module!");	/* What does this module do */
