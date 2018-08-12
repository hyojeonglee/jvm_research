import java.lang.reflect.Field;
import sun.misc.Unsafe;

public class Addresser
{
	private static Unsafe unsafe;
	static
	{
		try
		{
			Field field = Unsafe.class.getDeclaredField("theUnsafe");
			field.setAccessible(true);
			unsafe = (Unsafe)field.get(null);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public static long addressOf(Object o) throws Exception
	{
		Object[] array = new Object[] {o};
		long baseOffset = unsafe.arrayBaseOffset(Object[].class);
		int addressSize = unsafe.addressSize();
		long objectAddress;
		switch (addressSize)
		{
			case 4:
				objectAddress = unsafe.getInt(array, baseOffset);
				break;
			case 8:
				objectAddress = unsafe.getLong(array, baseOffset);
				break;
			default:
				throw new Error("unsupported address size: " + addressSize);
		}
		return (objectAddress);
	}
	
	public static void main(String... args) throws Exception
	{
		System.out.println("start");
	//	byte[] mine = new byte[1024 * 1024 *1024];
	//	byte[] yours = new byte[1024 * 1024 *1024];
		SampleObject[] obj = new SampleObject[90];
		for(int i = 0; i < 70; i++) {
			obj[i] = new SampleObject(i);
			System.out.println(i);
		}
		Thread.sleep(1000 * 10);
	//long address1 = addressOf(mine);
	 // long address2 = addressOf(yours);
	//	System.out.println("address1:" + Long.toHexString(address1));
	//	System.out.println("address2:" + Long.toHexString(address2));
		long address = addressOf(obj);
	    long address2 = addressOf(obj[0].array);
		System.out.println("address:" + Long.toHexString(address));
		System.out.println("address2:" + Long.toHexString(address2));
	}

}
