/*!
 * \file SevenSegmentDisplay.hpp
 * \author by Łukasz Liśnikowski
 */

#ifndef SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_
#define SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_

#include <stdint.h>
#include "gpio.h"
#include "../TemplateParametr/gpioParametr.hpp"


enum dots{
	dots0 = 0x08,
	dots1 = 0x04,
	dots2 = 0x02,
	dots3 = 0x01,
};

/*!
 * \brief The SevenSegmentDisplay class provides support for four-digit seven segment display.
 *
 * Display segments must be anode(+) and digits must be cathode(-).
 * Segments must be connected to eight consecutive pins of the one port.
 * Digits must be connected to four consecutive pins of the second port.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
class SevenSegmentDisplay {
	static_assert(SEGMENT_FIRST_PIN <= 8, "SEGMENT_FIRST_PIN can't be greater than 8");
	static_assert(DIGITS_FIRST_PIN <= 12, "DIGITS_FIRST_PIN can't be greater than 12");
	/*!
	 * \brief The pins of successive segments
	 */
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
	/*!
	 * \brief The pins of successive digits
	 */
	enum{
		dig0 = 1 << DIGITS_FIRST_PIN,
		dig1 = dig0 << 1,
		dig2 = dig1 << 1,
		dig3 = dig2 << 1,
	};
public:
	SevenSegmentDisplay();
	~SevenSegmentDisplay();

	void setNumber(uint16_t number);
	void setDots(uint8_t dots);
	void update();
	void forceUpdate();
	/*!
	 * \brief The function sets updateFlag.
	 * \see updateFlag
	 * \see update()
	 */
	void setUpdateFlag(){updateFlag = true;}

private:
	uint16_t codeOfDigit(uint8_t digit);
	uint16_t codeOfDots(uint8_t place);

	/*!
	 * \brief The displayed number.
	 * \see setNumber()
	 */
	uint16_t number;
	/*!
	 * \brief The displayed dots.
	 * \see setDots()
	 */
	uint8_t dots;
	/*!
	 * \brief The flag for refreshing the next digit.
	 * \see setUpdateFlag()
	 * \see update()
	 */
	bool updateFlag;
	/*!
	 * \brief Indicator of the refresh digit.
	 */
	uint8_t currentDisplayDigit;
};

//-------------


/*!
 * \brief Create new SevenSegmentDisplay class
 * \tparam SEGMENTS_PORT - The port for segments a÷g + dot (8).
 * \tparam SEGMENT_FIRST_PIN - The first segments pin.
 * \tparam DIGITS_PORT - The port for four digits.
 * \tparam DIGITS_FIRST_PIN - The pin of the first digit.
 *
 * \see Port
 *
 * \warning SEGMENT_FIRST_PIN cannot be greater that 8.
 * \warning SEGMENT_FIRST_PIN cannot be greater that 12.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::SevenSegmentDisplay()
	:number(), dots(), updateFlag(), currentDisplayDigit()
{
}

/*!
 * \brief Destructor SevenSegmentDisplay class
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::~SevenSegmentDisplay()
{
}

/*!
 * \brief This function allows you to set the displayed number.
 * \param number - The new displayed number.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::setNumber(uint16_t number)
{
	this->number = number % 10000;
}

/*!
 * \brief This function allows you to set dots on display.
 * \param dots - The flags of displayed dots.
 *
 * The dots starting from left have following the flags: 0x08, 0x04, 0x02, 0x01. You can use enum dots to facilitate the
 * set dots in the right place.
 *
 * You can connect the dots using the OR character ("|"), e.g. display.setDots(display.dots0 | display.dots1);.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::setDots(uint8_t dots)
{
	this->dots = dots;
}

/*!
 * \brief The function refresh next digit if updateFlag is set.
 * \see updateFlag
 * \see setUpdateFlag()
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::update()
{
	if(updateFlag){
		updateFlag = false;
		forceUpdate();
	}
}

/*!
 * \brief The function force refresh next digit without checking updateFlag.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
void SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::forceUpdate()
{
	(&DIGITS_PORT)->BSRR = dig0 | dig1 | dig2 | dig3;

	switch(currentDisplayDigit)
	{
	case 0:{
		uint16_t code = codeOfDigit(number / 1000) | codeOfDots(3);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig0;
		currentDisplayDigit = 1;
		break;
	}
	case 1:{
		uint16_t code = codeOfDigit((number / 100) % 10) | codeOfDots(2);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig1;
		currentDisplayDigit = 2;
		break;
	}
	case 2:{
		uint16_t code = codeOfDigit((number / 10) % 10) | codeOfDots(1);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16);
		(&DIGITS_PORT)->BRR = dig2;
		currentDisplayDigit = 3;
		break;
	}
	case 3:{
		uint16_t code = codeOfDigit(number % 10);
		(&SEGMENTS_PORT)->BSRR = code | (~code << 16) | codeOfDots(0);
		(&DIGITS_PORT)->BRR = dig3;
		currentDisplayDigit = 0;
		break;
	}
	}
}

/*!
 * \brief The function return seven-segment code from digit
 * \param digit - Digit for coding.
 * \return Code from digit. If digit is greater than 9 returns 0.
 */
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

/*!
 * \brief The function return the character of dot if the dot is set in a given place.
 * \param place - A place to check the presence of a dot. Place is from 0 to 3, where 0 is first from left place and 3 last from left.
 * \return The code of dot if in the given place is the dot or 0 if not.
 */
template <Port SEGMENTS_PORT, uint8_t SEGMENT_FIRST_PIN, Port DIGITS_PORT, uint8_t DIGITS_FIRST_PIN>
uint16_t SevenSegmentDisplay<SEGMENTS_PORT, SEGMENT_FIRST_PIN, DIGITS_PORT, DIGITS_FIRST_PIN>::codeOfDots(uint8_t place)
{
	uint8_t isDots = dots & (0x08 >> place);
	if(isDots){
		return segH;
	}
	return 0;
}

#endif /* SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_ */
