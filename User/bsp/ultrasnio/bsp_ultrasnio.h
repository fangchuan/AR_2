#ifndef  __BSP_ULTRASNIO_H
#define  __BSP_ULTRASNIO_H

#include "stm32f10x.h"
#include "IO_Operation.h"

#define  ULTRASNIO_TRIG        GPIO_Pin_8
#define  ULTRASNIO_ECHO				 GPIO_Pin_9
#define  ULTRASNIO_PORT        GPIOC
#define  NVIC_ULTRASNIO_PP     3
#define  NVIC_ULTRASNIO_SP     3

#define  Ultrasnio_Trigger_H   digitalHi(ULTRASNIO_PORT,ULTRASNIO_TRIG)
#define  Ultrasnio_Trigger_L	 digitalLo(ULTRASNIO_PORT,ULTRASNIO_TRIG)

#define  Ultranio_Echo         GPIO_ReadInputDataBit(ULTRASNIO_PORT , ULTRASNIO_ECHO)

extern void Ultrasnio_Init(void);
extern void Ultrasnio_update(void);
extern void Ultra_Ranging(void);

#endif /*_BSP_ULTRASNIO_H*/

/***************************** �����޿Ƽ� www.apollorobot.cn (END OF FILE) *********************************/
