package org.elsys.postfix.Tests;

import static org.junit.Assert.*;

import org.elsys.postfix.Plus;
import org.junit.Test;

public class PlusTestCase {
	@Test
	public void testCalculate() {
		Plus plusOperation = new Plus();
		
		double res = plusOperation.calculate(25.0, 10.0);
		
		assertEquals(35.0, res, 0.000001);
	}
}
