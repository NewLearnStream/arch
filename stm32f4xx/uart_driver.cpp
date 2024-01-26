/**
*********************************************************************************************************
* Copyright (C) 2023-2026 by xiongqulin - All Rights Reserved.                                              *
*                                                                                                       *
* This file is part of the project.                                                                     *
*                                                                                                       *
* This file can not be copied and/or distributed without the express permission of the project owner.   *
*********************************************************************************************************/

/**
*********************************************************************************************************
* @file   : usart_driver.cpp
* @author : xiongqulin
* @date   : 19 Jan 2023
* @brief  :
*
*********************************************************************************************************
*/

#include "uart_driver.hpp"

extern "C" {
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    UartDriver::tx_complete_handle(huart->Instance);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    UartDriver::rx_event_handle(huart->Instance, size);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    UartDriver::error_handle(huart->Instance);
}
}