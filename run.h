void straight(float len, float acc, float max_sp, float end_sp);
void turn(int deg, float ang_acc, float max_om, short dir);
void slalom(int deg, float ang_accel, float max_ang_velocity, short dir,float start_speed);
void back(float len);
extern volatile unsigned int timer;
