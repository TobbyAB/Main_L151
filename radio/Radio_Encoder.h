/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-22     Rick       the first version
 */
#ifndef RADIO_RADIO_ENCODER_H_
#define RADIO_RADIO_ENCODER_H_


void Check_Wor_Recv(uint32_t From_ID,uint8_t Command,uint8_t Data);
void Tx_Done_Callback(uint8_t *rx_buffer,uint8_t rx_len);
void RadioSend(uint32_t Taget_Id,uint8_t counter,uint8_t Command,uint8_t Data);
void WorSend(uint32_t Taget_Id,uint8_t counter,uint8_t Command,uint8_t Data);
void RadioEnqueue(uint32_t type,uint32_t Taget_Id,uint8_t counter,uint8_t Command,uint8_t Data);
void GatewaySyncEnqueue(uint8_t type,uint32_t device_id,uint8_t rssi,uint8_t bat);
void GatewaySyncSend(uint8_t type,uint32_t device_id,uint8_t rssi,uint8_t bat);
void GatewayWarningEnqueue(uint32_t device_id,uint8_t rssi,uint8_t warn_id,uint8_t value);
void GatewayWarningSend(uint32_t device_id,uint8_t rssi,uint8_t warn_id,uint8_t value);
void GatewayControlEnqueue(uint32_t device_id,uint8_t rssi,uint8_t control,uint8_t value);
void GatewayControlSend(uint32_t device_id,uint8_t rssi,uint8_t control,uint8_t value);
void RadioDequeueTaskInit(void);
void SendWithOldBuff(void);
void FreqRefresh_Init(void);

#endif /* RADIO_RADIO_ENCODER_H_ */
