/*
 * SevenSegmentDisplay.hpp
 *
 *  Created on: Sep 28, 2022
 *      Author: Łukasz
 */

#ifndef SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_
#define SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_

#include <stdint.h>
#include "gpio.h"
#include "../TemplateParametr/gpioParametr.hpp"

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
class SevenSegmentDisplay {
public:
	enum{
		segA = 1 << 0,
		segB = 1 << 1,
		segC = 1 << 2,
		segD = 1 << 3,
		segE = 1 << 4,
		segF = 1 << 5,
		segG = 1 << 6,
		segH = 1 << 7,
	};

	SevenSegmentDisplay();
	~SevenSegmentDisplay();

	void setNumber(uint16_t number);
	void setDots(uint8_t dots);
	void update();
	void forceUpdate();
	void setUpdateFlag(){updateFlag = true;}

private:
	uint16_t codeOfDigit(uint8_t digit);

	uint16_t number;
	uint8_t dots;
	bool updateFlag;
};

//-------------

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::SevenSegmentDisplay()
	:number(0), dots(0), updateFlag()
{
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::~SevenSegmentDisplay()
{
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
void SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::setNumber(uint16_t number)
{
	this->number = number % 10000;
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
void SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::setDots(uint8_t dots)
{
	this->dots = dots;
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
void SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::update()
{
	if(updateFlag){
		updateFlag = false;
		forceUpdate();
	}
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
void SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::forceUpdate()
{
	static uint8_t currentDigit = 0;

	(&DIGITS_PORT)->BSRR = DIGIT_0_Pin | DIGIT_1_Pin | DIGIT_2_Pin | DIGIT_3_Pin;

	switch(currentDigit)
	{
	case 0:{
		uint16_t code = codeOfDigit(number / 1000);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = DIGIT_0_Pin;
		currentDigit = 1;
		break;
	}
	case 1:{
		uint16_t code = codeOfDigit((number / 100) % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = DIGIT_1_Pin;
		currentDigit = 2;
		break;
	}
	case 2:{
		uint16_t code = codeOfDigit((number / 10) % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = DIGIT_2_Pin;
		currentDigit = 3;
		break;
	}
	case 3:{
		uint16_t code = codeOfDigit(number % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = DIGIT_3_Pin;
		currentDigit = 0;
		break;
	}
	}
}

template <Port SEGMENTS_PORT, Port DIGITS_PORT>
uint16_t SevenSegmentDisplay<SEGMENTS_PORT, DIGITS_PORT>::codeOfDigit(uint8_t digit)
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

#endif /* SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_ */
