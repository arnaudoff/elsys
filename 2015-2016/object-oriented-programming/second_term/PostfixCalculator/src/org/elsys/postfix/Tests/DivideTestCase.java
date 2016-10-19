package org.elsys.postfix.Tests;

import static org.junit.Assert.assertEquals;

import org.elsys.postfix.Divide;
import org.junit.Test;

public class DivideTestCase {
	@Test
	public void testCalculate() {
		Divide divideOperation = new Divide();
		
		double res = divideOperation.calculate(20.0, 2.0);
		
		assertEquals(10.0, res, 0.000001);
	}
}