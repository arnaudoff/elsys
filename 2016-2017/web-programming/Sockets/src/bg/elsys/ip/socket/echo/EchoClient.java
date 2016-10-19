package bg.elsys.ip.socket.echo;

import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class EchoClient {
	// Create constants with the host name and port that we are going to connect to.
	private static final String HOST_NAME = "localhost";
	private static final int PORT = 31114;

	public static void main(String[] args) throws UnknownHostException, IOException {
		/*
		 * Try with resources to open the socket and get the input and output streams
		 * of the socket, as well as the standard input stream.
		 */
		try (Socket echoSocket = new Socket(HOST_NAME, PORT);
				PrintStream out = new PrintStream(echoSocket.getOutputStream());
				Scanner in = new Scanner(echoSocket.getInputStream());
				Scanner stdIn = new Scanner(System.in)) {
			String userInput;
			/*
			 * While the standard input has next line, we send the userInput to
			 * the output stream of the socket. We then print what we receive as the 
			 * input of the socket.
			 */
			while((userInput = stdIn.nextLine()) != null) {
				out.println(userInput);
				System.out.println(in.nextLine());
			}
		}
	}

}
