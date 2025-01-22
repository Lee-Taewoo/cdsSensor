/*
 * app.c
 *
 *  Created on: Jan 16, 2025
 *      Author: user
 */

#include "app.h"

// 외부 장치 선언
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

uint16_t adcValue[2];
uint32_t adcSum = 0;
uint16_t adcDone = 0;

// 타이머가 리셋될 때 발생되는 인터럽트
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	adcSum += adcValue[1];
	if(adcDone > 0) {
		adcDone--;
	}
}

void app() {
	// 시리얼 포트 초기화
	initUart(&huart2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, 2);
	HAL_TIM_Base_Start_IT(&htim3);
	while(1) {
		if (adcDone == 0) {
			printf("%ld\n", adcSum / 100);
			adcSum = 0;
			adcDone = 100;
		}
	}
}
