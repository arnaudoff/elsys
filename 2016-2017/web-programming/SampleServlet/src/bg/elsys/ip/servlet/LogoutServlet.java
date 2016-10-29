package bg.elsys.ip.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@WebServlet("/logoutServlet")
public class LogoutServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// Probably a good idea to do that through some real DI.
		AuthenticationService authService = new AuthenticationService();
		
		String sessionId = authService.getSessionIdByRequest(request);
		SessionManager.getInstance().logOut(sessionId);
		authService.removeSessionId(request, response, sessionId);
		
		response.sendRedirect("index.html");
	}
}
