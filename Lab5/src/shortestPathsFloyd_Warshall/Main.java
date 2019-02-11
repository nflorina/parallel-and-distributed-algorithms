import java.util.concurrent.CyclicBarrier;

//package shortestPathsFloyd_Warshall;
/**
 * @author cristian.chilipirea
 *
 */

class MyThread extends Thread {
	private int[][] graph;
	private int thread_id, num_threads;
	public MyThread(int[][] graph, int thread_id, int num_threads) {
		this.graph = graph;
		this.thread_id = thread_id;
		this.num_threads = num_threads;
	}

	@Override
	public void run() {
		int start = thread_id * 5 / num_threads;
		int end = (thread_id + 1) * 5 / num_threads;

		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				for (int k = start; k < end; ++k) {
					graph[j][k] = Math.min(graph[j][k], graph[j][i] + graph[i][k]);
				}
			}
		}
	}
}

public class Main {

	public static void main(String[] args) {
		int M = 9;
		int graph[][] = { { 0, 1, M, M, M }, 
				          { 1, 0, 1, M, M }, 
				          { M, 1, 0, 1, 1 }, 
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };
		
		int p = Runtime.getRuntime().availableProcessors();
		Thread[] threads = new Thread[p];

		for (int i = 0; i < p; ++i) {
			threads[i] = new MyThread(graph, i, p);
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
		
		// Parallelize me (You might want to keep the original code in order to compare)
		/*
		for (int k = 0; k < 5; k++) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
				}
			}
		} */

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}
	}
}
