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

void app() {
	// 시리얼 포트 초기화
	initUart(&huart2);
	while(1) {
		// adc 변환 시작
		HAL_ADC_Start(&hadc1);
		// adc 대기
		HAL_ADC_PollForConversion(&hadc1, 1);
		// 값 읽기
		uint16_t value1 = HAL_ADC_GetValue(&hadc1);
		// adc 종료
		// adc 대기
		HAL_ADC_PollForConversion(&hadc1, 1);
		// 값 읽기
		uint16_t value2 = HAL_ADC_GetValue(&hadc1);
		// adc 종료
		HAL_ADC_Stop(&hadc1);
		printf("cds value = %d %d\n", value1, value2);
		HAL_Delay(100);
	}
}
