# virt_iio_device_driver

#### 介绍
实现一个虚拟的iio device driver，该iio device提供两路温度检测，支持读取当前的温度值，并且支持温度阈值告警中断，且支持连续数据采集功能，当温度数据可读时，可产生可读中断。

#### 软件架构
软件架构说明
主要借助linux iio子系统提供的接口，实现iio device driver

#### 安装教程

1.  本驱动已经在ubuntu16.04上测试验证，由于ubuntu16.04内核默认没有将iio 子系统 编译进内核，但是已编译成驱动模块，可以在路径/lib/modules/4.4.0-66-generic/kernel/drivers/iio下找到对应的驱动模块，我们需要这些驱动模块：
industrialio.ko、industrialio-triggered-event.ko、kfifo_buf.ko、industrialio-triggered-buffer.ko即可
2.  本次模拟主要涉及三个虚拟设备
   a.虚拟中断控制器，用于温度告警中断触发、温度数据可读中断触发；
   b.虚拟iic device设备，用于抽象温度传感器；
   c.虚拟iio device设备，该设备依附于iic device
#### 使用说明

