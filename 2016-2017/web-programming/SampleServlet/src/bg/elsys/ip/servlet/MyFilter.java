package bg.elsys.ip.servlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;
import java.util.UUID;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class MyFilter implements Filter {

	private Cookie cookie;

	@Override
	public void destroy() {
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
			throws IOException, ServletException {
		PrintWriter out = response.getWriter();

		String username = request.getParameter("name");
		String password = request.getParameter("password");

		if (checkCookie(request)) {
			chain.doFilter(request, response);
		} else if ("admin".equals(username) && "pass".equals(password)) {
			String sessionId = UUID.randomUUID().toString();
			
			Cookie cookie = new Cookie("SESSION_ID", sessionId);
			cookie.setMaxAge(60 * 60 * 24 * 30);
			cookie.setPath("/SampleServlet");
			
			((HttpServletResponse) response).addCookie(cookie);

			SessionManager.getInstance().logIn(sessionId);
			
			UserIdentification userId = new UserIdentification(request.getRemoteAddr(), new Date());
			SessionManager.getInstance().setSessionIdentification(sessionId, userId);
			
			// That's to populate the fields when the (initial) request is missing the Cookie header
			request.setAttribute("userIdentificationIp", userId.getIpAddress());
			request.setAttribute("userIdentificationTimestamp", userId.getSessionStarted().toString());
			
			chain.doFilter(request, response);
		} else {
			out.print("Username or password error!");
			RequestDispatcher dispatcher = request.getRequestDispatcher("index.html");
			dispatcher.forward(request, response);
		}

	}

	@Override
	public void init(FilterConfig arg0) throws ServletException {
	}

	private boolean checkCookie(ServletRequest request) {
		HttpServletRequest req = (HttpServletRequest) request;

		Cookie[] cookies = req.getCookies();
		if (cookies != null) {
			for (Cookie ck : cookies) {
				if ("SESSION_ID".equals(ck.getName()) && SessionManager.getInstance().isUserLogged(ck.getValue())) {
					return true;
				}
			}
		}
		
		return false;
	}

}
