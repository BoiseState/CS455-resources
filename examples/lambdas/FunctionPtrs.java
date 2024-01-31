import java.util.Arrays;
import java.util.function.Function;
import java.util.stream.IntStream;

class FunctionClass {
	int[] doubleElements(int[] myArray)
	{
		// for (int i = 0; i < myArray.length; i++)
		// myArray[i] *= 2;
		Arrays.setAll(myArray, i -> myArray[i] *= 2);
		IntStream.range(0, myArray.length).forEach(i -> myArray[i] *= 2);
		return myArray;
	}


	int[] zeroElements(int[] myArray)
	{
		// for (int i = 0; i < myArray.length; i++)
		// myArray[i] = 0;
		Arrays.setAll(myArray, i -> 0);
		IntStream.range(0, myArray.length).forEach(i -> myArray[i] = 0);

		return myArray;
	}
}

class FunctionPointers {
	public static void main(String[] args)
	{
		FunctionClass manyFunctions = new FunctionClass();
		test(manyFunctions::doubleElements);// Notice the missing "()" and arguments
		test(manyFunctions::zeroElements);
		// for static functions, we can refer as FunctionClass::staticMethod
	}


	private static void test(Function<int[], int[]> someFunction)
	{
		int n = 10;
		int[] A = new int[n];
		for (int i = 0; i < A.length; i++) {
			A[i] = i;
		}
		int[] result = someFunction.apply(A);
		for (int x : result)
			System.out.print(x + " ");
		System.out.println();
	}
}
