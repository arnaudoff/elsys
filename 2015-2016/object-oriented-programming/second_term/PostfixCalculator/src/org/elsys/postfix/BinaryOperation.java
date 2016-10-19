package org.elsys.postfix;

public abstract class BinaryOperation extends Operation {
	public BinaryOperation(String name) {
		super(name);
	}
	
	@Override
	public void eval() {
		if (super.getContext().size() < 2) {
			throw new IllegalStateException("A binary operation requires at least two operands.");
		}
		
		double firstOperand = super.getContext().pop();
		double secondOperand = super.getContext().pop();
		
		double res = this.calculate(secondOperand, firstOperand);
		System.out.println("R: " + res);
		getContext().push(res);
	}
	
	public abstract double calculate(double firstOperand, double secondOperand);
}
