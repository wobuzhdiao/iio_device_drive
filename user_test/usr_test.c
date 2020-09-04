#include "usr_test.h"



int main(int argc ,char *argv[])
{
    int fd;
    int ret = 0;
    int eventfd = -1;
    struct iio_event_data event;


    fd = open(DEV_NAME, 0);
    if(fd < 0)
    {
        printf("%s:open [%s] failed [%s]\n", __FUNCTION__, DEV_NAME, strerror(errno));
        return -1;
    }
    printf("%s:%d\n", __FUNCTION__, __LINE__);

    ret = ioctl(fd, IIO_GET_EVENT_FD_IOCTL, &eventfd);
    if (ret == -1 || eventfd == -1) 
    {
        printf("get event fd failed...\n");
        close(fd);
        return -1;
    }
    close(fd);

    while(1)
    {
	ret = read(eventfd, &event, sizeof(struct iio_event_data));
	if(ret >= 0)
	{
		printf("ts=%lld data=%lld\n", event.timestamp, event.id);
	}

    }

    return 0;
}


