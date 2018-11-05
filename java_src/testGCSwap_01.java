public class testGCSwap_01 {
	private static final int _1GB = 1024 * 1024 * 1000;
	static int caseNum = 4;

	public static void main(String[] args) {
		byte arr[][];

		arr = new byte[100][];
		for (int i = 0 ; i < 40 ; i++) {
			arr[i] = new byte[_1GB];

			System.out.println(i);
			
			switch (caseNum) {
				// default case (no null obj)
				case 1:
					break;
				// default case (null obj 10)
				case 2:
					if (i <= 9)
						arr[i] = null;
					break;
				// GC before swap
				case 3:
					if (i <= 9)
						arr[i] = null;
					if (i == 9)
						System.gc();
					break;
				// GC after swap
				case 4:
					if (i >= 30 && i < 40)
						arr[i - 30] = null;
					if (i == 40)
						System.gc();
					break;
			}
		}
	}
}
