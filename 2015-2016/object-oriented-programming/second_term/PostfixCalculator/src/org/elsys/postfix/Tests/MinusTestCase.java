package org.elsys.postfix.Tests;

import static org.junit.Assert.assertEquals;

import org.elsys.postfix.Minus;
import org.junit.Test;

public class MinusTestCase {
	@Test
	public void testCalculate() {
		Minus minusOperation = new Minus();
		
		double res = minusOperation.calculate(25.0, 10.0);
		
		assertEquals(15.0, res, 0.000001);
	}
}
