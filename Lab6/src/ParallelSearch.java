class Structure {
    int start, end, x, k, g;
	int[] c;
	int[] j;
	int[] numbers;
	int N;
	
	public Structure (int N, int start, int end, int x, int k, int g, int[] c, int[] j, int[] numbers) {
		this.start = start;
		this.end = end;
		this.x = x;
		this.k = k;
		this.g = g;
		this.c = c;
		this.j = j;
		
		this.numbers = numbers;
	
		this.N = N;
	}

}

class MyThread implements Runnable {
    int i;
	Structure a;

	public MyThread (int i, Structure a) {
		this.i = i;
		this.a = a;
	}
	
	@Override
	public void run() {
		
		while (a.start <= a.end && a.k == -1) {
			a.j[i] = (int) ((a.start - 1) + i * (Math.pow(a.N, a.g - 1)));
			
			if (a.j[i] <= a.end) {
				if (a.numbers[a.j[i]] == a.x){
					a.k = a.j[i];
				} else if (a.numbers[a.j[i]] > a.x) {
					a.c[i] = 0;
				} else
					a.c[i] = 1;
			} else {
				a.j[i] = a.end + 1;
				a.c[i] = 0;
			}
			
			if (a.c[i] != a.c[i - 1]) {
				a.end = a.j[i] - 1;
				if (i > 1)
					a.end = a.j[i - 1] + 1;
			}
			
			if (i == 1) {
				if (a.c[i] != a.c[i + 1])
					a.start = Math.max(a.j[i] + 1, 5);
				
				a.g--;
			}
		}
	}

}

public class ParallelSearch {

    public static final int N = 6;

    public static void main (String[] args) {
        Thread threads[] = new Thread[N];
		
		int[] numbers = {0, 1, 2, 3, 10, 24, 123};
		int[] c = new int[N + 1];
		int[] j = new int[N];
		
		
		c[0] = 1;
		c[N] = 0;
		
		int g = (int) Math.round(Math.log(7) / Math.log(N + 1));
		Structure a = new Structure(N, 0, 6, 5, -1, g, c, j, numbers);

		for (int i = 0; i < N; i++) 
			threads[i] = new Thread(new MyThread(i + 1, a));
		
		for (int i = 0; i < N; i++)
			threads[i].start();
		
		for (int i = 0; i < N; i++) {
            try {
			    threads[i].join();                
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
		
		System.out.println(a.k);

    }
}