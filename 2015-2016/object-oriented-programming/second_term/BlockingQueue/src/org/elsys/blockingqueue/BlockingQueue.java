package org.elsys.blockingqueue;

import java.util.ArrayList;
import java.util.List;

public class BlockingQueue<T> {
	private List<T> elements;
	private int size;
	
	public BlockingQueue(int size) {
		elements = new ArrayList<>(size);
		this.size = size;
	}
	
	public synchronized void put(T element) {
		while (elements.size() == size) {
			try {
				System.out.println("The queue is full, waiting..");
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		elements.add(element);
		System.out.println("New element added: " + element);
		notifyAll();
	}
	
	public synchronized T get() {
		while (elements.isEmpty()) {
			try {
				System.out.println("The queue is empty, waiting..");
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		T element = elements.remove(elements.size() - 1);
		System.out.println("Fetched a new element: " + element);
		notifyAll();
		
		return element;
	}
}
