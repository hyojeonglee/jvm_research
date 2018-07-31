public class testGCSwap_04 {
	private static final int _1GB = 1024 * 1024 * 1000;
	private static final int stride = 1;
	private static final int limit = 15;
	private static final int access_time = 10;

	public static void main(String[] args) {
		byte arr[][];
		long start, end;

		arr = new byte[100][];
		start = System.currentTimeMillis();
		for (int i = 0 ; i < 40 ; i++) {
			arr[i] = new byte[_1GB];
			System.out.println(i);
		}
		end = System.currentTimeMillis();
		System.out.println("allcation phase 1(ms): " + (end - start));
		System.out.println("###15 obj will be accessed");
		for (int i = 0 ; i < access_time ; i++) {
			start = System.currentTimeMillis();

			for (int j = 0 ; j < 40 ; j++) {
				int k = 0;
				if (j < limit) {
					long st = System.currentTimeMillis();
					while (k < _1GB) {
						arr[j][k] = Byte.MAX_VALUE;
						k += stride;
					}
					long ed = System.currentTimeMillis();
					System.out.println("obj " + j + " time " + (ed-st));
				}
			}
			end = System.currentTimeMillis();
			System.out.println(i + "루프 실행 시간(ms): " + (end - start));
		}
		System.out.println("###10 obj will be accessed (will make swapping)");
		for (int i = 30 ; i < 40 ; i++) {
			int k = 0;
		
			start = System.currentTimeMillis();
			while (k < _1GB) {
				arr[i][k] = Byte.MAX_VALUE;
				k += stride;
			}
			end = System.currentTimeMillis();
			System.out.println("obj " + i + " time " + (end - start));
		}

		// access 0 to 14 and 15 to 29
		System.out.println("###0 to 14(MRU) obj will be accessed");
		start = System.currentTimeMillis();
		for (int i = 0 ; i < 15 ; i++) {
			long st = System.currentTimeMillis();
			for (int j = 0 ; j < _1GB ; j++)
				arr[i][j] = Byte.MIN_VALUE;
			long ed = System.currentTimeMillis();
			System.out.println("obj " + i + " time " + (ed - st));
		}
		end = System.currentTimeMillis();
		System.out.println("Access 0 to 14: " + (end - start));

		System.out.println("###15 to 30(LRU) obj will be accessed");
		start = System.currentTimeMillis();
		for (int i = 15 ; i < 30 ; i++) {
			long st = System.currentTimeMillis();
			for (int j = 0 ; j < _1GB ; j++)
				arr[i][j] = Byte.MIN_VALUE;
			long ed = System.currentTimeMillis();
			System.out.println("obj " + i + "time " + (ed - st));
		}
		end = System.currentTimeMillis();
		System.out.println("Access 15 to 29: " + (end - start));

	}
}
