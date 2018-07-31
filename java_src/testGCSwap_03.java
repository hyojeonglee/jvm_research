public class testGCSwap_03 {
	private static final int _1GB = 1024 * 1024 * 1000;
	private static final int stride = 1000;
	private static final int limit = 10;

	public static void main(String[] args) {
		byte arr[][];
		long start, end;

		arr = new byte[100][];
		start = System.currentTimeMillis();
		for (int i = 0 ; i < 30 ; i++) {
			arr[i] = new byte[_1GB];
			System.out.println(i);
		}
		end = System.currentTimeMillis();
		System.out.println("allcation phase 1(ms): " + (end - start));

		for (int i = 0 ; i < 10 ; i++) {
			start = System.currentTimeMillis();

			for (int j = 0 ; j < 30 ; j++) {
				int k = 0;
				if (j < limit) {
					while (k < _1GB) {
						arr[j][k] = Byte.MAX_VALUE;
						k += stride;
					}
				}
			}
			end = System.currentTimeMillis();
			System.out.println(i + "루프 실행 시간(ms): " + (end - start));
		}

		start = System.currentTimeMillis();
		for (int i = 30 ; i < 40 ; i++) {
			arr[i] = new byte[_1GB];
			System.out.println(i);
		}
		end = System.currentTimeMillis();
		System.out.println("allcation phase 1(ms): " + (end - start));
	}
}
