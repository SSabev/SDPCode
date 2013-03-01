import lejos.pc.comm.NXTCommException;


public class DuoNXTCommunicator implements Runnable {

  private Communicator brick1; // brick 1 is RHS brick; ctrls front and rear wheels
	private Communicator brick2; // brick 2 is LHS brick; ctrls left and right wheels
	
	public DuoNXTCommunicator() throws NXTCommException {

		// Create new Communicator objects for each brick
		brick1 = new Communicator("Odysseus", "00:16:53:0a:4c:0d");
		brick2 = new Communicator("NXT", "00:16:53:15:af:97");

		// Establish connection to each brick
		brick1.connect();
		brick2.connect();
		
		// Start listener threads on Communicator objects for each brick
		// UPDATE: don't bother - no need for listening
		//new Thread( brick1 ).start();
		//new Thread( brick2 ).start();

		
	}
	
	public boolean sendPacket(byte[] packet) {
		byte[] packet1 = new byte[]{packet[2], packet[3], packet[4]};
		byte[] packet2 = new byte[]{packet[0], packet[1], packet[4]};
		
		return brick1.sendPacket(packet1) && brick2.sendPacket(packet2);
	}
	
	public void run() {
		while(true) {
			System.out.print(brick1.isTouched() +""+ brick2.isTouched());
			if(brick2.isTouched()) { }
			
			if(brick1.isTouched() || brick2.isTouched()) {

				byte[] stop = new byte[]{(byte) 0, (byte) 0};

				brick1.sendPacket(stop);
				brick2.sendPacket(stop);
				
			}
		}
	}
	
}
