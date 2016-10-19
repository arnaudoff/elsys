package org.elsys.postfix;

public class Minus extends BinaryOperation {
	public Minus() {
		super("-");
	}
	
	@Override
	public double calculate(double firstOperand, double secondOperand) {
		return firstOperand - secondOperand;
	}
}
