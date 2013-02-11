#if !defined(MOTOR_H)
#define MOTOR_H

/*  
 * Motor represents each of our robot motors.
 */
class Motor
{
public:
	Motor();
	Motor(float speed, float distance);
	void SetSpeed(int x);
	void SetWheelRotation(float x);
	void SetTime(int x);
	int GetSpeed();
	int GetTime();
	float GetWheelRotation();

	private:
	int speed;
	float wheelRotation;
	int time;
};

#endif
