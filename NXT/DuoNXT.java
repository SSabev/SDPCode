import java.io.*;

import lejos.nxt.*;
import lejos.nxt.comm.*;
import lejos.util.Delay;

public class DuoNXT implements Runnable {

  private static NXTConnection connection;
	private static DataInputStream dataIn;
	private static DataOutputStream dataOut;
	
	private static boolean listening = true;
	private static boolean connected = false;
	
	public static void main(String[] args) throws Exception {

		new Thread( new DuoNXT() ).start();

		LCD.drawString("Waiting...",0,0);
		connection = Bluetooth.waitForConnection();
		connected = true;
		Sound.beep();
		
		LCD.clear();
		LCD.drawString("Connected", 0, 0);
		
		dataIn = connection.openDataInputStream();
		dataOut = connection.openDataOutputStream();


		try {
			while (listening) {
				
				byte[] b = new byte[3];
				int bytes_read = dataIn.read(b);
				// TO-DO: Respond to this cmd


				/* We read in two bytes from the Communications server
				 * The first byte is the left motor value (that is, MOTOR B)
				 * The second byte is the right motor value (that is, MOTOR C)
				 */
				if(bytes_read > 0) {
					
					int left_speed = (b[0] & 0x7F) * 10;
					int right_speed = (b[1] & 0x7F) * 10;
					
					int left_dir = b[0] & 0x80;
					int right_dir = b[1] & 0x80;
					
					LCD.drawString("L speed: " + left_speed + "     ", 0, 0);
					LCD.drawString("R speed: " + right_speed + "     ", 0, 1);
					LCD.drawString("L direc: " + left_dir + "     ", 0, 2);
					LCD.drawString("R direc: " + right_dir + "     ", 0, 3);
					
					if(b[2] == (byte) 1) {
						Motor.A.setSpeed(1000);
						Motor.A.rotate(50);
						Motor.A.rotateTo(0);
					}
					
					if(left_dir == 0) {
						Motor.B.setSpeed(left_speed);
						Motor.B.forward();
					} else {
						Motor.B.setSpeed(left_speed);
						Motor.B.backward();
					}
					
					if(right_dir == 0) {
						Motor.C.setSpeed(right_speed);
						Motor.C.forward();
					} else {
						Motor.C.setSpeed(right_speed);
						Motor.C.backward();
					}
				}
				
			}
		} catch (Exception e) {
			LCD.clear();
			LCD.drawString("EOF: stream empty", 0,0);
			Delay.msDelay(5000);
		}
		
		/* If we reach here, then the program is terminating for some reason
		 * (most likely the CANCEL button has been pressed on the brick
		 */
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
				
				/* Because this thread is running all the time, we can check to
				 * see if the cancel button has been pressed at any time.
				 */
				if(Button.ESCAPE.isDown()) {
					LCD.drawString("            ", 0, 6);
					LCD.drawString("Shutting down", 0, 7);
					if(connected) { sendPacket(disconnect, (byte) 0, (byte) 0); }					
					Delay.msDelay(1000);
					shutdown(false);
				}
				
				/* Only try to send feedback data if we're connected
				 */
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
						//sendPacket(sensorData, sensorAval, sensorBval);
						Motor.A.rotateTo(0);
						Motor.B.stop();
						Motor.C.stop();
						
					} else if (reacting && !(touchA || touchB)) {
						// If we're reacting, but no sensors are pressed, stop reacting
						reacting = false;
						
						LCD.drawString("Sensor A: 0", 0, 6);
						LCD.drawString("Sensor B: 0", 0, 7);
						//sendPacket(sensorData, sensorAval, sensorBval);
					}
				}
				
				/* Let's not go overboard yo.
				 */
				Thread.sleep(50);

			} catch (Exception ex) {
				shutdown(true);
			}
		}
		
	}
	
	private static void shutdown(boolean forced) {
		listening = false;
		LCD.drawString("Disconnecting...",0,0);
		
		/* If 'forced' is true, then something's gone wrong with the communications
		 * and so there's no point trying to tell the PC that we're shutting down
		 *
		 * If 'forced' is false, then (most likely) the CANCEL button has been
		 * pressed on the robot.
		 */
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
	
	public DuoNXT() {
		// i don't know if this is needed for running the thread
		// cba testing
	}
}
