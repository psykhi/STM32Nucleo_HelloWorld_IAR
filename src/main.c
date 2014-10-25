/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    11-April-2014
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
/** @addtogroup Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void init_LED(void);
static void init_delay_timer(void);
static void delay_ms(uint16_t i_ms);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
	 */
	/* On the Nucleo board, there is not crystal connected to the STM32,
	 * meaning that it has to rely on the internal HSI clock to run.
	 * The maximum achievable speed is therefore 64 MHz, according to ST.
	 * We need to call SystemCoreClockUpdate so the good SystemCoreClock
	 * value is set. Otherwise 72MHz will be used...
	 */
	SystemCoreClockUpdate();
	/*printf("Hello World ! From theboredengineers.com");*/

	/* We init the LED GPIO */
	init_LED();
	/* In order to use a delay, we need to use a general purpose
	 * timer that will count the amount needed.
	 */
	init_delay_timer();
	/* Init timer */
	/* Infinite loop */
	while (1)
	{
		delay_ms(1000);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		delay_ms(1000);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	}
}

/**
 * @brief Initializes the LED on GPIO A5
 */
static void init_LED(void)
{
	GPIO_InitTypeDef gpio_led;

	/* STM32 Nucleo's LED is located on pin A5 */
	gpio_led.GPIO_Mode =GPIO_Mode_Out_PP;
	gpio_led.GPIO_Pin = GPIO_Pin_5;
	gpio_led.GPIO_Speed =GPIO_Speed_10MHz;
	/* We init the clock here before using the GPIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Now we init the GPIO */
	GPIO_Init(GPIOA,&gpio_led);
}

/**
 * @brief Here we will initializes a general purpose timer (TIM2)
 * that will be used for the delay functions.
 */
static void init_delay_timer(void)
{
	uint16_t prescaler = 0;
	TIM_TimeBaseInitTypeDef TIM2_config;
	/*We enable RCC on the timer 2. */
	/* Timers are low speed peripherals so we use the APB1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Now we need a prescaler value in order to achieve the clock rate we need.
	 * Here we need a 1000Hz frequency*/
	prescaler = (uint16_t) ((SystemCoreClock) / 1000) - 1;
	/* Time base configuration */
	/*65535 is actually the maximum value for the counter before it wraps around */
	TIM2_config.TIM_Period = 65535;
	TIM2_config.TIM_Prescaler = prescaler;
	TIM2_config.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM2_config.TIM_CounterMode = TIM_CounterMode_Up;

	/* Now we call the init function */
	TIM_TimeBaseInit(TIM2, &TIM2_config);
	/* We actually start the counter */
	TIM_Cmd(TIM2, ENABLE);
}
/**
 * @brief wait for i_ms
 * @param i_ms milliseconds to wait (max : 65535)
 */
static void delay_ms(uint16_t i_ms)
{
	assert_param(i_ms < 65536);
	/* We set the counter value to 0 */
	TIM_SetCounter(TIM2,0);


	/* Now we wait until we have counter enough ms*/
	while(TIM_GetCounter(TIM2) < i_ms);
}
#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
