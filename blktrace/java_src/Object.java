public class Object{
	private int _1GB = 1000 * 1014 * 1024;
	byte[] array;
	private int id;
	public Object(int i) {
		id = i;
		array = new byte[_1GB];
	}
	public void reset(byte i) {
		for(int j = 0; j < _1GB; j++) {
			array[j] = i;
		}
	}
	protected void finalize()
	{
		System.out.println("object " + id +" be finalized ");
	}
}
