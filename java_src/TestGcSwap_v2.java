public class TestGcSwap_v2 {
	private int _1GB = 1024 * 1024 * 1024;
	static private int beg = 0;
	static private int limit = 9;	
	
	public static void main(String [] args) {
		boolean case2 = true;
		
		System.out.println("[hjlee] start");	
		Object[] obj = new Object[50];
		for(int i = 0; i < 40; i++) {
			obj[i] = new Object();
			System.out.println("[hjlee] Allocate " + i);
		}
		if(case2 == true) {
			// Access specific portion
			System.out.println("[hjlee] Access " + beg + " to " + limit); 
			for (int i = beg; i <= limit; i++) {
				obj[i].reset(Byte.MAX_VALUE);
			}

			// trigger GC!
			System.out.println("[hjlee] Make NULL object 0 to 9");
			for(int i = 10; i< 19; i++)
				obj[i] = null;

			System.out.println("[hjlee] New ");
			long start = System.currentTimeMillis();
			for(int i = 40; i < 45; i++)
			 	obj[i] = new Object();
			long end = System.currentTimeMillis();
			System.out.println("time for gc case 1  is " + (end -start) + "ms");
		}
	}
}
