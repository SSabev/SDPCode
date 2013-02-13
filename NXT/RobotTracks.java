import java.io.*;

import lejos.nxt.*;
import lejos.nxt.comm.*;
import lejos.util.Delay;

public class RobotTracks implements Runnable {

	private static NXTConnection connection;
	private static DataInputStream dataIn;
	private static DataOutputStream dataOut;
	
	private static boolean listening = true;
	private static boolean connected = false;
	private static boolean reset     = false;
	
	public static void main(String[] args) throws Exception {

		new Thread( new RobotTracks() ).start();

		LCD.drawString("Waiting...",0,0);
		connection = Bluetooth.waitForConnection();
		dataIn = connection.openDataInputStream();
		dataOut = connection.openDataOutputStream();
		
		connected = true;
		Sound.beep();
		
		LCD.clear();
		LCD.drawString("Connected", 0, 0);

		try {
			while (listening) {
				
				byte[] b = new byte[5];
				int bytes_read = dataIn.read(b);
				// TO-DO: Respond to this cmd

				if(bytes_read > 0) {
					
					Motor.A.setSpeed(b[0] * 10);
					if(b[0] == 0) {
						Motor.A.stop();
					} else if (b[0] > 0) {
						Motor.A.forward();
					} else {
						Motor.A.backward();
					}
					
					Motor.C.setSpeed(b[1] * 10);
					if(b[1] == 0) {
						Motor.C.stop();
					} else if (b[1] > 0) {
						Motor.C.forward();
					} else {
						Motor.C.backward();
					}
					
					if(b[4] == 1) {
						reset = true;
						Motor.A.resetTachoCount();
						Motor.C.resetTachoCount();
					}
					
					LCD.drawString("Motor A: " + b[0]*10, 0, 0);
					LCD.drawString("Motor C: " + b[1]*10, 0, 1);
					LCD.drawString("Reset:   " + b[4], 0, 3);

				}
				
			}
		} catch (Exception e) {
			LCD.clear();
			LCD.drawString("EOF: stream empty", 0,0);
			Delay.msDelay(5000);
		}
		
		shutdown(false);
		
	}
	
	public void run() {

		int frequency = 100;
		
		int lastSendA = 0;
		int lastSendC = 0;
		
		while (true) {
			try {
				
				if(Button.ESCAPE.isDown()) {
					LCD.drawString("            ", 0, 6);
					LCD.drawString("Shutting down", 0, 7);
					if(connected) { sendPacket(99, 0); } // send disconnect notice to server					
					Delay.msDelay(1000);
					shutdown(false);
				}
				
				if(connected) {
					int tachoA = Motor.A.getTachoCount();
					int tachoC = Motor.C.getTachoCount();
	
					if(reset) {
						lastSendA = 0;
						lastSendC = 0;
						reset = false;
					}
					
					if ((tachoA - lastSendA) > frequency) {
						lastSendA = tachoA;
						sendPacket(1, tachoA);
						LCD.drawString("Tacho A: " + tachoA, 0, 4);					
					}
					
					if ((tachoC - lastSendC) > frequency) {
						lastSendC = tachoC;
						sendPacket(-1, tachoC);
						LCD.drawString("Tacho C: " + tachoC, 0, 5);
					}
				}
				
				Thread.sleep(50);

			} catch (Exception ex) {
				shutdown(true);
			}
		}
		
	}
	
	private static void shutdown(boolean forced) {
		listening = false;
		LCD.drawString("Disconnecting...",0,0);
		
		if(!forced && connected) {
			try {
				dataIn.close();
				dataOut.close();
				connection.close();
			} catch (Exception e) {
				LCD.drawString("Shutdown exception", 0, 0);
			}
		}
		
		LCD.clear();
		NXT.shutDown();
	}
	
	private void sendPacket(int motor, int tachoCount) throws IOException {
		
		if(motor != 99) {
			motor = motor * tachoCount;
		}
		
		dataOut.writeInt(motor);
		dataOut.flush();
	}
	
	public RobotTracks() {
		// i don't know if this is needed for running the thread
		// cba testing
	}
}
