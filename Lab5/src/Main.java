class MyThread extends Thread {
    @Override
    public void run() {
        System.out.println("Muie Decebal din thread-ul " + Thread.currentThread().getId());
    }
}

public class Main {
    public static void main (String[] args) {
        for (int i = 0; i < Runtime.getRuntime().availableProcessors(); i++) {
			MyThread h = new MyThread();
			h.start();
		}
    }
}