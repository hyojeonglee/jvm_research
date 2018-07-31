public class Case_5 {
	private int _1GB = 1024 * 1024 * 1024;
	public static void main(String [] args) throws InterruptedException {
		boolean case1 = false;
		boolean case0 = false;
		boolean case3 = true;
		boolean case2 = false;
		System.out.println("start");
		Object[] obj = new Object[90];
		for(int i = 0; i < 40; i++) {
	//		System.out.println("sleep");
	//		Thread.sleep(1000 * 10);
			obj[i] = new Object(i);
	//		Object a = new Object();
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
				obj[i] = new Object(i);
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
			for(int loop = 0; loop < 10; loop++) {
				System.out.println("loop " + loop);
				for(int i = 20; i < 70; i++ )
					if(loop % 2 == 0)
						obj[i].reset((byte)0xff);
					else
						obj[i].reset((byte)0x00);
			}

			for(int i = 0; i< 20 ; i++) {
				obj[i].array = null;
				obj[i] = null;
			}

			System.out.println("new ");
			long start = System.currentTimeMillis();
			for(int i = 70; i < 80; i++){
				obj[i] = new Object(i);
				System.out.println(i);
			}
			//	System.gc();
			long end = System.currentTimeMillis();
			System.out.println("time for gc case 2  is " + (end -start) + "ms");

		}
		if(case3 == true) {
			System.out.println("assign 20 nulls");
			for(int i = 30; i< 40; i++) {
				obj[i].array = null;
				obj[i] = null;
			}
			Thread.sleep(1000 * 10);

			for(int loop = 0; loop < 10; loop++) {
				System.out.println("loop " + loop);
				for(int i = 0; i < 30; i++ )
					obj[i].reset((byte)0xff);
			}


			System.out.println("new ");
			long start = System.currentTimeMillis();
			for(int i = 40; i < 50; i++) {
				obj[i] = new Object(i);
				System.out.println(i);
			}
			//	System.gc();
			long end = System.currentTimeMillis();
			System.out.println("time for gc case 3  is " + (end -start) + "ms");

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
