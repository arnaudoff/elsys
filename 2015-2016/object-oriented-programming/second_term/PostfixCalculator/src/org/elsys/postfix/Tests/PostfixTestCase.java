package org.elsys.postfix.Tests;

import static org.junit.Assert.assertEquals;

import java.util.Scanner;
import java.util.Stack;

import org.elsys.postfix.Divide;
import org.elsys.postfix.Minus;
import org.elsys.postfix.Multiply;
import org.elsys.postfix.Plus;
import org.elsys.postfix.Postfix;
import org.junit.Before;
import org.junit.Test;

public class PostfixTestCase {
	private Postfix postfix;
	
	@Before
	public void setUp() throws Exception {
		postfix = new Postfix();
		postfix.addOperation(new Plus());
		postfix.addOperation(new Minus());
		postfix.addOperation(new Multiply());
		postfix.addOperation(new Divide());
	}
	
	@Test
	public void testAddition() {
		Scanner scanner = new Scanner("10 20 +");
		postfix.interpret(scanner);
		
		Stack<Double> context = postfix.getContext();
		
		assertEquals(1, context.size());
		assertEquals(30.0, context.pop(), 0.000001);
	}
	
	@Test
	public void testSubstraction() {
		Scanner scanner = new Scanner("50 25 -");
		postfix.interpret(scanner);
		
		Stack<Double> context = postfix.getContext();
		
		assertEquals(1, context.size());
		assertEquals(25.0, context.pop(), 0.000001);
	}
	
	@Test
	public void testMultiplication() {
		Scanner scanner = new Scanner("25 2 *");
		postfix.interpret(scanner);
		
		Stack<Double> context = postfix.getContext();
		
		assertEquals(1, context.size());
		assertEquals(50.0, context.pop(), 0.00001);
	}
	
	@Test
	public void testDivision() {
		Scanner scanner = new Scanner("50 25 /");
		postfix.interpret(scanner);
		
		Stack<Double> context = postfix.getContext();
		
		assertEquals(1, context.size());
		assertEquals(2.0, context.pop(), 0.000001);
	}
	
	@Test(expected=IllegalStateException.class)
	public void testOneOperandBinaryOperation() {
		Scanner scanner = new Scanner("1337 *");
		postfix.interpret(scanner);
	}
	
	@Test(expected=IllegalArgumentException.class)
	public void testDivisionByZero() {
		Scanner scanner = new Scanner("1337 0 /");
		postfix.interpret(scanner);
	}
	
	@Test(expected=IllegalArgumentException.class)
	public void testUnknownOperation() {
		Scanner scanner = new Scanner("1337 0 )");
		postfix.interpret(scanner);
	}
}
