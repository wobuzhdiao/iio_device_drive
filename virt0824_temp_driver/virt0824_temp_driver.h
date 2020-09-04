#ifndef VIRT0824_TEMP_DRIVER_H_
#define VIRT0824_TEMP_DRIVER_H_
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mdio.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/kdev_t.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/regmap.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/triggered_buffer.h>

#include "../virt0824_temp_data.h"
#define VRIT0824_TEMP_CHANNEL_NUM 2
typedef enum 
{
	CHANNEL0_CURRENT_TEMP_REG,
	CHANNEL1_CURRENT_TEMP_REG,
	CHANNEL0_HIGH_ALARM_TEMP_REG,
	CHANNEL1_HIGH_ALARM_TEMP_REG,
	CHANNEL0_LOW_ALARM_TEMP_REG,
	CHANNEL1_LOW_ALARM_TEMP_REG,
	CHANNEL_FAULT_MASK//bit0 for channel0 temp high alarm, bit1 
}virt0824_regs;

typedef enum
{
	CHANNEL0_HIGH_ALARM_TEMP_BIT,
	CHANNEL1_HIGH_ALARM_TEMP_BIT,
	CHANNEL0_LOW_ALARM_TEMP_BIT,
	CHANNEL1_LOW_ALARM_TEMP_BIT,
}virt0824_fault_bitops;

struct virt0824_temp
{	
	char name[32];
	struct regmap *regmap;
	struct virt0824_temp_info *temp_info;
	struct iio_trigger *trig;
	wait_queue_head_t	event_ready_queue;
	spinlock_t lock;
	unsigned short	data[2] ____cacheline_aligned;
};
#endif
