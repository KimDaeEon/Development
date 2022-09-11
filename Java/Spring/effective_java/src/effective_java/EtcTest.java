package effective_java;

public class EtcTest {

	static void foo(Integer a) {
		a = 40;
	}
	public static void main(String[] args) {
		Integer b = new Integer(0);
		foo(b);
		
		System.out.println(b);
	}

}
