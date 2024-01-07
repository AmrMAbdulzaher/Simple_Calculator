#ifndef DIO_INTERFACE_H
#define DIO_INTERFACE_H

#define DIO_u8PORTA 		0u
#define DIO_u8PORTB 		1u
#define DIO_u8PORTC 		2u
#define DIO_u8PORTD 		3u

#define DIO_u8PIN0 			0u
#define DIO_u8PIN1 			1u
#define DIO_u8PIN2 			2u
#define DIO_u8PIN3 			3u
#define DIO_u8PIN4 			4u
#define DIO_u8PIN5 			5u
#define DIO_u8PIN6 			6u
#define DIO_u8PIN7 			7u

#define DIO_u8PIN_INPUT 	0u
#define DIO_u8PIN_OUTPUT 	1u

#define DIO_u8PIN_LOW		0u
#define DIO_u8PIN_HIGH		1u

#define DIO_u8PORT_OUTPUT	0xff
#define DIO_u8PORT_INPUT	0x00

uint8 DIO_u8SetPinDirection(uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8Direction);
uint8 DIO_u8SetPinValue(uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8Value);
uint8 DIO_u8TogglePinValue(uint8 Copy_u8Port, uint8 Copy_u8Pin);
uint8 DIO_u8ReadPinValue(uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8* Copy_pu8Value);

uint8 DIO_u8SetPortDirection(uint8 Copy_u8Port, uint8 Copy_u8Direction);
uint8 DIO_u8SetPortValue(uint8 Copy_u8Port, uint8 Copy_u8Value);



#endif
