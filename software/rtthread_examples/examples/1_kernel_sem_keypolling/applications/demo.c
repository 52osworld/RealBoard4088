/*
  ��demo������ʾ�ź�����ʹ��
 */
#include <rtthread.h>
#ifdef RT_USING_FINSH
#include <finsh.h>
#include <shell.h>
#endif

#include <board.h>

#define KEY_PIN  ((1<<19)|(1<<26)|(1<<10))

static void key_gpio_config(void)
{
  	LPC_IOCON->P4_19 &= ~0x07;				//����P4_19ΪGPIOģʽ
	LPC_IOCON->P4_26 &= ~0x07;			    //����P4_13ΪGPIOģʽ
  	LPC_IOCON->P2_10 &= ~0x07;				//����P2_10ΪGPIOģʽ
  	LPC_GPIO4->DIR&=~(1<<19);				//����P4_19��GPIO����Ϊ����
  	LPC_GPIO4->DIR&=~(1<<26);				//����P4_13��GPIO����Ϊ����
  	LPC_GPIO2->DIR&=~(1<<10);				//����P2_10��GPIO����Ϊ����  
}

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
struct rt_thread thread1;

static int key;
static struct rt_semaphore sem;

static void rt_thread_entry1(void* parameter)
{
    int temp;

	key_gpio_config();
    
    while (1)
    {
        key = ~((LPC_GPIO4->PIN&(1<<19)) | (LPC_GPIO4->PIN&(1<<26)) | (LPC_GPIO2->PIN&(1<<10)))&((1<<19)|(1<<26)|(1<<10));

        if (key & ((1<<19)|(1<<26)|(1<<10)))
        {
            temp = key;
            rt_thread_delay(RT_TICK_PER_SECOND / 50);
            key = ~((LPC_GPIO4->PIN&(1<<19)) | (LPC_GPIO4->PIN&(1<<26)) | (LPC_GPIO2->PIN&(1<<10)))&((1<<19)|(1<<26)|(1<<10));
            if (key == temp)
                rt_sem_release(&sem);
        }
        rt_thread_delay(RT_TICK_PER_SECOND/10);	
   }
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
struct rt_thread thread2;

static int key;
static void rt_thread_entry2(void* parameter)
{
    while (1)
    {
        rt_sem_take(&sem, RT_WAITING_FOREVER);

        if (key & KEY_PIN)
        {
            rt_kprintf("some keys has been pressed : %x\n", key);
        }
    }
}

int demo_init(void)
{
    rt_err_t result;
    
    result = rt_sem_init(&sem, "sem", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("error, init sem failed!\n");
        return 0;
    }
    
    rt_thread_init(&thread1,
                   "keyp", //producer
                   rt_thread_entry1,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),11,25);
    rt_thread_startup(&thread1);
    
    rt_thread_init(&thread2,
                   "keyc", //consumer
                   rt_thread_entry2,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),11,24);
    rt_thread_startup(&thread2);
    return 0;

}
