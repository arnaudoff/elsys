package org.elsys.postfix;

public class Plus extends BinaryOperation {
	public Plus() {
		super("+");
	}
	
	@Override
	public double calculate(double firstOperand, double secondOperand) {
		return firstOperand + secondOperand;
	}
}
