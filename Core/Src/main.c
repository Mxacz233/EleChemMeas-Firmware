/* USER CODE BEGIN Header */
/**
	******************************************************************************
	* @file           : main.c
	* @brief          : Main program body
	******************************************************************************
	* @attention
	*
	* Copyright (c) 2024 STMicroelectronics.
	* All rights reserved.
	*
	* This software is licensed under terms that can be found in the LICENSE file
	* in the root directory of this software component.
	* If no LICENSE file comes with this software, it is provided AS-IS.
	*
	******************************************************************************
	*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h> // ����math.h��ʹ��round����
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint16_t dacConvert(float targetVoltage);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_USART2_UART_Init();
	MX_TIM10_Init();
	MX_TIM11_Init();
	MX_TIM13_Init();
	/* USER CODE BEGIN 2 */
	// +----------------+-------------------+----------------+
	// | ��������       | ����              | ���ű��       |
	// +----------------+-------------------+----------------+
	// | �αȵ缫       | ReferEle ADC1_IN0 | PA0            |
	// | �����缫       | WorkEle  DAC1_OUT1| PA1             |
	// | �Ե缫         | ConEle DAC1_OUT2  | PA5            |
	// | �û�����       | B1 GPIO_EXTI13    | PC13           |
	// +----------------+-------------------+----------------+

	// | ����״̬ | �����缫�������� | ��ʵ��ѹ���     | ��Ч��ѹ        |
	// +--------+-------------------+------------------+-----------------+
	// | 0      | ��ѹ              | 0.65V            | -1.00V          |
	// | 1      | ��ѹ              | 1.65V            | 0.00V           |
	// | 2      | ��ѹ              | 2.65V            | 1.00V           |
	// | 3      | ��·/�ر�DAC���  | 0.00V            | -1.65V          |
	// | 4      | CVѭ������        | 0.65V~2.65V~0.65V| -1V~1V~1V 10mV  |
	// | 5      | DPV����������   | 0.65V~2.65V~0.65V| -1V~1V~1V 10mV  |
	// +--------+-------------------+------------------+-----------------+

	// ��ʼ��DAC���Ե缫�㶨1.65V�������缫��ʼ��Ϊ1.65V
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
	// ��ʼ��ADC
	HAL_ADC_Start(&hadc1);
	// ������ʱ��10 11
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);
	
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
	* @brief System Clock Configuration
	* @retval None
	*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
	Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
// region USER CODE BEGIN 4

// �ض���printf����
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

// ���尴��״̬��
typedef enum {
	BUTTON_IDLE,
	BUTTON_DEBOUNCE,
	BUTTON_PRESSED
} ButtonState;

volatile ButtonState buttonState = BUTTON_IDLE;
volatile uint32_t buttonPressCount = 5;
volatile uint32_t debounceCounter = 0;
volatile uint8_t counterStateNum = 6;
volatile uint8_t testflag=0;

