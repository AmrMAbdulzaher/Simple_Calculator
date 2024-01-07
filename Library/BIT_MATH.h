#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(REG, BIT_NUM) ((REG)|=(1U << (BIT_NUM)))
#define CLR_BIT(REG, BIT_NUM) ((REG)&=(~(1U << (BIT_NUM))))
#define TOG_BIT(REG, BIT_NUM) ((REG)^=(1U << (BIT_NUM)))
#define GET_BIT(REG, BIT_NUM) (((REG)>>(BIT_NUM)) & 1U)

#endif
