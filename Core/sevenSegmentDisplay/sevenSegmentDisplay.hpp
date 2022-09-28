/*
 * SevenSegmentDisplay.hpp
 *
 *  Created on: Sep 28, 2022
 *      Author: Łukasz
 */

#ifndef SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_
#define SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_

#include <stdint.h>

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

#endif /* SEVENSEGMENTDISPLAY_SEVENSEGMENTDISPLAY_HPP_ */
