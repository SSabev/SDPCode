package main;

import lejos.nxt.Button;
import lejos.nxt.Motor;
import lejos.nxt.SensorPort;
import lejos.nxt.TouchSensor;
import lejos.robotics.navigation.DifferentialPilot;


 public class OtherDrive {
	  DifferentialPilot pilot;
	  TouchSensor bump = new TouchSensor(SensorPort.S1);

	  public void go() {
	    pilot.travel(150, true);
	    while (pilot.isMoving()) {
	      if (bump.isPressed()) pilot.stop();
	    }
	  }

	  public static void main(String[] args) {
	    driveStrightLine traveler = new driveStrightLine();
	    traveler.pilot = new DifferentialPilot(2.25f, 5.5f, Motor.A, Motor.C);
	    traveler.go();
	  }
	}

