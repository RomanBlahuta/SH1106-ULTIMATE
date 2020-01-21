/**********************************************************************************************************************/


/** Includes --------------------------------------------------------------------------------------------------------**/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "sh1106Ultimate.h"


/**********************************************************************************************************************/


void SystemClock_Config(void);


/**********************************************************************************************************************/


int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C3_Init();


    /* USER CODE BEGIN */
    shu_InitDisplay();

    shu_GotoXY(0,0);
    shu_ChooseFont(Font_8x8);
    shu_DrawString("Pixels and lines", WHITE);
    shu_GotoXY(105,55);
    shu_DrawPixel(WHITE);
    shu_GotoXY(75,35);
    shu_DrawPixel(WHITE);
    shu_GotoXY(15, 27);
    shu_DrawLine(15, 60, WHITE);
    shu_GotoXY(25, 17);
    shu_DrawLine(90, 17, WHITE);
    shu_GotoXY(25, 47);
    shu_DrawLine(80, 60, WHITE);

    shu_UpdateDisplay();

    HAL_Delay(6000);
    shu_ClearDisplay();

    shu_GotoXY(0,0);
    shu_DrawString("Shapes", WHITE);
    shu_GotoXY(20, 20);
    shu_DrawRectangle(50, 30, WHITE);
    shu_GotoXY(60, 40);
    shu_FillArea(100, 60, WHITE);
    shu_GotoXY(35, 35);
    shu_DrawRectangle(55, 55, WHITE);
    shu_GotoXY(70, 10);
    shu_FillArea(120, 30, WHITE);
    shu_GotoXY(72, 12);
    shu_FillArea(118, 28, BLACK);

    shu_UpdateDisplay();

    HAL_Delay(6000);
    shu_ClearDisplay();

    shu_GotoXY(0,0);
    shu_DrawString("Fonts", WHITE);

    shu_GotoXY(0,12);
    shu_ChooseFont(Font_4x6);
    shu_DrawString("Font4x6", WHITE);

    shu_GotoXY(0,24);
    shu_ChooseFont(Font_5x7);
    shu_DrawString("Font5x7", WHITE);

    shu_GotoXY(0,36);
    shu_ChooseFont(Font_6x8);
    shu_DrawString("Font6x8", WHITE);

    shu_GotoXY(0,48);
    shu_ChooseFont(Font_8x8);
    shu_DrawString("Font8x8", WHITE);

    shu_GotoXY(100,40);
    shu_DrawString(":)", WHITE);

    shu_UpdateDisplay();


    HAL_Delay(6000);
    shu_ClearDisplay();

    shu_GotoXY(0,0);
    shu_DrawString("Bitmaps", WHITE);
    shu_GotoXY(0, 11);
    shu_DrawLine(60, 11, WHITE);

    uint16_t bmp[] = {0x3c, 0x42, 0x89, 0xa1, 0xa1, 0x89, 0x42, 0x3c};
    shu_GotoXY(60, 32);
    shu_DrawBitmap(8,8, bmp);
    shu_GotoXY(40,32);
    uint16_t bmp2[] = {0xff, 0x81, 0x89, 0x85, 0xb5, 0x89, 0x81, 0xff};
    shu_DrawBitmap(8,8,bmp2);

    shu_UpdateDisplay();
    HAL_Delay(6000);

    shu_ClearDisplay();

    for (int i = 0; i < 62; i+=2) {
        shu_GotoXY(i, 30);
        shu_DrawString("The end", BLACK);
        shu_UpdateDisplay();
        HAL_Delay(250);
        shu_ClearDisplay();
    }

    for (int i = 62; i > 0; i-=2) {
        shu_GotoXY(i, 30);
        shu_DrawString("The end", BLACK);
        shu_UpdateDisplay();
        HAL_Delay(250);
        shu_ClearDisplay();
    }



    while (1)
    {
    }

    /* USER CODE END */
}


/**********************************************************************************************************************/


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


/**********************************************************************************************************************/