package org.elsys.postfix.Tests;

import static org.junit.Assert.*;

import java.util.Stack;

import org.elsys.postfix.BinaryOperation;
import org.elsys.postfix.Plus;
import org.junit.Before;
import org.junit.Test;

public class BinaryOperationTestCase {
	private BinaryOperation binaryOperation;
	
	@Before
	public void setUp() throws Exception {
		binaryOperation = new Plus();
	}

	@Test(expected=IllegalStateException.class)
	public void testSingleOperand() {
		Stack<Double> context = new Stack<Double>();
		context.push(2.0);
		
		binaryOperation.setContext(context);
		
		binaryOperation.eval();
	}
	
	@Test
	public void testOperandsPoppedAndResultPushedToContext() {
		Stack<Double> context = new Stack<Double>();
		context.push(2.0);
		context.push(2.0);
		
		binaryOperation.setContext(context);
		binaryOperation.eval();
		
		assertEquals(4.0, binaryOperation.getContext().peek(), 0.00001);
		assertEquals(1, binaryOperation.getContext().size());
	}
}
