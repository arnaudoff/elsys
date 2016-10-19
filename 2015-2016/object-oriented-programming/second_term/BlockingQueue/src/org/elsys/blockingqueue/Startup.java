package org.elsys.blockingqueue;

import java.util.ArrayList;
import java.util.List;

public class Startup {
	
	public static void main(String[] args) {
		BlockingQueue<Double> queue = new BlockingQueue<>(3);
		List<Thread> threads = new ArrayList<Thread>();
		
		for (int i = 0; i < 5; i++) {
			Runnable consumerRunnable = new Runnable() {
				@Override
				public void run() {
					for (int i = 0; i < 1000; i++) {
						queue.get();
					}
				}
			};
			
			Thread consumerThread = new Thread(consumerRunnable);
			threads.add(consumerThread);
			
			Runnable producerRunnable = new Runnable() {
				@Override
				public void run() {
					for (int i = 0; i < 1000; i++) {
						queue.put(Double.valueOf(Math.random()));
					}
				}
			};
			
			Thread producerThread = new Thread(producerRunnable);
			threads.add(producerThread);
		}
		
		for (Thread thread : threads) {
			thread.start();
		}
	}
}
