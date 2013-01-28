package main;

import lejos.nxt.LCD;
import lejos.nxt.Motor;
import lejos.nxt.SensorPort;
import lejos.nxt.TouchSensor;
import lejos.util.Delay;

public class KickerProgram {


		/**
		 * @param args
		 */
		public static void main(String[] args) {
			// TODO Auto-generated method stub
			LCD.drawString("Kicker program", 0, 0);
			TouchSensor kick_rdy = new TouchSensor(SensorPort.S1);
			Motor.A.setAcceleration(500);
			Motor.C.setAcceleration(500);
			Motor.A.setSpeed(500);
			Motor.C.setSpeed(500);
			Motor.B.setSpeed(50);
			Motor.B.backward();
			while (!kick_rdy.isPressed()){
			}
			Motor.B.resetTachoCount();
			Motor.B.setSpeed(1000);
			Motor.A.forward();
			Motor.C.forward();
			Delay.msDelay(500);
			Motor.B.forward();
			Motor.A.setAcceleration(10000);
			Motor.C.setAcceleration(10000);
			Delay.msDelay(100);
			Motor.B.rotateTo(30, true);
			Motor.C.stop(true);
			Motor.A.stop(true);
			Delay.msDelay(1000);
		}

	}
