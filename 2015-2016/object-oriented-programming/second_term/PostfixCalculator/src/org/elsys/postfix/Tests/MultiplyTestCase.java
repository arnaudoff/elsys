package org.elsys.postfix.Tests;

import static org.junit.Assert.assertEquals;

import org.elsys.postfix.Multiply;
import org.junit.Test;

public class MultiplyTestCase {
	@Test
	public void testCalculate() {
		Multiply multiplyOperation = new Multiply();
		
		double res = multiplyOperation.calculate(20.0, 2.0);
		
		assertEquals(40.0, res, 0.000001);
	}
}
