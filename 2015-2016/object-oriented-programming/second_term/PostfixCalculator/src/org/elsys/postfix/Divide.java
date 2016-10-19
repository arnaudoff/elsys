package org.elsys.postfix;

public class Divide extends BinaryOperation {
	public Divide() {
		super("/");
	}
	
	@Override
	public double calculate(double firstOperand, double secondOperand) {
		if (secondOperand == 0.0) {
			throw new IllegalArgumentException("Cannot divide by zero.");			
		}
		
		return firstOperand / secondOperand;
	}
}