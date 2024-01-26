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
* @file   : dio.hpp
* @author : xiongqulin
* @date   : 3 Jan 2023
* @brief  :
*
*********************************************************************************************************
*/

#pragma once

#include "stm32f4xx_ll_gpio.h"
#include "infrastructure/platform/hal/dio.hpp"

class DioDriver : public Dio {
private:
    GPIO_TypeDef *_gpio;
    uint32_t _pin;

public:
    DioDriver(GPIO_TypeDef *gpio, uint32_t pin)
        : _gpio(gpio),
          _pin(pin)
    {
    }

    void write(uint32_t status)
    {
        if (status != 0u)
            LL_GPIO_SetOutputPin(_gpio, _pin);
        else
            LL_GPIO_ResetOutputPin(_gpio, _pin);
    }

    void toggle()
    {
        LL_GPIO_TogglePin(_gpio, _pin);
    }

    bool read_input()
    {
        return LL_GPIO_IsInputPinSet(_gpio, _pin);
    }

    bool read_output()
    {
        return LL_GPIO_IsOutputPinSet(_gpio, _pin);
    }
};
