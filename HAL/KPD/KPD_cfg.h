#ifndef KPD_CFG_H
#define KPD_CFG_H

#define KPD_u8COL_PORT			DIO_u8PORTD
#define KPD_u8ROW_PORT			DIO_u8PORTB

#define KPD_u8COL1_PIN			DIO_u8PIN2
#define KPD_u8COL2_PIN			DIO_u8PIN3
#define KPD_u8COL3_PIN			DIO_u8PIN4
#define KPD_u8COL4_PIN			DIO_u8PIN5

#define KPD_u8ROW1_PIN			DIO_u8PIN4
#define KPD_u8ROW2_PIN          DIO_u8PIN5
#define KPD_u8ROW3_PIN          DIO_u8PIN6
#define KPD_u8ROW4_PIN			DIO_u8PIN7

#define KPD_au8_BUTTON_ARR		{{7,8,9,'/'},{4,5,6,'*'},{1,2,3,'-'},{'!',0,'=','+'}};

#define KPD_u8NOPRESSED_KEY_VAL 0xff

#endif
