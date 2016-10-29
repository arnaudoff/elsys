package bg.elsys.ip.servlet;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class AdminServlet extends HttpServlet {

	private static final long serialVersionUID = 1126486565983113076L;

	// I added doGet to allow testing AdminServlet route through the URL, not only when login form is submitted
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		populateView(request, response);
		
		getServletContext().getRequestDispatcher("/WEB-INF/admin.jsp")
			.forward(request, response);
	}
	
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		populateView(request, response);
		
		getServletContext().getRequestDispatcher("/WEB-INF/admin.jsp")
			.forward(request, response);
	}
	
	private List<User> getUsers() {
		List<User> users = new ArrayList<>();
		users.add(new User(1, "admin", "admin@admin.bg"));
		users.add(new User(2, "user", "user@gmail.com"));
		return users;
	}
	
	private void populateView(HttpServletRequest request, HttpServletResponse response) {
		response.setContentType("text/html");

		request.setAttribute("users", getUsers());
		
		if (request.getAttribute("userIdentificationIp") == null ||
				request.getAttribute("userIdentificationTimestamp") == null) {
			UserIdentification id = getUserIdentification(request);
			request.setAttribute("userIdentificationIp", id.getIpAddress());
			request.setAttribute("userIdentificationTimestamp", id.getSessionStarted().toString());
		}
	}
	
	private UserIdentification getUserIdentification(HttpServletRequest request) {
		// Probably a good idea to do that through some real DI.
		AuthenticationService authService = new AuthenticationService();
		
		return SessionManager.getInstance().getSessionIdentification(authService.getSessionIdByRequest(request));
	}

}
