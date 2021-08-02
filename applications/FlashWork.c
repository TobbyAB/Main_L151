#include <rtthread.h>
#include <rtdevice.h>
#include <easyflash.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <spi_flash.h>
#include <drv_spi.h>
#include "rthw.h"
#include "pin_config.h"
#include "FlashWork.h"
#include "status.h"
#include "led.h"
#include "gateway.h"

#define DBG_TAG "flash"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

typedef struct _env_list {
    char *key;
} env_list;

Device_Info Global_Device={0};

rt_spi_flash_device_t fm25q128;

char read_value_temp[64] = {0};

int flash_Init(void)
{
    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PIN_12);//往总线spi2上挂载一个spi20设备，cs脚：PB12
    fm25q128 = rt_sfud_flash_probe("norflash0", "spi20");//使用sfud探测spi20从设备，并且将spi20连接的flash初始化为块设备，名称为FAL_USING_NOR_FLASH_DEV_NAME
    if (RT_NULL == fm25q128)
    {
        return -RT_ERROR;
    };
    return RT_EOK;
}
uint32_t Flash_Get_Boot_Times(void)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf="boot_times";
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
MSH_CMD_EXPORT(Flash_Get_Boot_Times,Flash_Get_Boot_Times);
uint32_t Flash_Get_Learn_Nums(void)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf="Learn_Nums";
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
uint32_t Flash_Get_Door_Nums(void)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf="88888888";
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
uint32_t Flash_Get_Gateway_Nums(void)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf="88887777";
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
uint32_t Flash_Get_Moto_Flag(void)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf="Moto";
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
uint32_t Flash_Get_Key_Value(uint32_t key)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "%ld", key);//将传入的数字转换成数组
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    rt_free(keybuf);//释放临时buffer对应内存空间
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
void Flash_Key_Change(uint32_t key,uint32_t value)
{
    char *Temp_KeyBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "%ld", key);
    char *Temp_ValueBuf = rt_malloc(20);//申请临时buffer空间
    sprintf(Temp_ValueBuf, "%ld", value);
    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
    rt_free(Temp_KeyBuf);
    rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %ld \r\n", value,key);
}
void Flash_LearnNums_Change(uint32_t value)
{
    const char *keybuf="Learn_Nums";
    char *Temp_ValueBuf = rt_malloc(10);
    sprintf(Temp_ValueBuf, "%ld", value);
    ef_set_env(keybuf, Temp_ValueBuf);
    rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %s\r\n", value,keybuf);
}
void Flash_Moto_Change(uint8_t value)
{
    const char *keybuf="Moto";
    char *Temp_ValueBuf = rt_malloc(10);
    sprintf(Temp_ValueBuf, "%d", value);
    ef_set_env(keybuf, Temp_ValueBuf);
    rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %s\r\n", value,keybuf);
}
uint8_t Device_RssiGet(uint32_t Device_ID)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "Rssi:%ld", Device_ID);//将传入的数字转换成数组
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    rt_free(keybuf);//释放临时buffer对应内存空间
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
void Device_RssiChange(uint32_t Device_ID,uint8_t value)
{
    char *Temp_KeyBuf = rt_malloc(20);
    char *Temp_ValueBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "Rssi:%ld", Device_ID);
    sprintf(Temp_ValueBuf, "%d", value);
    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
    rt_free(Temp_KeyBuf);
    rt_free(Temp_ValueBuf);
    LOG_D("Writing Rssi %d to key %ld\r\n", value,Device_ID);
}
uint8_t Device_BatGet(uint32_t Device_ID)
{
    uint8_t read_len = 0;
    uint32_t read_value = 0;
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "Bat:%ld", Device_ID);//将传入的数字转换成数组
    memset(read_value_temp,0,64);
    read_len = ef_get_env_blob(keybuf, read_value_temp, 64, NULL);
    if(read_len>0)
    {
        read_value = atol(read_value_temp);
    }
    else
    {
        read_value = 0;
    }
    rt_free(keybuf);//释放临时buffer对应内存空间
    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
    return read_value;
}
void Device_BatChange(uint32_t Device_ID,uint8_t value)
{
    char *Temp_KeyBuf = rt_malloc(20);
    char *Temp_ValueBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "Bat:%ld", Device_ID);
    sprintf(Temp_ValueBuf, "%d", value);
    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
    rt_free(Temp_KeyBuf);
    rt_free(Temp_ValueBuf);
    LOG_D("Writing Bat %ld to key %d\r\n", value,Device_ID);
}
uint8_t Add_Device(uint32_t Device_ID)
{
    uint32_t Num=0;
    Num = Flash_Get_Learn_Nums();
    if(Num>20)return RT_ERROR;
    Num++;
    Flash_LearnNums_Change(Num);
    Global_Device.Num = Num;
    Global_Device.ID[Num] = Device_ID;
    Flash_Key_Change(Num,Device_ID);
    return RT_EOK;
}
uint8_t Add_DoorDevice(uint32_t Device_ID)
{
    uint32_t Num=0;
    if(GetDoorID())
    {
        Num = Flash_Get_Door_Nums();
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.DoorNum = Num;
        Flash_Key_Change(88888888,Num);
        LOG_D("Replace Learn\r\n");
        return RT_EOK;
    }
    else
    {
        Num = Flash_Get_Learn_Nums();
        if(Num>20)return RT_ERROR;
        Num++;
        Flash_LearnNums_Change(Num);
        Global_Device.Num = Num;
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.DoorNum = Num;
        Flash_Key_Change(88888888,Num);
        LOG_D("New Learn\r\n");
        return RT_EOK;
    }
}
uint8_t Add_GatewayDevice(uint32_t Device_ID)
{
    uint32_t Num=0;
    if(GetGatewayID())
    {
        Num = Flash_Get_Door_Nums();
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.GatewayNum = Num;
        Flash_Key_Change(88887777,Num);
        LOG_D("Replace Learn\r\n");
        return RT_EOK;
    }
    else
    {
        Num = Flash_Get_Learn_Nums();
        if(Num>20)return RT_ERROR;
        Num++;
        Flash_LearnNums_Change(Num);
        Global_Device.Num = Num;
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.GatewayNum = Num;
        Flash_Key_Change(88887777,Num);
        LOG_D("New Learn\r\n");
        return RT_EOK;
    }
}
uint32_t GetDoorID(void)
{
    if(Global_Device.DoorNum)
    {
        if(Global_Device.ID[Global_Device.DoorNum])
        {
            return Global_Device.ID[Global_Device.DoorNum];
        }
        else {
            LOG_D("Not Include Door Device\r\n");
            return 0;
        }
    }
    else
    {
        LOG_W("Not Include Door Device ID\r\n");
        return 0;
    }
}
uint32_t GetGatewayID(void)
{
    if(Global_Device.GatewayNum)
    {
        if(Global_Device.ID[Global_Device.GatewayNum])
        {
            return Global_Device.ID[Global_Device.GatewayNum];
        }
        else {
            LOG_D("Not Include Gateway Device\r\n");
            return 0;
        }
    }
    else
    {
        LOG_W("Not Include Gateway Device ID\r\n");
        return 0;
    }
}
uint8_t Delete_Device(uint32_t device_id)
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num] == device_id)
        {
            Global_Device.ID[num]=0;
            Flash_Key_Change(num,0);
            return RT_EOK;
        }
        num--;
    }
    return RT_ERROR;
}
uint8_t Update_Device_Bat(uint32_t Device_ID,uint8_t bat)//更新电量
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.Bat[num] = bat;
            Device_BatChange(Device_ID,bat);
            LOG_D("Device Bat %d is Increase to %d",Global_Device.ID[num],bat);
            return RT_EOK;
        }
        num--;
    }
    LOG_E("Device Bat %d is Increase Fail",Global_Device.ID[num]);
    return RT_ERROR;
}
uint8_t Update_Device_Rssi(uint32_t Device_ID,uint8_t rssi)//更新Rssi
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.Rssi[num] = rssi;
            Device_RssiChange(Device_ID,rssi);
            LOG_D("Device rssi %d is Increase to %d",Global_Device.ID[num],rssi);
            return RT_EOK;
        }
        num--;
    }
    LOG_E("Device Rssi %d is Increase Fail",Global_Device.ID[num]);
    return RT_ERROR;
}
uint8_t Clear_Device_Time(uint32_t Device_ID)//更新时间戳为0
{
    uint16_t num = Global_Device.Num;
    LOG_D("Clear_Device_Time Num is %d\r\n",num);
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.ID_Time[num] = 0;
            Global_Device.Alive[num] = 1;
            LOG_D("Device %d is Clear to 0",Global_Device.ID[num]);
            return RT_EOK;
        }
        num--;
    }
    LOG_E("Device %d is Not Found\r\n",Device_ID);
    return RT_ERROR;
}
void Update_All_Time(void)
{
    uint16_t Num = Global_Device.Num;
    if(!Num)return;
    uint32_t Time = 0;
    for(uint8_t i=1;i<=Num;i++)
    {
        Time = Global_Device.ID_Time[i];//查询剩余时间
        if(Time<25)Time++;                     //自增
        Global_Device.ID_Time[i] = Time;//更新内存中的时间
        LOG_D("Device ID %ld With Time is Update to %d\r\n",Global_Device.ID[i],Global_Device.ID_Time[i]);
    }
    LOG_D("Update_All_Time OK\r\n");
}
void Clear_All_Time(void)
{
    uint16_t Num = Global_Device.Num;
    if(Num)
    {
        for(uint8_t i=1;i<=Num;i++)
        {
            if(Global_Device.ID_Time[i]<25 || Global_Device.Alive[i] == 1)
            {
                Global_Device.Alive[i] = 0;//更新内存中的时间
                Global_Device.ID_Time[i] = 0;//更新内存中的时间
                LOG_D("Device %ld's time is cleard",Global_Device.ID[i]);
            }
        }
        LOG_D("Clear_All_Time OK\r\n");
    }
}
void Detect_All_Time(void)
{
    uint16_t num = Global_Device.Num;
    uint8_t WarnFlag = 0;
    if(!num)return;
    while(num)
    {
        if(Global_Device.ID_Time[num]==25 && Global_Device.Alive[num]==0)
        {
            //掉线ID上报
            if(num == Global_Device.DoorNum)
            {
                LOG_D("Door is Offline\r\n");
            }
            else if(num == Global_Device.GatewayNum)
            {
                LOG_D("Gateway is Offline\r\n");
            }
            else
            {
                WarnFlag = 1;
                LOG_D("Device ID %ld is Offline\r\n",Global_Device.ID[num]);
                WarUpload_GW(Global_Device.ID[num],4,1);//Offline报警
            }
        }
        num--;
    }
    if(WarnFlag)
    {
        Warning_Enable_Num(5);
    }
    Clear_All_Time();
    LOG_D("Detect_All_Time OK\r\n");
}
uint8_t Flash_Get_Key_Valid(uint32_t Device_ID)//查询内存中的ID
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)return RT_EOK;
        num--;
    }
    return RT_ERROR;
}
uint8_t Flash_GetRssi(uint32_t Device_ID)//查询内存中的RSSI
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            return Global_Device.Rssi[num];
        }
        num--;
    }
    return 0;
}
void Offline_React(uint32_t ID)
{
    if(GetNowStatus()!=Offline)
    {
        return;
    }
    uint16_t num = Global_Device.Num;
    uint8_t WarnFlag = 0;
    if(!num)return;
    while(num)
    {
        if(Global_Device.ID_Time[num]==25 && Global_Device.Alive[num]==0)
        {
            //掉线ID上报
            if(num == Global_Device.DoorNum)
            {
                LOG_D("Door is Offline\r\n");
            }
            else if(num == Global_Device.GatewayNum)
            {
                LOG_D("Gateway is Offline\r\n");
            }
            else
            {
                WarnFlag = 1;
                LOG_D("Device ID %ld is Offline\r\n",Global_Device.ID[num]);
            }
        }
        num--;
    }
    if(WarnFlag==0)
    {
        WarUpload_GW(ID,4,0);//Offline报警
        OfflineDisableWarning();
    }
    LOG_D("Detect_All_Time OK\r\n");
}