// ��ѹ����
volatile uint32_t voltageStep = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// TIM10��״̬��
	if (htim->Instance == TIM10) {
		// ��ȡADCֵ
		// HAL_ADC_PollForConversion(&hadc1, 100);
		// uint32_t adcValue = HAL_ADC_GetValue(&hadc1);
		// float voltage = (float)adcValue / 4095 * 3.3f;
		// printf("ADC Value: %lu, Voltage: %.2fV\r\n", adcValue, voltage);
		// printf("buttonPressCount: %d\r\n", buttonPressCount);
		// ���ݰ���״̬�����ͬ��ѹ
		switch (buttonPressCount) {
			case 0: //��ѹ-1V�����Ե缫�㶨1.65V�������缫�㶨0.65V
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 807);
				printf("WorkMode 0: Constant Voltage -1V\r\n");
				break;
			case 1: //��ѹ0V�����Ե缫�㶨1.65V�������缫�㶨1.65V
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
				printf("WorkMode 1: Constant Voltage 0V\r\n");
				break;
			case 2: //��ѹ1V�����Ե缫�㶨1.65V�������缫�㶨2.65V
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3288);
				printf("WorkMode 2: Constant Voltage 1V\r\n");
				break;
			case 3: //��·/�ر�DAC��������Ե缫�㶨1.65V�������缫�ر�
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
				printf("WorkMode 3: Open Circuit\r\n");
				break;
			case 4: //CVѭ�����������Ե缫�㶨1.65V�������缫��-1V��1V��-1V��10mV����
				// �����ʱ��13δ��������������ʱ��13
				if (voltageStep == 0) {
					HAL_TIM_Base_Start_IT(&htim13);
				}

				// ��ʼ��ѹmV
				uint16_t startVoltageCV = 650;
				// �����ѹ������10�ı��������е�ѹ����
				if (voltageStep % 10 == 0) {
					if (voltageStep <= 200) {
						startVoltageCV += 10;
					} else {
						startVoltageCV -= 10;
					}
				}
				// ˢд��ѹ
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacConvert(startVoltageCV / 1000.0f));

				// -1V~1V~1V 10mV������Ӧ����400�Σ���������ֹͣ
				if (voltageStep >= 400) {
					HAL_TIM_Base_Stop_IT(&htim13);
					voltageStep = 0;
				}
				printf("WorkMode 4: CV\r\n");
				break;
			case 5: //DPV���������������Ե缫�㶨1.65V�������缫��-1V��1V��-1V��10mV������ÿ��������������һ�������Ĺ���
				// �����ʱ��13δ��������������ʱ��13
				if (voltageStep == 0) {
					HAL_TIM_Base_Start_IT(&htim13);
				}

				// ��ʼ��ѹmV
				uint16_t startVoltageDPV = 650;
				// �����ѹ������10�ı��������е�ѹ����
				if (voltageStep % 10 == 0) {
					if (voltageStep <= 200) {
						startVoltageDPV += 10;
					} else {
						startVoltageDPV -= 10;
					}
					// ����������
					startVoltageDPV += 15;
				}
				if (voltageStep % 10 == 1) {
					// �����½���
					startVoltageDPV -= 15;
				}
				// ˢд��ѹ
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacConvert(startVoltageDPV / 1000.0f));

				printf("WorkMode 5: DPV \r\n");
				break;
			default:
				break;
		}
	}

	// TIM11��������״̬��
	if (htim->Instance == TIM11) {
		switch (buttonState) {
			case BUTTON_IDLE:
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
					buttonState = BUTTON_DEBOUNCE;
					debounceCounter = 0;
				}
				break;

			case BUTTON_DEBOUNCE:
				if (debounceCounter++ >= 50) { // ����50���ж�Ϊ����ʱ��
					if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
						buttonState = BUTTON_PRESSED;
						buttonPressCount++;
						buttonPressCount %= counterStateNum;
					} else {
						buttonState = BUTTON_IDLE;
					}
				}
				break;

			case BUTTON_PRESSED:
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
					buttonState = BUTTON_IDLE;
				}
				break;
		}
	}

	// TIM13��ѹ����
	if (htim->Instance == TIM13) {
		voltageStep++;
	}
}


uint16_t dacConvert(float targetVoltage) {
	// �ο���ѹΪ3.3V���ֱ���Ϊ4096
	float referenceVoltage = 3.3f;
	uint16_t resolution = 4096;

	// ����DACֵ����������
	uint16_t dacValue = (uint16_t)round((targetVoltage / referenceVoltage) * (resolution - 1));

	// ȷ��DACֵ��0��4095֮��
	if (dacValue > 4095) {
		dacValue = 4095;
	} else if (dacValue < 0) {
		dacValue = 0;
	}

	return dacValue;
}
// endregion USER CODE END 4
/* USER CODE END 4 */

/**
	* @brief  This function is executed in case of error occurrence.
	* @retval None
	*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
	* @brief  Reports the name of the source file and the source line number
	*         where the assert_param error has occurred.
	* @param  file: pointer to the source file name
	* @param  line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
