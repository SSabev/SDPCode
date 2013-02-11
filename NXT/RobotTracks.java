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
	
	public static void main(String[] args) throws Exception {

		LCD.drawString("Waiting...",0,0);
		connection = Bluetooth.waitForConnection();
		connected = true;
		
		LCD.clear();
		LCD.drawString("Connected", 0, 0);
		
		dataIn = connection.openDataInputStream();
		dataOut = connection.openDataOutputStream();

		new Thread( new RobotTracks() ).start();
	
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
					
					LCD.drawString("Motor A: " + b[0]*10, 0, 0);
					LCD.drawString("Motor C: " + b[1]*10, 0, 1);
					LCD.drawString("Kick:    " + b[4], 0, 3);

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
		// Feedback byte signals
		byte disconnect = (byte) 99;
		byte sensorData = (byte) 1;
		
		TouchSensor sensorA = new TouchSensor(SensorPort.S1);
		TouchSensor sensorB = new TouchSensor(SensorPort.S2);

		boolean reacting = false;
		
		while (true) {
			try {
				if(connected) {
					boolean touchA = sensorA.isPressed();
					boolean touchB = sensorB.isPressed();
	
					byte sensorAval = touchA ? (byte) 1 : (byte) 0;
					byte sensorBval = touchB ? (byte) 1 : (byte) 0;
					
					if (!reacting && (touchA || touchB)) {
						// If sensor A or B are pressed, activate start reacting
						reacting = true;
						Sound.beep();
						LCD.drawString("Sensor A: " +  sensorAval, 0, 6);
						LCD.drawString("Sensor B: " +  sensorBval, 0, 7);
						sendPacket(sensorData, sensorAval, sensorBval);
						
					} else if (reacting && !(touchA || touchB)) {
						// If we're reacting, but no sensors are pressed, stop reacting
						reacting = false;
						
						LCD.drawString("Sensor A: 0", 0, 6);
						LCD.drawString("Sensor B: 0", 0, 7);
						sendPacket(sensorData, sensorAval, sensorBval);
					}
				}
				
				if(Button.ESCAPE.isDown()) {
					LCD.drawString("            ", 0, 6);
					LCD.drawString("Shutting down", 0, 7);
					if(connected) { sendPacket(disconnect, (byte) 0, (byte) 0); }					
					Delay.msDelay(1000);
					shutdown(false);
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
	
	private void sendPacket(byte opcode, byte val1, byte val2) throws IOException {
		byte[] packet = new byte[] { opcode, val1, val2 };
		dataOut.write(packet, 0, 3);
		dataOut.flush();
	}
	
	public RobotTracks() {
		// i don't know if this is needed for running the thread
		// cba testing
	}
}
