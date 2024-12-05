#include "Gpio.h"

#include "types.h"



typedef struct {
    GPIO_Type *gpio;
    /*! PIN */
    uint32_t pin;
    /*! 功能模式 */
    uint32_t mode;
    /*! 输出类型 */
    uint32_t outputType;
    /*! 上拉使能 */
    uint32_t pull;
    /*! 数字功能重定向 */
    uint32_t remapPin;
    /*! 模拟开关使能 */
    uint32_t analogSwitch;
} GPIO_ConfigType;
/*M3 IO配置*/
const GPIO_ConfigType GPIO_Config[] = {
    /*TQ06GAZ01把所有带有AD功能的引脚不管有没有用到都配置好了*/
    /* pin 1 Unused 未使用的引脚应设置为输出的，TQ06GAZ01为什么是输入，TQ04JS601都是设置为输出，硬件借用GAZ，但是引脚配置里面未使用的应该改为输出*/
    {
        GPIOA, FL_GPIO_PIN_15, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_ENABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 2 IS adc*/
    {
        GPIOA, FL_GPIO_PIN_0, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 3 AD_IN12*/
    {
        GPIOA, FL_GPIO_PIN_1, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 4 Unused*/
    {
        GPIOA, FL_GPIO_PIN_2, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 5 Unused */
    {
        GPIOA, FL_GPIO_PIN_3, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 6 Unused*/
    {
        GPIOA, FL_GPIO_PIN_4, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 7 Unused*/
    {
        GPIOA, FL_GPIO_PIN_5, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 8  CAN-RX*/
    {
        CAN_RXD_GPIO, CAN_RXD_PIN, FL_GPIO_MODE_DIGITAL, FL_GPIO_OUTPUT_PUSHPULL, FL_ENABLE, FL_ENABLE, FL_DISABLE,
    },
    /* pin 9  CNA-TX*/
    {
        CAN_TXD_GPIO, CAN_TXD_PIN, FL_GPIO_MODE_DIGITAL, FL_GPIO_OUTPUT_PUSHPULL, FL_ENABLE, FL_ENABLE, FL_DISABLE,
    },
    /* pin 10  CNA-STB*/
    {
        CAN_STB_GPIO, CAN_STB_PIN, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 11  reserved*/
    {
        GPIOA, FL_GPIO_PIN_9, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 12  reserved*/
    {
        GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 13  reserved*/
    {
        GPIOA, FL_GPIO_PIN_12, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 14   reserved*/
    {
        GPIOB, FL_GPIO_PIN_2, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 15 reserved */
    {
        GPIOB, FL_GPIO_PIN_3, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 16 reserved*/
    {
        GPIOB, FL_GPIO_PIN_4, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_ENABLE, FL_DISABLE,
    },
    /* pin 17 reserved*/
    {
        GPIOB, FL_GPIO_PIN_5, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_ENABLE, FL_DISABLE,
    },
    /* pin 18 reserved*/
    {
        GPIOB, FL_GPIO_PIN_6, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 19 reserved*/
    {
        GPIOB, FL_GPIO_PIN_7, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 20 reserved*/
    {
        GPIOB, FL_GPIO_PIN_8, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 21 reserved*/
    {
        GPIOB, FL_GPIO_PIN_9, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 22 DEN*/
    {
        GPIOB, FL_GPIO_PIN_10, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 23 reserved */
    {
        GPIOB, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 24 reserved*/
    {
        GPIOB, FL_GPIO_PIN_12, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_ENABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 25 IN-HeatOutput*/
    {
        GPIOC, FL_GPIO_PIN_0, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 26 reserved*/
    {
        GPIOC, FL_GPIO_PIN_1, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 27 XT1 */
    /* pin 28 XT2 */
    /* pin 29 Unused */
    {
        GPIOC, FL_GPIO_PIN_4, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 30 UNUSED */
    {
        GPIOC, FL_GPIO_PIN_5, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 31 Unused */
    {
        GPIOC, FL_GPIO_PIN_6, FL_GPIO_MODE_OUTPUT, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 32  AD_IN6 */
    {
        GPIOC, FL_GPIO_PIN_7, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 33 AD_IN13 */
    {
        GPIOC, FL_GPIO_PIN_8, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 34 AD_IN14*/
    {
        GPIOC, FL_GPIO_PIN_9, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 35 AD_IN15*/
    {
        GPIOC, FL_GPIO_PIN_10, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 36 XT32KO */
    /* pin 37 XT32KI */
    /* pin 38 VDD15 */
    /* pin 39 VSS */
    /* pin 40 VDD */
    /* pin 41 VREFP */
    /* pin 42 VBAT */
    /* pin 43 AD_IN0 */
    {
        GPIOD, FL_GPIO_PIN_11, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 44 AD_IN7*/
    {
        GPIOD, FL_GPIO_PIN_0, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 45 AD-IN1*/
    {
        GPIOD, FL_GPIO_PIN_1, FL_GPIO_MODE_ANALOG, FL_GPIO_OUTPUT_PUSHPULL, FL_DISABLE, FL_DISABLE, FL_DISABLE,
    },
    /* pin 46 CLK */
    /* pin 47 DIO */
    /* pin 48 NRST */
};


void GPIO_Init(void) {
    uint32_t i;
    GPIO_ConfigType *gpio_config;
    GPIO_Type *gpio;
    FL_GPIO_InitTypeDef      GPIO_InitStruct = {0};
    
    
    for (i = 0; i < (sizeof(GPIO_Config) / sizeof(GPIO_ConfigType)); i ++) {
        gpio_config = (GPIO_ConfigType *)(&GPIO_Config[i]);
        GPIO_InitStruct.pin          = gpio_config->pin;
        GPIO_InitStruct.mode         = gpio_config->mode;
        GPIO_InitStruct.pull         = gpio_config->pull;
        GPIO_InitStruct.outputType   = gpio_config->outputType;
        GPIO_InitStruct.remapPin     = gpio_config->remapPin;
        GPIO_InitStruct.analogSwitch = gpio_config->analogSwitch;
        gpio = gpio_config->gpio;
        FL_GPIO_Init(gpio, &GPIO_InitStruct);
    }
    
    FL_GPIO_SetOutputPin(POWER_12V_ENABLE_GPIO, POWER_12V_ENABLE_PIN);
    FL_GPIO_SetOutputPin(HEATING_DEN_GPIO, HEATING_DEN_PIN);
    
    
    
}
/* */
void  vGpioDeinit(void) {
    uint32_t i;
    GPIO_ConfigType *gpio_config;
    GPIO_Type *gpio;
    FL_GPIO_InitTypeDef      GPIO_InitStruct = {0};
    for (i = 0; i < (sizeof(GPIO_Config) / sizeof(GPIO_ConfigType)); i ++) {
        if ((i > 6) && (i < 10))  {}
        else {
            gpio_config = (GPIO_ConfigType *)(&GPIO_Config[i]);
            GPIO_InitStruct.pin          = gpio_config->pin;
            gpio = gpio_config->gpio;
            FL_GPIO_DeInit(gpio, GPIO_InitStruct.pin);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////


/*控制加热开关的两个函数*/
void vHeatOutputOn(void) {
    FL_GPIO_SetOutputPin(HEATING_CTRL_GPIO, HEATING_CTRL_PIN);
}
void vHeatOutputOff(void) {
    FL_GPIO_ResetOutputPin(HEATING_CTRL_GPIO, HEATING_CTRL_PIN);
}




