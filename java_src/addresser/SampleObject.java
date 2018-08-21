public class SampleObject{
	private int _1GB = 1000 * 1014 * 1024;
	byte[] array;
	int id;
	public SampleObject(int i) {
		id = i;
		array = new byte[_1GB];
	}
	public void reset(byte i) {
		for(int j = 0; j < _1GB; j++) {
			array[j] = i;
		}
	}
	public int read_id() {
//		System.out.println(id);
		return id;
	}
}
