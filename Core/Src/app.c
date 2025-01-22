/*
 * app.c
 *
 *  Created on: Jan 16, 2025
 *      Author: user
 */

#include "app.h"
#include <stdbool.h>

// 외부 장치 선언
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

uint16_t adcValue[2];
uint16_t adcDone = 0;
char str[50];

uint16_t movingMean (uint16_t inValue) {
#define max 100
	static uint16_t buffer[max] = {0,};
	static uint32_t sum = 0;
	static uint16_t pos = 0;
	static int isFirst = 5;
	if (isFirst) {
		isFirst--;
		for(int i = 0; i < max; i++) {
			buffer[i] = inValue;
		}
		sum = inValue * 100;
	}
	sum -= buffer[pos];
	buffer[pos] = inValue;
	sum += buffer[pos];
	pos++;
	pos %= 100;
	return sum/100;
}

double Kalman(double measurement) { // Kalman filter
	// setup
	static double P = 1.0;
	static double varP = 0.0001;
	static double R = 0.25;
	static double K = 1.0;
	static double X = 20.0;

	// simple filter
	P = P + varP;
	K = P / (P + R);
	X = (K* measurement) + (1 - K) * X;
	P = (1 - K) * P;

	return X;
}
// 타이머가 리셋될 때 발생되는 인터럽트
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	sprintf(str, "%d, %d, %d\n", adcValue[1], movingMean(adcValue[1]), (int)Kalman(adcValue[1]));
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
			printf(str);
			adcDone = 1;
		}
	}
}
