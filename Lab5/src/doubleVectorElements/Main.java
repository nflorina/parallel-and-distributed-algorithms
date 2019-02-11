//package doubleVectorElements;
/**
 * @author cristian.chilipirea
 *
 */

class MyThread extends Thread {
	private int[] arr;
	private int size, i, n;
	public MyThread (int[] arr, int size, int i, int n) {
		this.arr = arr;
		this.size = size;
		this.i = i;
		this.n = n;
	}

	@Override
	public void run() {
		int start = i * size / n;
		int end = Math.min((i + 1) * size / n, size);

		for (int x = start; x < end; ++x) {
			arr[x] *= 2;
		}
	}
}

public class Main {

	public static void main(String[] args) {
		int N = 100000013;
		int v[] = new int[N];
		int p = Runtime.getRuntime().availableProcessors();
		Thread[] threads = new Thread[p];

		for(int i=0;i<N;i++)
			v[i]=i;

		for (int i = 0; i < p; ++i) {
			threads[i] = new MyThread(v, N, i, p);
		}

		for (int i = 0; i < p; ++i) {
			threads[i].start();
		}

		for (int i = 0; i < p; ++i) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		// Parallelize me
		/*
		for (int i = 0; i < N; i++) {
			v[i] = v[i] * 2;
		} */

		for (int i = 0; i < N; i++) {
			if(v[i]!= i*2) {
				System.out.println("Wrong answer");
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