void LoadDevice2Memory(void)//数据载入到内存中
{
    memset(&Global_Device,0,sizeof(Global_Device));
    Global_Device.Num = Flash_Get_Learn_Nums();

    for(uint8_t i=1;i<=Global_Device.Num;i++)
    {
        Global_Device.ID[i] = Flash_Get_Key_Value(i);
        LOG_D("GOT ID is %ld\r\n",Global_Device.ID[i]);
        Global_Device.Bat[i] = Device_BatGet(Global_Device.ID[i]);
        LOG_D("GOT Bat is %ld\r\n",Global_Device.Bat[i]);
        Global_Device.Rssi[i] = Device_RssiGet(Global_Device.ID[i]);
        LOG_D("GOT Rssi is %ld\r\n",Global_Device.Rssi[i]);
    }
    Global_Device.DoorNum = Flash_Get_Key_Value(88888888);
    Global_Device.GatewayNum = Flash_Get_Key_Value(88887777);
    Global_Device.LastFlag = Flash_Get_Moto_Flag();
    LOG_I("Num is %d",Global_Device.Num);
}
MSH_CMD_EXPORT(LoadDevice2Memory,LoadDevice2Memory);
void DeleteAllDevice(void)//数据载入到内存中
{
    Gateway_RemoteDelete();
    LOG_D("Before Delete num is %d",Global_Device.Num);
    memset(&Global_Device,0,sizeof(Global_Device));
    ef_env_set_default();
//    Flash_LearnNums_Change(0);
//    Flash_Key_Change(88888888,0);//DoorID
//    Flash_Key_Change(88887777,0);//GatewayID
//    Flash_Moto_Change(0);//LastFlag
    LOG_D("After Delete num is %d",Global_Device.Num);
}
