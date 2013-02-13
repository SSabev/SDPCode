package main;
import lejos.nxt.Motor;
import lejos.nxt.LCD;


public class MainDrive {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		LCD.drawString("Milestone 1 Drive In A Straight Line", 0, 0);
		Motor.C.setAcceleration(100);
		Motor.A.setAcceleration(105);
		Motor.C.setSpeed(500); 
		Motor.A.setSpeed(509); 
		Motor.C.rotateTo(7450, true);
		Motor.A.rotateTo(7550);
	}

}
