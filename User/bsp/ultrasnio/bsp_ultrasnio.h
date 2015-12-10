#ifndef  __BSP_ULTRASNIO_H
#define  __BSP_ULTRASNIO_H

#include "stm32f10x.h"
#include "IO_Operation.h"

#define  ULTRASNIO_TRIG        GPIO_Pin_8
#define  ULTRASNIO_ECHO				 GPIO_Pin_9
#define  ULTRASNIO_PORT        GPIOC

#define  Ultrasnio_Trigger_H   digitalHi(ULTRASNIO_PORT,ULTRASNIO_TRIG)
#define  Ultrasnio_Trigger_L	 digitalLo(ULTRASNIO_PORT,ULTRASNIO_TRIG)



extern void Ultrasnio_Init(void);
extern void Ultrasnio_update(void);

#endif /*_BSP_ULTRASNIO_H*/
