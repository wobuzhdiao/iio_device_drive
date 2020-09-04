#include <linux/sysfs.h>
#include <linux/iio/events.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#include "virt0824_temp_driver.h"

static struct regmap_config virt0824_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
};


static int virt0824_read_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int *val,
			     int *val2,
			     long mask)
{
	int ret;
	struct virt0824_temp *devp = iio_priv(indio_dev);

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		switch (chan->type) {
		case IIO_TEMP:
			/*read i2c dev reg for get temp*/
			ret = regmap_read(devp->regmap, CHANNEL0_CURRENT_TEMP_REG + chan->channel, val);
			if (ret) {
				printk("Error %d reading IO Regmap\n", ret);
				return -ENODEV;
			}
			ret = IIO_VAL_INT;
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_TEMP:
			*val = 200;
			*val2 = 0;
			return IIO_VAL_INT;
		default:
			return -EINVAL;
		}
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

 static int virt0824_write_raw(struct iio_dev *indio_dev,
	struct iio_chan_spec const *chan,int val,int val2,long mask)
{
	int ret = -EINVAL;
	
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return ret;
}

static const struct iio_event_spec virt0824_temp_event[] = {
 {
	 .type = IIO_EV_TYPE_THRESH,
	 .dir = IIO_EV_DIR_RISING,
	 .mask_separate = BIT(IIO_EV_INFO_VALUE) |
		 BIT(IIO_EV_INFO_ENABLE),
 },
 {
	 .type = IIO_EV_TYPE_THRESH,
	 .dir = IIO_EV_DIR_FALLING,
	 .mask_separate = BIT(IIO_EV_INFO_VALUE) |
		 BIT(IIO_EV_INFO_ENABLE),
 },

};

static const struct iio_chan_spec virt0824_channels[] = {
		{
			.type = IIO_TEMP,
			.indexed = 1,
			.channel = 0,
			.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
			.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE), 
			.event_spec =virt0824_temp_event,
			.num_event_specs = ARRAY_SIZE(virt0824_temp_event),
			.scan_index = 0,
			.scan_type = {
				.sign = 's',
				.realbits = 16,
				.storagebits = 16,
				.endianness = IIO_CPU,
			},	
		}, 
		{
			.type = IIO_TEMP,
			.indexed = 1,
			.channel = 1,
			.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) ,
			.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE), 
			.event_spec = virt0824_temp_event,
			.num_event_specs = ARRAY_SIZE(virt0824_temp_event),
			.scan_index = 1,
			.scan_type = {
				.sign = 's',
				.realbits = 16,
				.storagebits = 16,
				.endianness = IIO_CPU,
			},	
		}

};


int virt0824_read_event_config(struct iio_dev *indio_dev,
			 const struct iio_chan_spec *chan,
			 enum iio_event_type type,
			 enum iio_event_direction dir)
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	
	switch (chan->type) {
	case IIO_TEMP:
			return 1;
	default:
		return -EINVAL;
	}
}

int virt0824_write_event_config(struct iio_dev *indio_dev,
			  const struct iio_chan_spec *chan,
			  enum iio_event_type type,
			  enum iio_event_direction dir,
			  int state)
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	return -EINVAL;
}



int virt0824_read_event_value(struct iio_dev *indio_dev,
		  const struct iio_chan_spec *chan,
		  enum iio_event_type type,
		  enum iio_event_direction dir,
		  enum iio_event_info info, int *val, int *val2)
{
	struct virt0824_temp *devp = iio_priv(indio_dev);
	unsigned int base_reg = 0;
	int ret = 0;

	printk("%s:%d\n", __FUNCTION__, __LINE__);

