package org.elsys.postfix;

import java.util.Stack;

public abstract class Operation {
	private Stack<Double> context;
	private String name;
	
	public Operation(String name) {
		this.name = name;
		this.context = null;
	}
	
	public void setContext(Stack<Double> context) {
		this.context = context;
	}
	
	public Stack<Double> getContext() {
		return context;
	}
	
	public String getName() {
		return name;
	}
	
	public abstract void eval();
}
