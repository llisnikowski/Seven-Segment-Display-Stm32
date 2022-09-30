/*!
 * \file gpioParametr.hpp
 * \author by Łukasz Liśnikowski
 */

#ifndef TEMPLATEPARAMETR_GPIOPARAMETR_HPP_
#define TEMPLATEPARAMETR_GPIOPARAMETR_HPP_

#include <stdint.h>
#include <gpio.h>

/*!
 * \brief The enum of ports.
 */
enum class Port : uint8_t
{
	A,
	B,
	C,
	D,
	E
};

/*!
 * \brief Function return GPIO_TypeDef * based on the enum class Port
 * \param port The port number
 * \return The address for the corresponding port
 * \see Port
 * \see operator&(Port port)
 * \see operator--(Port port, int)
 */
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

/*!
 * \brief The equivalent to getPort function
 * \see getPort(Port port)
 * \warning The operator -> has a higher priority than &, so if you want to use these operators,
 * you have to use additional parentheses, e.g. (&Port::A)->BSRR
 */
constexpr GPIO_TypeDef * operator&(Port port)
{
	return getPort(port);
}

/*!
 * \brief The equivalent to getPort function
 * \see getPort(Port port)
 */
constexpr GPIO_TypeDef * operator--(Port port, int)
{
	return getPort(port);
}



#endif /* TEMPLATEPARAMETR_GPIOPARAMETR_HPP_ */
