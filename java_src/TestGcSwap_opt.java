/*
 * Simple test program for swap-aware GC policy.
 */

public class TestGcSwap_opt {
	private int _1GB = 1024 * 1024 * 1024;
	
	public static void main (String[] args) {
		boolean _exec = true;

		System.out.println("[tester] start");
		Object[] obj = new Object[50];

		// Allocate 40 new objects (0 to 9 will be swapped out)
		for (int i = 0; i < 40; i++) {
			obj[i] = new Object();
			System.out.println("[tester] " + i);
		}

		if (_exec == true) {
			// Access 0 to 9 to be swapped in (10 to 19 will be swapped out)
			for (int i = 0; i < 10; i++)
				obj[i].reset(Byte.MAX_VALUE);

			// Make null 0 to 9 (will be sweep target)
			for (int i = 0; i < 10; i++)
			 	obj[i] = null;

			System.out.println("[tester] explicit GC ");
			long start = System.currentTimeMillis();
			
			// Allocate 5 new objects (FGC triggered)
			for (int i = 40; i < 45; i++)
				obj[i] = new Object();
			
			long end = System.currentTimeMillis();
			System.out.println("[tester] time for gc is " + (end - start) + "ms");
		}
	}
}
