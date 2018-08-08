#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <asm/atomic.h>

#define GPIO_IOC_REQ _IO('R',0)
#define GPIO_IOC_SET _IO('S',0)
#define GPIO_IOC_GET _IO('G',0)
#define GPIO_IOC_FRE _IO('F',0)

#define GPIO_O_VERSION ("V0.02")
#define DEV_NAME       "gen_gpio"
#define CMD_LENGTH     (32)
#define GPIO_MAX_NUM   (256)

#define TEST_BANKS 8
#define TEST_PORTS_PER_BANK 4
#define TEST_GPIOS_PER_PORT 8
#define TEST_TOTAL_GPIOS (TEST_BANKS * TEST_PORTS_PER_BANK * TEST_GPIOS_PER_PORT)

static struct class *gpio_cls;
struct cdev gpio_cdev;
static int major, minor;

atomic_t using_cnt[TEST_TOTAL_GPIOS];

static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret   = 0;
	int dir   = 0;
	int gpio  = 0;
	int value = 0;
	char label[16] = "";

	gpio  = (arg & 0x000000FF);
	value = (arg & 0x0000FF00) >> 8;
	dir   = (arg & 0x80000000) ? GPIOF_DIR_IN : GPIOF_DIR_OUT;
	sprintf(label, "gpio%d", gpio);

	if ((gpio < 0) || (gpio >= GPIO_MAX_NUM)) {
		printk(KERN_ERR "Gpio index should be [%d, %d]\n", 0, (GPIO_MAX_NUM - 1));
		return -EINVAL;
	}

	if ((value < 0) || (value > 1)) {
		printk(KERN_ERR "Gpio value should be [%d, %d]\n", 0, 1);
		return -EINVAL;
	}

	switch (cmd)
	{
	case GPIO_IOC_REQ://request a gpio
		if (0 == atomic_read(&using_cnt[gpio])) {
			sprintf(label,"gpio%d",gpio);
			ret = gpio_request(gpio, label);
			if (ret < 0) {
				printk(KERN_ERR "Unable to call gpio_request(ERR:%d) for gpio %d\n", ret, gpio);
				return ret;
			}
		} else {
			printk(KERN_WARNING "The GPIO%d is already requested before\n", gpio);
		}

		atomic_inc(&using_cnt[gpio]);
		if (GPIOF_DIR_IN == dir) {
			ret = gpio_direction_input(gpio);
			if (ret < 0) {
				printk(KERN_ERR "Unable to call gpio_direction_input(ERR:%d) for gpio %d\n", ret, gpio);
				return ret;
			}
		}
		break;

	case GPIO_IOC_SET:
		printk(KERN_ERR "set gpio %d to %d \n", gpio,value);
		ret = gpio_direction_output(gpio, value);
		if (ret < 0) {
			printk(KERN_ERR "Unable to call gpio_direction_output(ERR:%d) for gpio %d\n", ret, gpio);
			return ret;
		}
		break;

	case GPIO_IOC_GET:
		ret = gpio_get_value(gpio);
		if (ret < 0) {
			printk(KERN_ERR "Unable to call gpio_get_value(ERR:%d) for gpio %d\n", ret, gpio);
			return ret;
		}
		break;

	case GPIO_IOC_FRE:
		if (atomic_read(&using_cnt[gpio]) > 0) {
			atomic_dec(&using_cnt[gpio]);
			if (0 == atomic_read(&using_cnt[gpio])) {
				gpio_free(gpio);
			} else {
				printk(KERN_WARNING "Ignore free command whent other is using GPIO%d\n", gpio);
			}
		} else {
			printk(KERN_WARNING "No need to free un-requested GPIO%d\n", gpio);
		}
		break;

	default:
		ret = -ENOSYS;
		break;
	}

	return ret;
}

static struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = gpio_ioctl,
};

static int __init gpio_init(void)
{
	int i = 0;
	int ret = 0;
	dev_t dev_id = 0;
	struct device *gpio_dev = NULL;

	ret = alloc_chrdev_region(&dev_id, 0, 1, DEV_NAME);
	if (ret < 0) {
		printk(KERN_ERR "Unable to allocate device number\n");
		return ret;
	}

	major = MAJOR(dev_id);
	minor = MINOR(dev_id);

	cdev_init(&gpio_cdev, &gpio_fops);
	gpio_fops.owner = THIS_MODULE;
	ret = cdev_add(&gpio_cdev, dev_id, 1);
	if (ret < 0) {
		printk(KERN_ERR "Unable to register driver gpio\n");
		unregister_chrdev_region(MKDEV(major, minor), 1);
		return ret;
	}

	gpio_cls = class_create(THIS_MODULE, DEV_NAME);
	if (NULL == gpio_cls) {
		printk(KERN_ERR "Unable to create device class\n");
		cdev_del(&gpio_cdev);
		unregister_chrdev_region(MKDEV(major, minor), 1);
		return -ENODEV;
	}

	gpio_dev = device_create(gpio_cls, NULL, dev_id, NULL, DEV_NAME);
	if (NULL == gpio_dev) {
		printk(KERN_ERR "Unable to create device node\n");
		class_destroy(gpio_cls);
		cdev_del(&gpio_cdev);
		unregister_chrdev_region(MKDEV(major, minor), 1);
		return -ENODEV;
	}

	for (i = 0; i < TEST_TOTAL_GPIOS; i++) {
		atomic_set(&using_cnt[i], 0);
	}

	printk(KERN_INFO "driver gpio %s installed\n", GPIO_O_VERSION);

	return 0;
}

static void __exit gpio_exit(void)
{
	int i = 0;

	for (i = 0; i < TEST_TOTAL_GPIOS; i++) {
		if (atomic_read(&using_cnt[i]) != 0) {
			gpio_free(i);
			atomic_set(&using_cnt[i], 0);
		}
	}

	device_destroy(gpio_cls, MKDEV(major, minor));
	class_destroy(gpio_cls);
	cdev_del(&gpio_cdev);
	unregister_chrdev_region(MKDEV(major, minor), 1);

	printk(KERN_INFO "driver gpio %s uninstalled\n", GPIO_O_VERSION);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("XXX@XXX");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("Test generic IO output driver");