	switch (chan->type) {
	case IIO_TEMP:
		if(type != IIO_EV_TYPE_THRESH)
			return -EINVAL;
		if(dir == IIO_EV_DIR_RISING)
			base_reg = CHANNEL0_HIGH_ALARM_TEMP_REG;
		else if(dir == IIO_EV_DIR_FALLING)
			base_reg = CHANNEL0_LOW_ALARM_TEMP_REG;
		else
			return -EINVAL;
		printk("%s:%d\n", __FUNCTION__, __LINE__);

		ret = regmap_read(devp->regmap, base_reg + chan->channel, val);
		if (ret) {
			printk("Error %d reading IO Regmap\n", ret);
			return -ENODEV;
		}
		return IIO_VAL_INT;
	default:
		return -EINVAL;
	}
}


int virt0824_write_event_value(struct iio_dev *indio_dev,
		   const struct iio_chan_spec *chan,
		   enum iio_event_type type,
		   enum iio_event_direction dir,
		   enum iio_event_info info, int val, int val2)
{
	struct virt0824_temp *devp = iio_priv(indio_dev);
	unsigned int base_reg = 0;
	int ret = 0;

	printk("%s:%d\n", __FUNCTION__, __LINE__);

	switch (chan->type) {
	case IIO_TEMP:
		if(type != IIO_EV_TYPE_THRESH)
			return -EINVAL;
		if(dir == IIO_EV_DIR_RISING)
			base_reg = CHANNEL0_HIGH_ALARM_TEMP_REG;
		else if(dir == IIO_EV_DIR_FALLING)
			base_reg = CHANNEL0_LOW_ALARM_TEMP_REG;
		else
			return -EINVAL;
		printk("%s:%d\n", __FUNCTION__, __LINE__);

		ret = regmap_write(devp->regmap, base_reg + chan->channel, val);
		if (ret) {
			printk("Error %d writing IO Regmap\n", ret);
			return -ENODEV;
		}
		return 0;
	default:
		return -EINVAL;
	}
}




static const struct iio_info virt0824_info = {
	.driver_module = THIS_MODULE,
	.read_raw = &virt0824_read_raw,
	.write_raw = &virt0824_write_raw,
	.read_event_config = virt0824_read_event_config,
	.write_event_config = virt0824_write_event_config,
	.read_event_value = virt0824_read_event_value,
	.write_event_value = virt0824_write_event_value,
};
	


static 	irqreturn_t virt0824_event_handler(int irq, void * data)
{
	struct iio_dev *indio_dev = (struct iio_dev *)data;
	struct virt0824_temp *devp = iio_priv(indio_dev);
	unsigned long flags = 0;
	unsigned int val = 0;
	int ret = 0;
	
	spin_lock_irqsave(&devp->lock, flags);
	ret = regmap_read(devp->regmap, CHANNEL_FAULT_MASK, &val);
	if(ret)
		goto unlock;

	if (val & BIT(CHANNEL0_HIGH_ALARM_TEMP_BIT))
		{
		iio_push_event(indio_dev,
			IIO_UNMOD_EVENT_CODE(IIO_TEMP,
				CHANNEL0_HIGH_ALARM_TEMP_BIT,
				IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING), iio_get_time_ns());
				printk("%s:%d \n", __FUNCTION__, __LINE__);
		}
	
	if (val & BIT(CHANNEL1_HIGH_ALARM_TEMP_BIT))
	{
		iio_push_event(indio_dev,
			IIO_UNMOD_EVENT_CODE(IIO_TEMP,
				CHANNEL1_HIGH_ALARM_TEMP_BIT,
				IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING), iio_get_time_ns());
		printk("%s:%d\n", __FUNCTION__, __LINE__);
	}

	if (val & BIT(CHANNEL0_LOW_ALARM_TEMP_BIT))
	{
		iio_push_event(indio_dev,
			IIO_UNMOD_EVENT_CODE(IIO_TEMP,
				CHANNEL0_HIGH_ALARM_TEMP_BIT,
				IIO_EV_TYPE_THRESH, IIO_EV_DIR_FALLING), iio_get_time_ns());
		printk("%s:%d\n", __FUNCTION__, __LINE__);
	}
	
	if (val & BIT(CHANNEL1_LOW_ALARM_TEMP_BIT))
	{
		iio_push_event(indio_dev,
			IIO_UNMOD_EVENT_CODE(IIO_TEMP,
				CHANNEL1_HIGH_ALARM_TEMP_BIT,
				IIO_EV_TYPE_THRESH, IIO_EV_DIR_FALLING), iio_get_time_ns());
		printk("%s:%d\n", __FUNCTION__, __LINE__);
	}

unlock:
	spin_unlock_irqrestore(&devp->lock, flags);
	return IRQ_HANDLED;
}

