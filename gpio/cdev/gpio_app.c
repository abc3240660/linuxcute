#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "gpio-names.h"
#include <stdbool.h>
#include <linux/types.h>
#include <fcntl.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define GPIO_IOC_REQ _IO('R',0)
#define GPIO_IOC_SET _IO('S',0)
#define GPIO_IOC_GET _IO('G',0)
#define GPIO_IOC_FRE _IO('F',0)


int fd_gpio = -1;
int main(int argc,char *argv[])
{
	int  ret;
    int  gpio=0;
    int  value=0;
    char ch;
    int dir=0;
    unsigned int arg=0;
	fd_gpio = open("/dev/gen_gpio", O_RDWR);
	if(fd_gpio < 0) {
		printf("unable to open device node /dev/gen_gpio\n");
		return -1;
	}
    //test app core board gpio83/66 as test LED
    while(1)
    {
        ch = getchar();
        switch(ch)
        {
        case 'r'://request gpio level
            printf("Please input the gpio num:");
            scanf("%d",&gpio);
            printf("Please input the dir(0:output 1:input):");
            scanf("%d",&dir);

            arg = (dir?0x80000000:0x00000000)|gpio;
           	ret = ioctl(fd_gpio,GPIO_IOC_REQ,arg);
            printf("request gpio %d as %s !\n",gpio,dir?"input":"output");
            break;
        case 'g'://get gpio level
            printf("Please input the gpio num:");
            scanf("%d",&gpio);
           	value = ioctl(fd_gpio,GPIO_IOC_GET,gpio);
            printf("gpio %d value is %d !\n",gpio,value);           
            break;
        case 's'://set gpio level
            printf("Please input the gpio num:");
            scanf("%d",&gpio);
            printf("Please input the gpio value(0/1):");
            scanf("%d",&value);
           	ret = ioctl(fd_gpio,GPIO_IOC_SET,(value<<8)|gpio);
            if(ret == 0)
                printf("Set gpio %d to %d success!\n",gpio,value);
            else
                printf("Set gpio %d to %d failed!\n",gpio,value);
            break;
        case 'f'://free gpio level
            printf("Please input the gpio num:");
            scanf("%d",&gpio);
           	ret = ioctl(fd_gpio,GPIO_IOC_FRE,gpio);
            break;
        case 'q':
            goto exit;
        default:
            break;
        }
    }

exit:
    close(fd_gpio);
    return 0;
}

