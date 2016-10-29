package bg.elsys.ip.servlet;

import java.util.Arrays;
import java.util.stream.Collectors;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class AuthenticationService {
	public String getSessionIdByRequest(HttpServletRequest request) {
		if (request.getCookies() != null) {
			String sessionId = Arrays.stream(request.getCookies())
					.filter(c -> "SESSION_ID".equals(c.getName()))
					.map(c -> c.getValue())
					.collect(Collectors.toList())
					.get(0);
			
			return sessionId;	
		}
		
		return null;
	}
	
	public void removeSessionId(HttpServletRequest request, HttpServletResponse response, String sessionId) {
		Cookie[] cookies = request.getCookies();
		
		if (cookies != null) {
			for (int i = 0; i < cookies.length; i++) {
				if ("SESSION_ID".equals(cookies[i].getName()) && sessionId.equals(cookies[i].getValue())) {
					cookies[i].setMaxAge(0);
					response.addCookie(cookies[i]);	
				}
			}			
		}
	}
}
