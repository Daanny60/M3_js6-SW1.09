/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the keyboard config.
 *
 *  \file       keyboard config.c
 *  \ingroup    driver
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       09/04/2021
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 09/04/2021 | by.lin           | N/A |
 *
******************************************************************************/




/*****************************************************************************
 *  Files Include
 *****************************************************************************/

#include "keyboard_config.h"
#include "Adc_Drv.h"
#include "gpio.h"
/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/
static const GPIO_Type *io_keyboard_config_gpio_tab[NUMBER_OF_IO_KEYBOARD] =
{
  KEY_LEFT_GPIO,
  KEY_RIGHT_GPIO,
  KEY_DOWN_GPIO, 
  KEY_LEFT_PICH_GPIO,
  KEY_RIGHT_PICH_GPIO
};


static const uint32_t io_keyboard_config_gpio_pin_tab[NUMBER_OF_IO_KEYBOARD] =
{
  KEY_LEFT_PIN,
  KEY_RIGHT_PIN,
  KEY_DOWN_PIN, 
  KEY_LEFT_PICH_PIN,
  KEY_RIGHT_PICH_PIN
};
/*
staitc const uint8_t io_keyboard_config_invalid_level_tab[NUMBER_OF_IO_KEYBOARD] =
{
  1,
  1,
  1,
  1,
  1,
};
*/

GPIO_Type* io_keyboard_gpio_get(uint8_t keyboard_id)
{
  GPIO_Type *gpio;

  gpio = (GPIO_Type *)io_keyboard_config_gpio_tab[keyboard_id];

  return (gpio);
}

uint32_t io_keyboard_gpio_pin_get(uint8_t keyboard_id)
{
  uint32_t gpio_pin;

  gpio_pin = io_keyboard_config_gpio_pin_tab[keyboard_id];

  return (gpio_pin);
}





