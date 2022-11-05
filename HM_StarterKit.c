/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :2017/6/27	                                       */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :RX631 48P                                             */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "mathf.h"
#include "sci.h"
#include "init.h"
#include "spi.h"
#include "i2c.h"
#include "parameters.h"
#include "glob_var.h"
#include "run.h"
#include "interface.h"
#include "DataFlash.h"
#include "portdef.h"
#include "fast.h"
#include "search.h"

#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


extern void wait_ms(int wtime);
extern void adjust(void);
void battery_disp();
void body_correction();
void config_center();
void setup_sensor();
void setup_fsensor();

void main(void)
{

	init_all();
	unsigned long i = 0;
	
	//ブザー
	BEEP();
	//最初は0しておく
	speed_r=0;
	speed_l=0;
	
	battery_disp();
	
	//起動時のログはとらない
	log_flag = 0;
	short mode = 1;
	while(1){
		I_tar_ang_vel = 0;
		I_ang_vel = 0;
		I_tar_speed = 0;
		I_speed = 0;

		switch(mode){
			
			case 1:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	X	X	*
				*					*
				*****************************************/
				//探索走行
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					LED(0);
					BEEP();
					gyro_get_ref();
					degree = 0;
					timer = 0;
					log_timer = 0;
					len_mouse=0;
					BEEP();
					mypos.x = mypos.y = 0;			//座標を初期化
					mypos.dir = north;			//方角を初期化
					log_flag = 1;
					log_timer = 0;
					body_correction();
					LED(12);
					search_adachi(GOAL_X,GOAL_Y);		//ゴールまで足立法
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//ゴールしたら180度回転する
					mypos.dir = (mypos.dir+6) % 4;		//方角を更新
					map_write();
					BEEP();
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					LED(3);
					search_adachi(0,0);			//スタート地点まで足立法で帰ってくる
					LED(15);
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//帰ってきたら180度回転	
					MOT_POWER_OFF;
					map_write();
					log_flag = 0;
					BEEP();
					LED(0);
				}
				
				break;
				
			case 2:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	X	X	*
				*					*
				*****************************************/	
				//最短走行(スラロームなし)
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					LED(0);
					BEEP();
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					BEEP();
					mypos.x = mypos.y = 0;			//座標を初期化
					mypos.dir = north;			//方角を初期化
					log_flag = 1;
					log_timer = 0;
					len_mouse=0;
					body_correction();
					LED(12);
					fast_run(GOAL_X,GOAL_Y);		//ゴールまで足立法
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//ゴールしたら180度回転する
					mypos.dir = (mypos.dir+6) % 4;		//方角を更新
					map_write();
					BEEP();
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					LED(3);
					search_adachi(0,0);			//スタート地点まで足立法で帰ってくる
					LED(15);
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//帰ってきたら180度回転	
					MOT_POWER_OFF;
					map_write();
					log_flag = 0;
					BEEP();
					LED(0);
				}
				
				break;
				
			case 3:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	X	X	*
				*					*
				*****************************************/
				//最短走行(スラロームあり)
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					LED(0);
					BEEP();
					map_copy();
					degree = 0;
					timer = 0;
					gyro_get_ref();
					BEEP();
					mypos.x = mypos.y = 0;			//座標を初期化
					mypos.dir = north;			//方角を初期化
					log_flag = 1;
					log_timer = 0;
					len_mouse=0;
					body_correction();
					LED(12);
					slalom_run(GOAL_X,GOAL_Y);		//ゴールまで足立法
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//ゴールしたら180度回転する
					mypos.dir = (mypos.dir+6) % 4;		//方角を更新
					map_write();
					BEEP();
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					wait_ms(100);
					BEEP();//ゴールしたことをアピール
					LED(3);
					search_adachi(0,0);			//スタート地点まで足立法で帰ってくる
					LED(15);
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);			//帰ってきたら180度回転	
					MOT_POWER_OFF;
					map_write();
					log_flag = 0;
					BEEP();
					LED(0);
				}
				
				break;
				
			case 4:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	O	X	*
				*					*
				*****************************************/
				
				//左右センサーセットアップ
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					setup_sensor();
					wait_ms(500);
				}
				
				break;
				
			case 5:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	O	X	*
				*					*
				*****************************************/
				
				//前方センサーセットアップ
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					setup_fsensor();
					turn(180,TURN_ACCEL,TURN_SPEED,RIGHT);
					wait_ms(2000);		
				}
				
				break;
				
			case 6:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	O	X	*
				*					*
				*****************************************/
				
				//バッテリー残量表示
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					battery_disp();
					wait_ms(500);
				}
				
				break;
				
			case 7:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	O	X	*
				*					*
				*****************************************/
				
				//スラローム検証用
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					wait_ms(500);
					body_correction();
					straight((SECTION*2)-45,FAST_ACCEL,FAST_SPEED,0.14);
					slalom(90,PI*2*3.3,PI*15,LEFT,0.14);
					straight(0,0,0,0.0);
					wait_ms(500);	
				}
				
				break;
				
			case 8:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	X	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					
					wait_ms(500);
				}
				
				break;
				
			case 9:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	X	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);		
				}
				
				break;
				
			case 10:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	X	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);		
				}

				break;
				
			case 11:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	X	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);			
				}
				
				break;
				
			case 12:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	X	O	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);		
				}
				
				break;
				
				
			case 13:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	X	O	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);		
				}
				
				break;
				
			case 14:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	X	O	O	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();

					wait_ms(500);		
				}
				
				break;
				
			case 15:
				/****************************************
				*MODE LED STATE				*
				*					*
				*	D3	D4	D5	D6	*
				*	O	O	O	O	*
				*					*
				*****************************************/
			
				//センサーの前に手をかざしてスタート
				if(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4){
					BEEP();
					while(sen_fr.value + sen_fl.value + sen_r.value + sen_l.value > SEN_DECISION * 4);
					adjust();
					
					BEEP();
					wait_ms(500);
				}
				
				break;
				
			//mode0~15以外の場合。何もしない。
			default:
				break;
			
		}
		
		//モード切り替え用処理
		if(speed > 0.1){
			if(mode == 15){
				mode = 1;
			}else{
				mode ++;
			}
			for(i = 0; i < 100*1000*10; i++);
			BEEP();
		}
		
		if(speed < -0.1){
			if(mode == 1){
				mode = 15;
			}else{
				mode --;
		}
			for(i = 0; i < 100*1000*10; i++);
			BEEP(); 
		}
		LED(mode);
		
		//プッシュスイッチ用処理
		push_switch = IOex_SWITCH();
		MOT_POWER_OFF;
	
	}
	

}

