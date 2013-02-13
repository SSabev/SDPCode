import lejos.pc.comm.*;

import java.io.*;
import java.util.Arrays;

public class Communicator implements Runnable {

	private lejos.pc.comm.NXTComm connection;
	private NXTInfo robot;
	private DataOutputStream dataOut;
	private DataInputStream dataIn;
	private boolean connected;
	
	private int tachoA;
	private int tachoC;
	
	public Communicator() throws NXTCommException {
		this.connected = false;
		this.connection = NXTCommFactory.createNXTComm(NXTCommFactory.BLUETOOTH);
		this.robot = new NXTInfo(NXTCommFactory.BLUETOOTH, "Odysseus", "00:16:53:0a:4c:0d");
	}
	
	public void connect() {
		System.out.println("Connecting to robot...");
		
		boolean suppress = false;
		
		while(!connected) {
			try {
				connection.open(robot);
				dataIn = new DataInputStream(connection.getInputStream());
				dataOut = new DataOutputStream(connection.getOutputStream());
				System.out.println("Connection to robot established.");
				connected = true;
			} catch (NXTCommException error) {
				connected = false;
				if(!suppress) {
					System.err.println("Connection failure: " + error.getMessage());
					System.out.println("Retrying connection");
					suppress = true;
				} 
			}
		}
	}
	
	public void disconnect() {
		System.out.println("Disconnecting from robot...");
		
		try {
			dataIn.close();
			dataOut.close();
			connection.close();
			connected = false;
		} catch (IOException error) {
			System.err.println("Disconnect failure: " + error.getMessage());
		}
	}
	
	public boolean isConnected() {
		return connected;
	}

	
	/* ** Do not use **
	   This method is defunct. The NXJ brick will not handle int commands

	public int sendRawInt(int i) throws IOException {
		System.out.println("Sending message: " + i);
		
		dataOut.writeInt(i);
		dataOut.flush();
		
		return dataIn.readInt();
	}
	*/
	
	public boolean sendPacket(byte[] packet) {
		if(connected) {
			System.out.println("Sending packet: " + Arrays.toString(packet));
		
			try {
				dataOut.write(packet, 0, 5);
				dataOut.flush();
				
				return true;
			} catch (IOException error) {
				System.err.println("Transmission failure: " + error.getMessage());
				connected = false;
				return false;
			}
		} else {
			System.err.println("Can't send packet: robot is disconnected!");
			connect();
			return false;
		}
	}
	
	public void run() {

		while (true) {
			if(connected) {
				/* do nothing; needed for autorecovery after robot disconnects (somehow?!) */
			}
			
			while(connected) {
				try {	
					int tachoCount = dataIn.readInt();
			
					if(tachoCount == 99) {
						connected = false;
						
						System.out.println("Robot invoked shutdown sequence");
					} else if(tachoCount > 0) {
						tachoA = tachoCount;
						System.out.println("Tacho count data received: motor A=" + tachoA);
					} else {
						tachoC = tachoCount * (-1);
						System.out.println("Tacho count data received: motor C=" + tachoC);
					}
				
					Thread.sleep(50);
				} catch (Exception e) {
					System.err.println(e.getMessage());
				
					// Set 'connected' to false, and try to reconnect
					// THIS COULD BE PROBLEMATIC! ******************************************
					// When robot ungracefully disconnects, system is unaware until timeout.
					// Once time-out occurs, I'd assume we are disconnected
					// However, an exception could be raised if the input stream genuinely can't
					// be read for some reason other than disconnection..?
					connected = false;
					
					connect();
				}
			}

		}
	
	}

}
