/*
 * SevenSegmentDisplay.cpp
 *
 *  Created on: Sep 28, 2022
 *      Author: Łukasz
 */

#include "sevenSegmentDisplay.hpp"
#include "gpio.h"



SevenSegmentDisplay::SevenSegmentDisplay()
	:number(0), dots(0), updateFlag()
{
}

SevenSegmentDisplay::~SevenSegmentDisplay()
{
}

void SevenSegmentDisplay::setNumber(uint16_t number)
{
	this->number = number % 10000;
}

void SevenSegmentDisplay::setDots(uint8_t dots)
{
	this->dots = dots;
}

void SevenSegmentDisplay::update()
{
	if(updateFlag){
		updateFlag = false;
		forceUpdate();
	}
}

void SevenSegmentDisplay::forceUpdate()
{
	static uint8_t currentDigit = 0;

	GPIOB->BSRR = DIGIT_0_Pin | DIGIT_1_Pin | DIGIT_2_Pin | DIGIT_3_Pin;

	switch(currentDigit)
	{
	case 0:{
		uint16_t code = codeOfDigit(number / 1000);
		GPIOA->BSRR = code | (~code << 16);
		GPIOB->BRR = DIGIT_0_Pin;
		currentDigit = 1;
		break;
	}
	case 1:{
		uint16_t code = codeOfDigit((number / 100) % 10);
		GPIOA->BSRR = code | (~code << 16);
		GPIOB->BRR = DIGIT_1_Pin;
		currentDigit = 2;
		break;
	}
	case 2:{
		uint16_t code = codeOfDigit((number / 10) % 10);
		GPIOA->BSRR = code | (~code << 16);
		GPIOB->BRR = DIGIT_2_Pin;
		currentDigit = 3;
		break;
	}
	case 3:{
		uint16_t code = codeOfDigit(number % 10);
		GPIOA->BSRR = code | (~code << 16);
		GPIOB->BRR = DIGIT_3_Pin;
		currentDigit = 0;
		break;
	}
	}
}

uint16_t SevenSegmentDisplay::codeOfDigit(uint8_t digit)
{
	switch(digit)
	{
	case 0:
		return segA | segB | segC | segD | segE | segF;
	case 1:
		return segB | segC;
	case 2:
		return segA | segB | segD | segE | segG;
	case 3:
		return segA | segB | segC | segD | segG;
	case 4:
		return segB | segC | segF | segG;
	case 5:
		return segA | segC | segD | segF | segG;
	case 6:
		return segA | segC | segD | segE | segF | segG;
	case 7:
		return segA | segB | segC;
	case 8:
		return segA | segB | segC | segD | segE | segF | segG;
	case 9:
		return segA | segB | segC | segD | segF | segG;
	default:
		return 0;
	}
}