void battery_disp(){
	float battery_level=(V_bat-3.5)*200;
	LED(0);
	for(int loop=0;loop<3;loop++){
		if(battery_level>=80){
			LED(15);
		}else if(battery_level>=60){
			LED(7);
		}else if(battery_level>=40){
			LED(3);
		}else{
			LED(1);
		}
		wait_ms(200);
		LED(0);
		wait_ms(200);
	}
	return 0;
	
}

void body_correction(){
	turn(90,TURN_ACCEL,TURN_SPEED,RIGHT);			//ゴールしたら180度回転する
	back(-30);                     //30mm back
	degree = 0;
	straight(20,1,0.1,0);                  //20mm 前進
	turn(90,TURN_ACCEL,TURN_SPEED,LEFT);			//ゴールしたら180度回転する
	back(-30);                     //30mm back
	degree = 0;
	straight(20,1,0.1,0);                  //20mm 前進
	return;
}

void config_center(){
	sen_r.ref = sen_r.value;			//右センサの壁有無判断の閾値を初期化
	sen_l.ref = sen_l.value;			//左センサの壁有無判断の閾値を初期化
	return;
}

void setup_fsensor(){
	int tmp;
	int count;
	//壁の閾値
	LED(15);
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_fr.value;
	}
	sen_fr.th_wall=tmp;
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_fl.value;
	}
	sen_fl.th_wall=tmp/100;
	wait_ms(2000);
	return;
}

void setup_sensor(){
	int tmp;
	int count;
	
	//中心においたとき
	LED(6);
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_r.value;
	}
	sen_r.ref=tmp/100;
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_l.value;
	}
	sen_l.ref=tmp/100;
	/*
	//壁の閾値
	LED(15);
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_fr.value;
	}
	sen_fr.th_wall=tmp;
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_fl.value;
	}
	sen_fl.th_wall=tmp/100;
	*/
	//車体を左に寄せた時
	LED(8);
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_r.value;
	}
	sen_r.th_wall=tmp/100;
	
	//車体を右に寄せた時
	LED(1);
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	BEEP();
	wait_ms(500);
	tmp=0;
	for(count=0;count<100;count++){
		wait_ms(1);
		tmp=tmp+sen_l.value;
	}
	sen_l.th_wall=tmp/100;
	
	return;
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
