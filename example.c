#include "stm32f405xxx.h"
#include "stm32f405xxx_gpio_driver.h"
#include "stm32f405xxx_spi_driver.h"
#include "sys_clock.h"
#include "ST7789.h"

/*
 * ==========================================================
 * ST7789 PIN CONNECTIONS — STM32F407
 * ==========================================================
 *
 * SPI2:
 * SCK  -> PB13 (AF5)
 * MISO -> PB14 (AF5)  // optional
 * MOSI -> PB15 (AF5)
 *
 * ST7789:
 * CS   -> PD0
 * DC   -> PD1
 * RESET-> PD2
 * BL   -> PD3
 *
 * ==========================================================
 */

SPI_Handle_t SPIHandle;
LCD_Handle_t LCD;


/*
 * Initialize SPI2 GPIO pins
 */

#define SCB_CPACR (*((volatile uint32_t *)0xE000ED88))
static void SPI2_GPIO_Init(void)
{
    GPIO_Handle_t GPIOHandle;

    GPIOHandle.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_ALTFN;
    GPIOHandle.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    GPIOHandle.GPIO_PinConfig.GPIO_PinOPType     = GPIO_OP_TYPE_PP;
    GPIOHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIOHandle.GPIO_PinConfig.GPIO_PinSpeed      = GPIO_SPEED_FAST;

    GPIOHandle.pGPIOx = GPIOB;

    // SCK -> PB13
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&GPIOHandle);

    // MISO -> PB14
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&GPIOHandle);

    // MOSI -> PB15
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&GPIOHandle);
}


/*
 * Configure SPI2 for ST7789
 */
static void SPI2_Config(void)
{
    SPIHandle.pSPI = SPI2;

    SPIHandle.SPI_Pinconfig.BusConfig =
            SPI_BUS_CONFIG_FD;

    SPIHandle.SPI_Pinconfig.CPHA =
            SPI_CPHA_HIGH;

    SPIHandle.SPI_Pinconfig.CPOL =
            SPI_CPOL_HIGH;

    SPIHandle.SPI_Pinconfig.DFF =
            SPI_DFF_8BITS;

    SPIHandle.SPI_Pinconfig.DeviceMode =
            SPI_DEVICE_MODE_MASTER;

    SPIHandle.SPI_Pinconfig.SSM =
            SPI_SSM_EN;

    SPIHandle.SPI_Pinconfig.SclkSpeed =
            SPI_CLK_SPEED_DIV16;

    SPI_Init(&SPIHandle);

    SPI_PeripheralControl(SPI2, ENABLE);
}


/*
 * Initialize ST7789 control pins
 */
static void LCD_Pins_Init(void)
{
    GPIO_Handle_t GPIOHandle;

    GPIOHandle.GPIO_PinConfig.GPIO_PinMode =
            GPIO_MODE_OUT;

    GPIOHandle.GPIO_PinConfig.GPIO_PinOPType =
            GPIO_OP_TYPE_PP;

    GPIOHandle.GPIO_PinConfig.GPIO_PinPuPdControl =
            GPIO_NO_PUPD;

    GPIOHandle.GPIO_PinConfig.GPIO_PinSpeed =
            GPIO_SPEED_FAST;

    GPIOHandle.pGPIOx = GPIOD;


    // CS -> PD0
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber =
            GPIO_PIN_NO_0;

    GPIO_Init(&GPIOHandle);

    GPIO_WriteToOutputPin(
            GPIOD,
            GPIO_PIN_NO_0,
            ENABLE
    );


    // DC -> PD1
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber =
            GPIO_PIN_NO_1;

    GPIO_Init(&GPIOHandle);


    // RESET -> PD2
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber =
            GPIO_PIN_NO_2;

    GPIO_Init(&GPIOHandle);


    // BL -> PD3
    GPIOHandle.GPIO_PinConfig.GPIO_PinNumber =
            GPIO_PIN_NO_3;

    GPIO_Init(&GPIOHandle);

    GPIO_WriteToOutputPin(
            GPIOD,
            GPIO_PIN_NO_3,
            ENABLE
    );


    /*
     * Configure LCD handle
     */
    LCD.pSPIHandle = &SPIHandle;

    LCD.CSPort = GPIOD;
    LCD.CSPin  = GPIO_PIN_NO_0;

    LCD.DCPort = GPIOD;
    LCD.DCPin  = GPIO_PIN_NO_1;

    LCD.ResetPort = GPIOD;
    LCD.ResetPin  = GPIO_PIN_NO_2;

    LCD.BLPort = GPIOD;
    LCD.BLPin  = GPIO_PIN_NO_3;
}


/*
 * Main
 */
int main(void)
{
    /*
     * Enable FPU
     */
    SCB_CPACR |= (0xF << 20);


    /*
     * Configure system clock
     */
    SystemClock_Config_168MHz();


    /*
     * Enable GPIO clocks
     */
    GPIO_PeriClockControl(GPIOB, ENABLE);
    GPIO_PeriClockControl(GPIOD, ENABLE);


    /*
     * Configure SPI2
     */
    SPI2_GPIO_Init();
    SPI2_Config();


    /*
     * Configure LCD control pins
     */
    LCD_Pins_Init();


    /*
     * Initialize ST7789
     */
    LCD_Init(&LCD);


    /*
     * Test different colors
     */

    LCD_FillColor(&LCD, LCD_COLOR_RED);

    LCD_FillColor(&LCD, LCD_COLOR_BLACK);

    LCD_FillColor(&LCD, LCD_COLOR_BLUE);

    LCD_FillColor(&LCD, LCD_COLOR_WHITE);

    LCD_FillColor(&LCD, LCD_COLOR_GREEN);


    /*
     * Final screen
     */
    LCD_FillColor(&LCD, LCD_COLOR_BLACK);
    /*
     * Test the ST7789 display
     */

    LCD_FillColor(&LCD, LCD_COLOR_BLACK);

    /* Display title */
    LCD_WriteText(
        &LCD,
        10,
        10,
        "ST7789 TFT DISPLAY",
        LCD_COLOR_WHITE,
        LCD_COLOR_BLACK,
        2
    );

    /* Display resolution */
    LCD_WriteText(
        &LCD,
        10,
        50,
        "240 x 320",
        LCD_COLOR_GREEN,
        LCD_COLOR_BLACK,
        2
    );

    /* Display interface */
    LCD_WriteText(
        &LCD,
        10,
        80,
        "SPI INTERFACE",
        LCD_COLOR_RED,
        LCD_COLOR_BLACK,
        2
    );

    /* Display MCU */
    LCD_WriteText(
        &LCD,
        10,
        95,
        "STM32F407",
        LCD_COLOR_GREEN,
        LCD_COLOR_BLACK,
        2
    );

    while(1)
    {
    }
}
