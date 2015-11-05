/*
   此demo用于演示网络部分
 */
#include <board.h>
#include <rtthread.h>
#ifdef RT_USING_FINSH
#include <finsh.h>
#include <shell.h>
#endif


#include "components.h"

#include "spi_wifi_rw009.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define MAXBUF 1024
#define PORT 35352
char ServerIP[]="192.168.1.101";
#define BACKLOG 1
void StartServer()
{
		char buffer[MAXBUF];
    int receivedBytes = -2;
    socklen_t clientaddrLen;
    int listenfd, connectfd;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
 int opt = 1;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenfd)
    {
        rt_kprintf("Create listen socket failed");
    }

   
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&serveraddr,0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)))
    {
        rt_kprintf("Bind error");
    }

    if (-1 == listen(listenfd, BACKLOG))
    {
        rt_kprintf("listen error");
    }

    clientaddrLen = sizeof(clientaddr);
    rt_kprintf("Server started. \n");
    while(1)
    {
        if (-1 == (connectfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrLen)))
        {
            rt_kprintf("accept error");
        }
        rt_kprintf("You got a connection from client,IP is %s, PORT is %d\n",
                 inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        /*recieve data*/
        while(1)
        {
            receivedBytes = recv(connectfd, buffer, MAXBUF, 0);
            if(-1 == receivedBytes)
            {
                rt_kprintf("receive data error");
                break;
            }
            else if (0 == receivedBytes)
            {
                /*printf("network disconnected accidentally.\n");*/
                break;
            }

            buffer[receivedBytes] = '\0';
        rt_kprintf("received data: %s\n",buffer);
         //reverse(buffer, receivedBytes);
            /*send data*/
            send(connectfd, buffer, strlen(buffer), 0);
        }
        /*printf("close.\n");*/
        closesocket(connectfd);
    }

    closesocket(listenfd);
  
}

extern void lpc_emac_hw_init(void);

void rt_init_thread_entry(void *parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

#ifdef RT_USING_LWIP
    /* initialize eth interface */
    rt_hw_wifi_init("spi01",MODE_STATION);

		rt_kprintf("wifi Init Compl..\n");
    rw009_join("Go", "xiaomi12");
		rt_kprintf("Join wifi  Compl..\n");
#endif
		StartServer();
	
}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
                           rt_init_thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX / 3, 20); //

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
