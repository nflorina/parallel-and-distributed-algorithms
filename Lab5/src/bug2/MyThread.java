//package bug2;
/**
 * @author cristian.chilipirea
 * 
 *         Why does this code not block? We took the same lock twice!
 */
public class MyThread implements Runnable {
	static int i;
	private String a = "LOCK";
	private String b = "hr";
	@Override
	public void run() {
		synchronized(a) {
			synchronized (b) {
					i++;
			}
		}
	}
}
