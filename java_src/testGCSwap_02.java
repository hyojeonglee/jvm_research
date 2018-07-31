public class testGCSwap_02 {
	private static final int _1GB = 1024 * 1024 * 1000;
	private static final int stride = 1;
	private static final boolean case3 = true;
	private static final int limit = 30;
	private static final boolean gcBeforeSwap = false;

	public static void main(String[] args) {
		byte arr[][];
		long start, end;

		arr = new byte[100][];
		for (int i = 0 ; i < 40 ; i++) {
			arr[i] = new byte[_1GB];
			System.out.println(i);
		}

		// make null 10 obj that will be swapped out
		if (case3) {
			for (int i = 30 ; i < 40 ; i++)
				arr[i] = null;
		}
		
		// access 0 to 30 in 10 times
		for (int i = 0 ; i < 10 ; i++) {
			if (i == 0 && gcBeforeSwap == true) {
				System.out.println("GC before swap!\n");
				long gc_s = System.currentTimeMillis();
				System.gc();
				long gc_e = System.currentTimeMillis();
				System.out.println("GC time: " + (gc_e-gc_s));
			}
			if (i == 1 && gcBeforeSwap == false) {
				System.out.println("GC after swap!\n");
				long gc_s = System.currentTimeMillis();
				System.gc();
				long gc_e = System.currentTimeMillis();
				System.out.println("GC time: " + (gc_e-gc_s));
			}
			start = System.currentTimeMillis();
			for (int j = 0 ; j < 40 ; j++) {
				int k = 0;
				//long st = System.currentTimeMillis();
				if (j < limit) {
					while (k < _1GB) {
						arr[j][k] = Byte.MAX_VALUE;
						k += stride;
					}
				}
				//long ed = System.currentTimeMillis();
				//System.out.println("obj " + j + " time " + (ed-st));
			}
			end = System.currentTimeMillis();
			System.out.println(i + "루프 실행 시간(ms): " + (end - start));
		}
		/*
		// test gc priority 
		// make null 10 obj in memory
		for (int i = 0 ; i < 10 ; i++)
			arr[i] = null;

		start = System.currentTimeMillis();
		// make gc (5 to 15 obj)	
		for (int i = 40 ; i < 45 ; i++) {
			arr[i] = new byte[_1GB];
			System.out.println(i);
		}
		end = System.currentTimeMillis();
		System.out.println("###GC 소요 시간 " + (end-start));
		*/

		// test swap
		/*
		System.out.println("10 to 29 obj");
		for (int i = 10 ; i < 30 ; i++) {
			int k = 0;

			start = System.currentTimeMillis();
			while (k < _1GB) {
				arr[i][k] = Byte.MAX_VALUE;
				k+= stride;
			}

			end = System.currentTimeMillis();
			System.out.println("obj " + i + " time " + (end-start));
		}

		System.out.println("0 to 10 obj");
		for (int i = 0 ; i < 10 ; i++) {
			int k = 0;

			start = System.currentTimeMillis();
			while (k < _1GB) {
				arr[i][k] = Byte.MAX_VALUE;
				k+= stride;
			}

			end = System.currentTimeMillis();
			System.out.println("obj " + i + " time " + (end-start));
		}

		System.out.println("30 to 39 obj");
		for (int i = 30 ; i < 40 ; i++) {
			int k = 0;

			start = System.currentTimeMillis();
			while (k < _1GB) {
				arr[i][k] = Byte.MAX_VALUE;
				k+= stride;
			}

			end = System.currentTimeMillis();
			System.out.println("obj " + i + " time " + (end-start));
		}

		*/

		
	}
}
