/******************************************************************************
  * @file           bt_timer.c
  * @author         Yu-ZhongJun(124756828@qq.com)
  * @Taobao link    https://shop220811498.taobao.com/
  * @version        V0.0.1
  * @date           2020-4-13
  * @brief          bt timer source file
******************************************************************************/

#include "bt_timer.h"
#include <string.h>


utimer_t timer[BT_TIMER_COUNT];

/******************************************************************************
 * func name   : find_idle_timer
 * para        : NULL
 * return      : ����timer handle
 * description : Ѱ�ҿ��е�timer
******************************************************************************/
static int32_t find_idle_timer()
{
    uint8_t index = 0;
    for(index = 0; index < BT_TIMER_COUNT; index++)
    {
        if(timer[index].used == 0)
        {
            return index;
        }
    }
    return -1;
}

/******************************************************************************
 * func name   : utimer_create
 * para        : ticks(IN) -> ����timeout��tick����
 					 cb(IN) -> timeout�ص����� 
 					 para(IN) -> ����timeout�ص������Ĳ���,��Ҫȫ�ֱ���
 * return      : ����timer handle
 * description : ���������ʱ��timer
******************************************************************************/
int32_t  utimer_create(uint32_t ticks, timer_cb cb,void *para)
{
    int32_t idle_timer;

    idle_timer = find_idle_timer();
    if(-1 == idle_timer)
    {
        return -1;
    }

    timer[idle_timer].used = 1;
    timer[idle_timer].cb = cb;
    timer[idle_timer].para = para;
    timer[idle_timer].count = ticks;
    return idle_timer;
}

/******************************************************************************
 * func name   : utimer_cancel
 * para        : timerHandle(IN) -> �����ʱ����handle
 * return      : VOID
 * description : ȡ�������ʱ��timer
******************************************************************************/
void utimer_cancel(int32_t timerHandle)
{
    if(1 == timer[timerHandle].used)
    {
        memset(&timer[timerHandle],0,sizeof(utimer_t));
    }
}

/******************************************************************************
 * func name   : utimer_polling
 * para        : VOID
 * return      : VOID
 * description : ��ѯ�����ʱ���������Ƿ���timeout�Ķ�ʱ��
******************************************************************************/
void utimer_polling()
{
    int index = 0;
    for(index = 0; index < BT_TIMER_COUNT; index++)
    {
        if(1 == timer[index].used)
        {
            timer[index].count -= 1;
            if(0 == timer[index].count)
            {
                timer[index].cb(timer[index].para);
                memset(&timer[index],0,sizeof(utimer_t));
            }
        }
    }
}

/******************************************************************************
 * func name   : utimer_init
 * para        : VOID
 * return      : VOID
 * description : ��ʼ�������ʱ��
******************************************************************************/
void utimer_init()
{
    memset(&timer,0,sizeof(utimer_t)*BT_TIMER_COUNT);
}

