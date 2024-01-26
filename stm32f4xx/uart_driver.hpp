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
* @file   : usart_driver.hpp
* @author : xiongqulin
* @date   : 18 Jan 2023
* @brief  :
*
*********************************************************************************************************
*/

#pragma once

#include "infrastructure/platform/hal/uart.hpp"
#include "infrastructure/component/common/os.hpp"
#include "stm32f4xx.h"

class UartDriver : public Uart {

public:
    inline static UartDriver *uart_object[4];

private:
    UART_HandleTypeDef *_uart;
    os::Semaphore _sem;

    uint16_t _last_index   = 0x00;
    uint16_t _send_len_max = 0x00;
    bool _succed           = true;

public:
    UartDriver(UART_HandleTypeDef *uart)
        : _uart(uart)

    {
        uart_object[change_object(_uart->Instance)] = this;
    }

    bool write(uint8_t *buf, uint32_t len)
    {
        if (HAL_UART_Transmit_DMA(_uart, buf, len) != HAL_OK)
            return false;
        _sem.put();
        return _succed;
    }

    bool read_start(uint8_t *buf, uint32_t len)
    {
        return (HAL_UARTEx_ReceiveToIdle_DMA(_uart, buf, len) == HAL_OK);
    }

public:
    static uint8_t change_object(USART_TypeDef *instance)
    {
        if (instance == USART1)
            return 0;
        else if (instance == USART2)
            return 1;
        else if (instance == USART3)
            return 2;
        else if (instance == USART6)
            return 3;
        return 0xFF;
    }

    static void tx_complete_handle(USART_TypeDef *instance)
    {
        UartDriver *me = uart_object[change_object(instance)];

        me->_succed = true;
        me->_sem.put();
    }

    static void rx_event_handle(USART_TypeDef *instance, uint16_t size)
    {
        UartDriver *me = uart_object[change_object(instance)];

        uint8_t *ptr = &me->_uart->pRxBuffPtr[me->_last_index];
        uint16_t len = size - me->_last_index;

        me->_last_index = size;

        if (len > 0)
            me->_receive_data_callback.call(reinterpret_cast<uint8_t *>(ptr), reinterpret_cast<uint16_t>(len));

        if (me->_uart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
            me->_receive_idle_callback.call();

        if (me->_last_index > me->_send_len_max)
            me->_last_index -= me->_send_len_max;

        me->_succed = true;
    }

    static void error_handle(USART_TypeDef *instance)
    {
        UartDriver *me = uart_object[change_object(instance)];

        me->_succed = false;
        me->_sem.put();
    }
};