public class TestGcSwap {
	private int _1GB = 1024 * 1024 * 1024;
	public static void main(String [] args) {
		boolean case0 = false;
		boolean case1 = false;
		boolean case2 = true;
		boolean case3 = false;

		System.out.println("start");
		Object[] obj = new Object[50];
		for(int i = 0; i < 40; i++) {
			obj[i] = new Object();
			System.out.println(i);
		}
		if(case0 == true) {
			for(int loop = 0; loop < 10; loop++) {
				long start = System.currentTimeMillis();
				for(int i = 0; i < 70; i++) {
					obj[i].reset((byte)0xff);
				}
				long end = System.currentTimeMillis();
				System.out.println("time for lop: " + loop + " is " + (end -start) + "ms");
			}
		}
		if(case1 == true) {
			for(int j = 0; j < 30; j++) {
				for(int loop = 0; loop < 10; loop++) {
					System.out.println("reset "+j);
					if(loop % 2 == 0)
						obj[j].reset((byte)0x00);	
					else
						obj[j].reset((byte)0xff);	
				}
			}

			for(int i = 51; i < 70; i++) {
				obj[i] = new Object();
				System.out.println(i);
			}

			for(int i = 0; i < 50; i++) {
				long start = System.currentTimeMillis();
				obj[i].reset((byte)0x00);
				long end = System.currentTimeMillis();
				System.out.println("time for accesing object " + i + " is " + (end -start) + "ms");
			}
		}	
		if(case2 == true) {
/*
 * for JVM cr_print test
			for(int loop = 0; loop < 10; loop++) {
				System.out.println("loop " + loop);
				for(int i = 0; i < 30; i++ )
					obj[i].reset((byte)0xff);
			}
*/
			for(int i = 0; i< 10; i++)
				obj[i] = null;

			System.out.println("new ");
			long start = System.currentTimeMillis();
			for(int i = 40; i < 45; i++)
				obj[i] = new Object();
			long end = System.currentTimeMillis();
			System.out.println("time for gc case 1  is " + (end -start) + "ms");
			
		}
		if(case3 == true) {
			for(int loop = 0; loop < 10; loop++) {
				System.out.println("loop " + loop);
				for(int i = 0; i < 30; i++ )
					obj[i].reset((byte)0xff);
			}
			for(int i = 30; i< 40; i++)
				obj[i] = null;

			System.out.println("new ");
			long start = System.currentTimeMillis();
			for(int i = 40; i < 45; i++)
				obj[i] = new Object();
			long end = System.currentTimeMillis();
			System.out.println("time for gc case 2  is " + (end -start) + "ms");

		}
		// default case
		//if (i <= 19)
		//	arr[i] = null;

		// GC before swap out
		/*
		   if(i <= 19)
		   arr[i] = null;
		   if(i == 19)
		   System.gc();
		   */
		// GC after swap out
		/*
		   if(i >= 50 && i < 70)
		   arr[i-50] = null;
		   if(i == 69)
		   System.gc();

	}
	*/
}
}
