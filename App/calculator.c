#include "../Library/STD_TYPES.h"
#include <util/delay.h>

#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/PORT/PORT_interface.h"
#include "../HAL/CLCD/CLCD_interface.h"
#include "../HAL/KPD/KPD_interface.h"
#include "calculator.h"

void main(void)
{
	PORT_voidInit();
	CLCD_voidInit();

	uint8 Local_u8KPDKey=0xff;
	uint8 Local_u8InputCounter=0;
	sint16 Local_s16FirstNumber=0;
	sint16 Local_s16SecondNumber=0;
	sint32 Local_s32ResultIntegerPart=0;
	sint32 Local_s32ResultFractionPart=0;
	uint8 Local_u8Operator=0;
	uint8 Local_u8DivisionByZero=0;

	while(1)
	{
		/*Store digits in the first number variable*/
		while(Local_u8InputCounter != MAX_CHAR_COUNT || Local_u8KPDKey != '!' || Local_u8KPDKey != '=' || Local_u8KPDKey != '/' || Local_u8KPDKey != '+' || Local_u8KPDKey != '*' || Local_u8KPDKey != '-')
		{
			if(Local_u8InputCounter == 0)
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == 0xff || Local_u8KPDKey == '=' || Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-');
			}
			else
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == 0xff);
			}

			if(Local_u8InputCounter > 0)
			{
				if(Local_u8KPDKey == '=')
				{
					break;
				}

				if(Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-')
				{
					CLCD_voidSendData(Local_u8KPDKey);
					Local_u8Operator = Local_u8KPDKey;
					Local_u8InputCounter++;
					break;
				}
			}

			if(Local_u8KPDKey == '!')
			{
				break;
			}

			CLCD_voidSendNumber(Local_u8KPDKey);
			Local_s16FirstNumber=(sint16)((Local_s16FirstNumber*10U)+(sint16)Local_u8KPDKey);

			Local_u8InputCounter++;
		}

		/* Store FirstNumber value in Result Just in Case '=' is pressed early */
		Local_s32ResultIntegerPart = (sint32)Local_s16FirstNumber;

		/*Store digits in the second number variable if operator is chosen*/
		if(Local_u8Operator != 0)
		{
			while(Local_u8InputCounter != MAX_CHAR_COUNT || Local_u8KPDKey != '!' || Local_u8KPDKey != '=' || Local_u8KPDKey != '/' || Local_u8KPDKey != '+' || Local_u8KPDKey != '*' || Local_u8KPDKey != '-')
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == 0xff || Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-');

				if(Local_u8KPDKey == '!' || Local_u8KPDKey == '=')
				{
					break;
				}

				CLCD_voidSendNumber(Local_u8KPDKey);
				Local_s16SecondNumber=(sint16)((Local_s16SecondNumber*10U)+(sint16)Local_u8KPDKey);
				Local_u8InputCounter++;
			}
		}

		/* Perform the operation */
		switch(Local_u8Operator)
		{
		case '+':	Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber + (sint32)Local_s16SecondNumber);	break;
		case '-': 	Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber - (sint32)Local_s16SecondNumber);	break;
		case '/':
		{
			/* Disallow dividing by zero */
			if(Local_s16SecondNumber == 0)
			{
				Local_u8DivisionByZero = 1;
				CLCD_voidClear();
				CLCD_u8SendString("CAN'T / BY ZERO!");
				_delay_ms(2000);
			}
			else
			{
				Local_s32ResultFractionPart= (sint32)((((sint32)Local_s16FirstNumber % (sint32)Local_s16SecondNumber)*100L)/(sint32)Local_s16SecondNumber);
				Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber / (sint32)Local_s16SecondNumber);
			}
			break;
		}
		case '*': Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber * (sint32)Local_s16SecondNumber);	break;
		}
		/* If result valid then show it */
		if(Local_u8DivisionByZero == 0)
		{
			CLCD_voidGoToXY(0,1);
			CLCD_voidSendNumber(Local_s32ResultIntegerPart);
			if(Local_s32ResultFractionPart > 0)
			{
				CLCD_voidSendData('.');
				CLCD_voidSendNumber(Local_s32ResultFractionPart);
			}
		}
		/* Disable all buttons except clear Button */
		while(Local_u8KPDKey != '!')
		{
			Local_u8KPDKey = KPD_u8GetPressedKey();
		}

		/*Reset calculator*/
		CLCD_voidClear();
		Local_s16SecondNumber=0;
		Local_u8DivisionByZero=0;
		Local_s16FirstNumber=0;
		Local_u8InputCounter=0;
		Local_s32ResultIntegerPart=0;
		Local_s32ResultFractionPart=0;
		Local_u8Operator=0;
		Local_u8KPDKey= 0xff;
	}
}
