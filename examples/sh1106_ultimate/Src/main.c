/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "sh1106Ultimate.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

//I2C_HandleTypeDef hi2c3;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void DEMO() {
    int choices[] = {0, 1, 2, 3};
    int choice = 0;

    shu_InitDisplay();
    shu_ChooseFont(Font_5x7);

    for (int i = 0; i < 4; i++) {
        choice = choices[i];

        if (choice == 0) {
            shu_GotoXY(0, 1);
            shu_DrawString("SH1106-ULTIMATE");
            shu_GotoXY(0, 2);
            shu_DrawString("Hello World!");
            shu_GotoXY(0, 4);
            shu_DrawString("MADE FOR UC");
            shu_DrawChar('U');
            HAL_Delay(8000);
        }
        else if (choice == 1) {

            shu_GotoXY(24,3);
            shu_DrawChar('I');
            shu_DrawChar(' ');
            //shu_DrawChar(' ');

            shu_GotoXY(shu_GetX(),2);
            shu_DrawHeart();

            uint16_t someX = shu_GetX();
            shu_GotoXY(someX, 3);
            shu_DrawString("|| O K");

            shu_GotoXY(someX+3, 2);
            shu_DrawString("_");

            shu_GotoXY(50, 6);
            shu_DrawString(":=)");
            HAL_Delay(8000);
        }
        else if (choice == 3) {

            shu_GotoXY(0, 2);
            shu_DrawString("=====================");

            shu_GotoXY(35, 3);
            shu_DrawString("Thank you");

            shu_GotoXY(24, 4);
            shu_DrawString("for attention!");

            shu_GotoXY(0, 5);
            shu_DrawString("=====================");
            HAL_Delay(8000);
        }
        else if (choice == 2) {
            uint16_t prevX = 0;
            uint16_t prevY = 4;
            int ticks = 0;
            while (ticks < 15) {

                shu_GotoXY(30, 1);
                shu_DrawString("ANIMATION!");

                shu_GotoXY(prevX, prevY);
                shu_DrawString("-->");
                prevX = shu_GetX();
                prevY = shu_GetY();

                HAL_Delay(5);

                shu_ClearDisplay();

                shu_GotoXY(30, 1);
                shu_DrawString("ANIMATION!");

                HAL_Delay(1);
                ticks++;
            }
            shu_ClearDisplay();
            shu_GotoXY(40, 4);
            shu_DrawString("THE END");
            HAL_Delay(3000);
        }
        shu_ClearDisplay();
    }


}

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
    MX_I2C3_Init();
    MX_SPI1_Init();
    /* USER CODE BEGIN 2 */
    DEMO();
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
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
