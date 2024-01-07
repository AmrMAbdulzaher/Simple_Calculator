#include "../../Library/STD_TYPES.h"
#include "../../Library/BIT_MATH.h"
#include "../../Library/ErrType.h"
#include <util/delay.h>

#include "../../MCAL/DIO/DIO_interface.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"

#if CLCD_u8BIT_MODE == FOUR_BIT
static void voidSetHalfDataPort(uint8 Copy_u8Data)
{
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D4_PIN, GET_BIT(Copy_u8Data,0));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D5_PIN, GET_BIT(Copy_u8Data,1));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D6_PIN, GET_BIT(Copy_u8Data,2));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D7_PIN, GET_BIT(Copy_u8Data,3));
}
#endif

static void voidSendEnablePulse(void)
{
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_LOW);
}

void CLCD_voidSendCmd(uint8 Copy_u8Cmd)
{
	/*Set RS pin to low for command*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN, DIO_u8PIN_LOW);

	/*Set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS == DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN, DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/*Send the command*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT, Copy_u8Cmd);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/*send the MSB 4 bits of the command first*/
	voidSetHalfDataPort(Copy_u8Cmd >> 4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Cmd);
	voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration option
#endif
}
void CLCD_voidSendData(uint8 Copy_u8Data)
{
	/*Set RS pin to high for data*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN, DIO_u8PIN_HIGH);

	/*Set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS == DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN, DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/*Send the data*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT, Copy_u8Data);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/*send the MSB 4 bits of the data first*/
	voidSetHalfDataPort(Copy_u8Data >> 4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Data);
	voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration option
#endif
}
void CLCD_voidInit(void)
{
	/*Wait for more than 30 ms after power on*/
	_delay_ms(40);

	/*Function set command: 2 lines, Font size: 5x7*/
#if CLCD_u8BIT_MODE == EIGHT_BIT
	CLCD_voidSendCmd(0b00111000);

#elif CLCD_u8BIT_MODE == FOUR_BIT
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b1000);
	voidSendEnablePulse();
#endif

	/*Display on/off control: Display on, cursor off, blink cursor off*/
	CLCD_voidSendCmd(0b00001100);
	/*Display Clear*/
	CLCD_voidSendCmd(1);
}
void CLCD_voidClear(void)
{
	CLCD_voidSendCmd(1);
}
uint8 CLCD_u8SendString(const char* Copy_pchString)
{
	uint8 Local_u8ErrorState= OK;

	if(Copy_pchString != NULL)
	{
		while(*Copy_pchString != '\0')
		{
			CLCD_voidSendData(*Copy_pchString);
			Copy_pchString++;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}
	return Local_u8ErrorState;
}
uint8 CLCD_u8SendAnimatedString(const char* Copy_pchString)
{
	uint8 Local_u8ErrorState= OK;

	if(Copy_pchString != NULL)
	{
		while(*Copy_pchString != '\0')
		{
			CLCD_voidSendData(*Copy_pchString);
			Copy_pchString++;
			_delay_ms(100);
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}
	return Local_u8ErrorState;
}
void CLCD_voidSendNumber(sint32 Copy_s32Number)
{
	uint8 Local_u8Counter1 = 0u;
	sint8 Local_s8Counter2;
	char Local_achNumber[10];
	uint8 Local_u8RightDigit;

	if(Copy_s32Number <0)
	{
		CLCD_voidSendData('-');

		Copy_s32Number *= -1;
	}
	else if(Copy_s32Number == 0)
	{
		CLCD_voidSendData('0');
		return;
	}

	while(Copy_s32Number != 0)
	{
		Local_u8RightDigit = (uint8)((uint32)Copy_s32Number % 10); /*Get the right most digit*/

		Copy_s32Number = Copy_s32Number / 10; /*remove the right most digit*/

		Local_achNumber[Local_u8Counter1]= Local_u8RightDigit + '0'; /*Convert to ASCII*/

		Local_u8Counter1++;
	}

	for (Local_s8Counter2 = (sint8)Local_u8Counter1-1; Local_s8Counter2 >= 0; Local_s8Counter2--)
	{
		CLCD_voidSendData(Local_achNumber[(uint8)Local_s8Counter2]);
	}
}
void CLCD_voidSendAnimatedNumber(sint32 Copy_s32Number)
{
	uint8 Local_u8Counter1 = 0u;
	sint8 Local_s8Counter2;
	char Local_chNumber[10];
	uint8 Local_u8RightDigit;

	if(Copy_s32Number <0)
	{
		CLCD_voidSendData('-');

		Copy_s32Number *= -1;
	}
	else if(Copy_s32Number == 0)
	{
		CLCD_voidSendData('0');
		return;
	}

	while(Copy_s32Number != 0)
	{
		Local_u8RightDigit = (uint8)((uint32)Copy_s32Number % 10); /*Get the right most digit*/

		Copy_s32Number = Copy_s32Number / 10; /*remove the right most digit*/

		Local_chNumber[Local_u8Counter1]= Local_u8RightDigit + '0'; /*Convert to ASCII*/

		Local_u8Counter1++;
	}

	for (Local_s8Counter2 = (sint8)Local_u8Counter1-1; Local_s8Counter2 >= 0; Local_s8Counter2--)
	{
		CLCD_voidSendData(Local_chNumber[(uint8)Local_s8Counter2]);
		_delay_ms(100);
	}
}

void CLCD_voidGoToXY(uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8Adress;

	if(Copy_u8YPos == 0)
	{
		Local_u8Adress = Copy_u8XPos;
	}
	else if(Copy_u8YPos == 1)
	{
		Local_u8Adress = 0x40 + Copy_u8XPos;
	}

	/*Set bit 7 for DDRAM Address command*/
	SET_BIT(Local_u8Adress, 7);

	/*Execute Set DDRAM Address command*/
	CLCD_voidSendCmd(Local_u8Adress);
}

uint8 CLCD_u8SendSpecialCharacter(uint8 Copy_u8LocationNum, uint8* Copy_pu8Pattern, uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8ErrorState = OK;

	if(Copy_pu8Pattern != NULL)
	{
		uint8 Local_u8Counter;
		uint8 Local_u8GCRAMAdress = Copy_u8LocationNum * 8;

		/*Set bit 6 for CGRAM Address command*/
		SET_BIT(Local_u8GCRAMAdress,6);

		/*Execute Set CGRAM Address command*/
		CLCD_voidSendCmd(Local_u8GCRAMAdress);

		/*Write the input pattern inside CGRAM*/
		for(Local_u8Counter = 0; Local_u8Counter < 8; Local_u8Counter++)
		{
			CLCD_voidSendData(Copy_pu8Pattern[Local_u8Counter]);
		}

		/*Go back to DDRAM*/
		CLCD_voidGoToXY(Copy_u8XPos, Copy_u8YPos);

		/*Display the special pattern inside CGRAM*/
		CLCD_voidSendData(Copy_u8LocationNum);
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}
