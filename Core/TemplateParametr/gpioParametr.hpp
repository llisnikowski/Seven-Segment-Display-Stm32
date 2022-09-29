/*
 * gpioParametr.hpp
 *
 *  Created on: 29 wrz 2022
 *      Author: Łukasz
 */

#ifndef TEMPLATEPARAMETR_GPIOPARAMETR_HPP_
#define TEMPLATEPARAMETR_GPIOPARAMETR_HPP_

#include <stdint.h>
#include <gpio.h>

enum class Port : uint8_t
{
	A,
	B,
	C,
	D,
	E
};

constexpr GPIO_TypeDef * getPort(Port port)
{
	switch(port)
	{
	case Port::A:
		return GPIOA;
	case Port::B:
		return GPIOB;
	case Port::C:
		return GPIOC;
	case Port::D:
		return GPIOD;
	default:
		return GPIOE;
	}
	return GPIOE;
}

constexpr GPIO_TypeDef * operator&(Port port)
{
	return getPort(port);
}

constexpr GPIO_TypeDef * operator--(Port port)
{
	return getPort(port);
}

constexpr GPIO_TypeDef * operator--(Port port, int)
{
	return getPort(port);
}
constexpr GPIO_TypeDef * operator++(Port port, int)
{
	return getPort(port);
}



#endif /* TEMPLATEPARAMETR_GPIOPARAMETR_HPP_ */
