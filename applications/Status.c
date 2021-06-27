/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-27     Rick       the first version
 */
#include <rtthread.h>
#include "pin_config.h"
#include "key.h"
#include "led.h"
#include "status.h"
#include "flashwork.h"
#include "radio_encoder.h"
#include "Moto.h"
#include "work.h"
#include "string.h"
#include "rthw.h"

#define DBG_TAG "status"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern enum Device_Status Now_Status;
extern uint8_t ValveStatus;
extern uint16_t Radio_Counter;

WariningEvent NowStatusEvent;
WariningEvent SlaverLowPowerEvent;
WariningEvent SlaverUltraLowPowerEvent;
WariningEvent SlaverWaterAlarmActiveEvent;
WariningEvent MasterLostPeakEvent;
WariningEvent MasterWaterAlarmActiveEvent;
WariningEvent OfflineEvent;
WariningEvent MotoFailEvent;

void WarningEventInit(uint8_t warning_id,uint8_t priority,WariningEvent *event,void (*callback)(void*))
{
    memset(event,0,sizeof(&event));
    event->warning_id = warning_id;
    event->last_id = 0;
    event->priority = priority;
    event->callback = callback;
}
void Warning_Enable(WariningEvent event)
{
    if(event.priority >= NowStatusEvent.priority)
    {
        NowStatusEvent.last_id = event.warning_id;
        NowStatusEvent.priority = event.priority;
        if(event.callback!=RT_NULL)
        {
            NowStatusEvent.callback = event.callback;
            NowStatusEvent.callback(RT_NULL);
        }
        LOG_D("Warning_Enable Success,warning id is %d , now priority is %d\r\n",event.warning_id,event.priority);
    }
    else
    {
        LOG_D("Warning_Enable Fail last is %d Now is %d\r\n",NowStatusEvent.priority,event.priority);
    }
}
void Warning_Enable_Num(uint8_t id)
{
    switch(id)
    {
    case 1:Warning_Enable(SlaverUltraLowPowerEvent);break;
    case 2:Warning_Enable(SlaverWaterAlarmActiveEvent);break;
    case 3:Warning_Enable(MasterLostPeakEvent);break;
    case 4:Warning_Enable(MasterWaterAlarmActiveEvent);break;
    case 5:Warning_Enable(OfflineEvent);break;
    case 6:Warning_Enable(MotoFailEvent);break;
    case 7:Warning_Enable(SlaverLowPowerEvent);break;
    }
}
void Warning_Disable(void)
{
    NowStatusEvent.last_id = 0;
    NowStatusEvent.priority = 0;
    BackToNormal();
    LOG_D("Warning is Disable\r\n");
}
void SlaverLowBatteryWarning(void *parameter)
{
    beep_start(0,15);//红灯,蜂鸣器一下
    //Now_Status = SlaverLowPower;
    LOG_D("SlaverLowBatteryWarning\r\n");
}
void SlaverUltraLowBatteryWarning(void *parameter)
{
    Moto_Close(NormalOff);
    beep_start(0,15);//红灯,蜂鸣器一下
    Now_Status = SlaverUltraLowPower;
    LOG_D("SlaverUltraLowBatteryWarning\r\n");
}
void SlaverWaterAlarmWarning(void *parameter)
{
    Moto_Close(OtherOff);
    beep_start(0,2);//红灯,蜂鸣器三下
    Now_Status = SlaverWaterAlarmActive;
    LOG_D("SlaverWaterAlarmWarning\r\n");
}
void MasterLostPeakWarning(void *parameter)
{
    beep_start(0,1);//红灯,蜂鸣器三下
    loss_led_start();
    LOG_D("MasterLostPeakWarning\r\n");
}
void MasterStatusChangeToDeAvtive(void)
{
    Now_Status = MasterWaterAlarmDeActive;
    LOG_D("MasterStatusChangeToDeAvtive\r\n");
}
void MasterWaterAlarmWarning(void *parameter)
{
    Moto_Close(NormalOff);
    beep_start(0,2);//红灯,蜂鸣器三下
    Now_Status = MasterWaterAlarmActive;
    LOG_D("MasterWaterAlarmWarning\r\n");
}
void MasterAlarmWaterDisable(void)
{
}
rt_timer_t Delay_Timer = RT_NULL;
void Delay_Timer_Callback(void *parameter)
{
    LOG_D("Delay_Timer_Callback is Now\r\n");
    Moto_Close(NormalOff);
}
void Delay_Timer_Init(void)
{
    LOG_D("Delay_Timer_Init Success\r\n");
    Delay_Timer = rt_timer_create("Delay_Timer", Delay_Timer_Callback, RT_NULL, 10*1000,RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_ONE_SHOT);
}
MSH_CMD_EXPORT(Delay_Timer_Init,Delay_Timer_Init);
void Delay_Timer_Open(void)
{
    LOG_D("Delay_Timer is Open\r\n");
    rt_timer_start(Delay_Timer);
}
MSH_CMD_EXPORT(Delay_Timer_Open,Delay_Timer_Open);
void Delay_Timer_Close(void)
{
    LOG_D("Delay_Timer is Close\r\n");
    rt_timer_stop(Delay_Timer);
}
void OfflineWarning(void *parameter)
{
    if(Now_Status!=Offline)
    {
        Moto_Close(NormalOff);
        Now_Status = Offline;
        LOG_D("OfflineWarning\r\n");
        beep_start(0,5);
    }
    else
    {
        LOG_D("Already OfflineWarning Now\r\n");
    }
}
void OfflineDisableWarning(void)
{
    if(Now_Status == Offline)
    {
        Warning_Disable();
        LOG_D("Disable OfflineWarning\r\n");
    }
    else
    {
        LOG_D("Not OfflineWarning Now\r\n");
    }
}
void RadioInitFail(void)
{
    rt_hw_cpu_reset();
}
void WarningInit(void)
{
    WarningEventInit(7,2,&SlaverLowPowerEvent,SlaverLowBatteryWarning);
    WarningEventInit(2,3,&SlaverWaterAlarmActiveEvent,SlaverWaterAlarmWarning);
    WarningEventInit(3,0,&MasterLostPeakEvent,MasterLostPeakWarning);
    WarningEventInit(4,3,&MasterWaterAlarmActiveEvent,MasterWaterAlarmWarning);
    WarningEventInit(5,4,&OfflineEvent,OfflineWarning);
    WarningEventInit(6,0,&MotoFailEvent,RT_NULL);
    WarningEventInit(1,2,&SlaverUltraLowPowerEvent,SlaverUltraLowBatteryWarning);
    WarningEventInit(0,0,&NowStatusEvent,RT_NULL);//本地存储器
    LOG_D("Warning Event Init Success\r\n");
}
MSH_CMD_EXPORT(WarningInit,WarningInit);
uint8_t Detect_Learn(void)
{
    if(Now_Status!=Learn)
    {
        return 1;
    }
    else return 0;
}
void BackToNormal(void)
{
    WaterScan_Clear();
    beep_stop();
    led_Stop(0);
    if(ValveStatus)
    {
        Now_Status = Open;
    }
    else
    {
        Now_Status = Close;
    }
}

