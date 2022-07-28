#ifndef __TRACKING_H
#define __TRACKING_H 			   
#include "stm32f10x.h"
#include "sys.h"

#define 	F1		ROI[0]
#define 	F2		ROI[1]
#define 	F3		ROI[2]
#define 	F4		ROI[3]
#define 	F5		ROI[4]

extern u8 car_state;
#define	START				0
#define	GOTO_DA			1
#define	AT_DA				2
#define	BACKTO_SA		3
#define	AT_SA				4

#define DIR_SIZE	10
extern u8 Dir_record[DIR_SIZE];
extern u8 Dir_record_1,Dir_record_2,Dir_record_3,Dir_record_4;
extern int cross_cnt;

extern u8 next_Dir;

void tracking_init(void);
void find(void);
void turn_1(void);
void turn_2(void);
void turn_3(void);
void turn_test(void);
void tracking(void);

#endif
