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

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
class SevenSegmentDisplay {
public:
	enum{
		segA = 1 << SEGMENT_FIRST_PIN,
		segB = segA << 1,
		segC = segB << 1,
		segD = segC << 1,
		segE = segD << 1,
		segF = segE << 1,
		segG = segF << 1,
		segH = segG << 1,
	};
	enum{
		dig0 = 1 << DIGITS_FIRST_PIN,
		dig1 = dig0 << 1,
		dig2 = dig1 << 1,
		dig3 = dig2 << 1,
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

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::SevenSegmentDisplay()
	:number(0), dots(0), updateFlag()
{
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::~SevenSegmentDisplay()
{
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::setNumber(uint16_t number)
{
	this->number = number % 10000;
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::setDots(uint8_t dots)
{
	this->dots = dots;
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::update()
{
	if(updateFlag){
		updateFlag = false;
		forceUpdate();
	}
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::forceUpdate()
{
	static uint8_t currentDigit = 0;

	(&DIGITS_PORT)->BSRR = dig0 | dig1 | dig2 | dig3;

	switch(currentDigit)
	{
	case 0:{
		uint16_t code = codeOfDigit(number / 1000);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig0;
		currentDigit = 1;
		break;
	}
	case 1:{
		uint16_t code = codeOfDigit((number / 100) % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig1;
		currentDigit = 2;
		break;
	}
	case 2:{
		uint16_t code = codeOfDigit((number / 10) % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig2;
		currentDigit = 3;
		break;
	}
	case 3:{
		uint16_t code = codeOfDigit(number % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig3;
		currentDigit = 0;
		break;
	}
	}
}

template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
uint16_t SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::codeOfDigit(uint8_t digit)
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
