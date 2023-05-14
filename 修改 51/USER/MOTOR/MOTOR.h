#ifndef _MOTOR_H_
#define _MOTOR_H_

/*--------------------------------------------------*/
/*    左前轮      左后轮     右后轮     右前轮      */
/*	 FI / BI     FI / BI     FI / BI    FI / BI     */
/*	P4.5/P4.6	P4.4/P4.3  	P4.2/P4.1  P4.7/P4.0	*/
/*    ------     -------     -------    -------     */
/*   |  JP1 |   | JP2   |   | JP3   |  | JP4   |    */	  
/*    ------     -------     -------    -------     */
/*--------------------------------------------------*/


/*--------------小车灰度传感器布局-----------------*/
/*   _________________________________________     */
/*  |                                         |    */
/*  |          D6 D5 D4 D3 D2 D1 D0           | front*/
/*  |                                         |    */
/*  |                                         |    */
/*  |                                         |    */
/*  |                                         |    */
/*  |                                         |    */
/*  |          D0 D1 D2 D3 D4 D5 D6           | back */
/*  |_________________________________________|    */

extern bdata unsigned char frontPortData,backPortData;
extern idata bit timer_10ms;
extern bdata unsigned int t_sum;
void brake();
void TestIrStatus(void);

void motor_pwm_set(char pwml,char pwmr);
void motor_init();
#endif  /*_MOTOR_H_*/

