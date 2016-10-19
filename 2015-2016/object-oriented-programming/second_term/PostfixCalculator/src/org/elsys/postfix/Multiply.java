package org.elsys.postfix;

public class Multiply extends BinaryOperation {
	public Multiply() {
		super("*");
	}
	
	@Override
	public double calculate(double firstOperand, double secondOperand) {
		return firstOperand * secondOperand;
	}
}