static	irqreturn_t virt0824_trigger_handler(int irq, void * data)
{
	struct iio_dev *indio_dev = (struct iio_dev *)data;
	struct virt0824_temp *devp = iio_priv(indio_dev);
	disable_irq_nosync(irq);
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	iio_trigger_poll(devp->trig);
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	return IRQ_HANDLED;
}


static int virt0824_set_trigger_state(struct iio_trigger *trig, bool enable)
{
	struct virt0824_temp *devp = iio_trigger_get_drvdata(trig);

	if (enable)
		enable_irq(devp->temp_info->trigger_irq);
	else
		disable_irq(devp->temp_info->trigger_irq);

	return 0;
}

static int virt0824_iio_trigger_validate(struct iio_trigger *trig,
	struct iio_dev *indio_dev)
{
	if (indio_dev->dev.parent != trig->dev.parent)
		return -EINVAL;
	return 0;
}
static int virt0824_trig_try_reen(struct iio_trigger *trig)
{
	struct virt0824_temp *devp = iio_trigger_get_drvdata(trig);

	enable_irq(devp->temp_info->trigger_irq);
	/* irq reenabled so success! */
	return 0;
}
static irqreturn_t virt0824_trigger_buffer_handler(int irq, void *data)
{
	struct iio_poll_func *pf = data;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct virt0824_temp *devp = iio_priv(indio_dev);
	unsigned long flags;
	int ret = 0;
	int bit = 0;
	int i = 0;
	
	spin_lock_irqsave(&devp->lock, flags);
	printk("%s:%d\n", __FUNCTION__, __LINE__);

	for_each_set_bit(bit, indio_dev->active_scan_mask,
					 indio_dev->masklength)
	{
		ret = regmap_read(devp->regmap, CHANNEL0_CURRENT_TEMP_REG + bit, &devp->data[i++]);
		if(ret)
			goto err;
		
	}

	printk("%d:%d\n", devp->data[0], devp->data[1]);
	iio_push_to_buffers_with_timestamp(indio_dev, devp->data,
		iio_get_time_ns());
	iio_trigger_notify_done(indio_dev->trig);
	printk("%s:%d\n", __FUNCTION__, __LINE__);

err:
	
	spin_unlock_irqrestore(&devp->lock, flags);

	return IRQ_HANDLED;
}


static const struct iio_trigger_ops virt0824_trigger_ops = {
	.set_trigger_state = virt0824_set_trigger_state,
	.validate_device = virt0824_iio_trigger_validate,
	.try_reenable = virt0824_trig_try_reen
};
static const struct iio_buffer_setup_ops virt0824_iio_triggered_buffer_setup_ops = {
	.postenable = &iio_triggered_buffer_postenable,
	.predisable = &iio_triggered_buffer_predisable,
};
	static const unsigned long virt0824_scan_masks[] = {0x3, 0};

static int virt0824_temp_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = -ENODEV;
	struct virt0824_temp *devp;
	struct iio_dev *indio_dev;

	printk("%s:%d\n", __FUNCTION__, __LINE__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
	{
		printk("%s:%d\n", __FUNCTION__, __LINE__);
		return -EIO;
	}
	

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*devp));
	if (!indio_dev)
	{
		printk("%s:%d \n", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}
	devp = iio_priv(indio_dev);
	spin_lock_init(&devp->lock);
	devp->temp_info = client->dev.platform_data;
	if(devp->temp_info == NULL)
		return -EINVAL;

	printk("event irq=%d trigger irq=%d\n", devp->temp_info->event_irq, devp->temp_info->trigger_irq);
	devp->regmap = devm_regmap_init_i2c(client, &virt0824_regmap_config);
	if (IS_ERR(devp->regmap)) {
			ret = PTR_ERR(devp->regmap);
			printk("%s:%d ret=0x%x\n", __FUNCTION__, __LINE__, ret);
			return ret;
	}
	printk("regmap->reg_stride=%d\n", regmap_get_reg_stride(devp->regmap));
	indio_dev->dev.parent = &client->dev;
	indio_dev->info = &virt0824_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = virt0824_channels;
	indio_dev->num_channels = ARRAY_SIZE(virt0824_channels);
	indio_dev->available_scan_masks = virt0824_scan_masks;
	indio_dev->name = client->dev.driver->name;

	ret = devm_request_threaded_irq(&client->dev, devp->temp_info->event_irq, NULL, virt0824_event_handler,
		IRQF_ONESHOT, client->name, indio_dev);
	if(ret)
		return ret;
	
	devp->trig = devm_iio_trigger_alloc(&client->dev, "%s-dev%d",
		indio_dev->name, indio_dev->id);
	if (!devp->trig)
		return -ENOMEM;
	
	devp->trig->ops = &virt0824_trigger_ops;
	devp->trig->dev.parent = &client->dev;
	iio_trigger_set_drvdata(devp->trig, devp);

	ret = devm_request_threaded_irq(&client->dev, devp->temp_info->trigger_irq, NULL, virt0824_trigger_handler,
		IRQF_ONESHOT, client->name, indio_dev);
	if(ret)
		return ret;
	disable_irq(devp->temp_info->trigger_irq);
	
	ret = iio_trigger_register(devp->trig);
	if (ret)
		return ret;

	indio_dev->trig = iio_trigger_get(devp->trig);
	ret = iio_triggered_buffer_setup(indio_dev, 
		&iio_pollfunc_store_time, &virt0824_trigger_buffer_handler, &virt0824_iio_triggered_buffer_setup_ops);
	if(ret)
	{

		goto free_trig;
	}

	ret = devm_iio_device_register(&client->dev, indio_dev);
	if (ret)
		goto free_trig_buff;

	i2c_set_clientdata(client, devp);

	printk("%s:%d\n", __FUNCTION__, __LINE__);

	return ret;
free_trig_buff:
	iio_triggered_buffer_cleanup(indio_dev);

free_trig:
	iio_trigger_put(devp->trig);
	iio_trigger_unregister(devp->trig);
	
	return ret;
}

static int virt0824_temp_remove(struct i2c_client *client)
{
	struct virt0824_temp *devp = i2c_get_clientdata(client);
	struct iio_dev * indio_dev = iio_priv_to_dev(devp);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	
	iio_triggered_buffer_cleanup(indio_dev);
	
	iio_trigger_put(devp->trig);
	iio_trigger_unregister(devp->trig);
		

	i2c_set_clientdata(client, NULL);
	return 0;
}

static const struct of_device_id virtual_regulator_of_match[] = {
	{
		.compatible = "virtual,virt0824_temp",
	},
	{},
};

MODULE_DEVICE_TABLE(of, virtual_regulator_of_match);

static const struct i2c_device_id virt0824_temp_dev_id[] = {
	{"virt0824_temp", 0},
	{},
};

static struct i2c_driver virt0824_temp_dev_driver = {
	.driver = {
		.name	= "virt0824_temp_driver",
		.of_match_table = of_match_ptr(virtual_regulator_of_match),
	},
	.probe      = virt0824_temp_probe,
	.remove     = virt0824_temp_remove,
	.id_table   = virt0824_temp_dev_id,
};

module_i2c_driver(virt0824_temp_dev_driver);

MODULE_DESCRIPTION("Virtual Temp Device Drivers"); 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jerry_chg");
