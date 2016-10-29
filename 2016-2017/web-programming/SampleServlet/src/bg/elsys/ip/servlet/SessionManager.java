package bg.elsys.ip.servlet;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

// This is an implementation of a thread safe singleton. I don't have experience with Servlets,
// but I suppose each request requires a new thread to be spawned (or reused from a pool) like in the
// other frameworks, so thread safety with the classical singleton implementation might be an issue.

// Source: http://crunchify.com/thread-safe-and-a-fast-singleton-implementation-in-java/

public class SessionManager {
 
	private static SessionManager instance = null;
 
	protected SessionManager() {
		this.sessionMappings = new HashMap<String, UserIdentification>();
		this.activeSessions = new HashSet<String>();
	}
 
	private Map<String, UserIdentification> sessionMappings;
	
	private Set<String> activeSessions;
	
	public static SessionManager getInstance() {
		if (instance == null) {
			synchronized (SessionManager.class) {
				if (instance == null) {
					instance = new SessionManager();
				}
			}
		}
		
		return instance;
	}
	
	public Map<String, UserIdentification> getSessionMappings() {
		return sessionMappings;
	}
	
	public void setSessionIdentification(String sessionId, UserIdentification id) {
		sessionMappings.put(sessionId, id);
	}
	
	public UserIdentification getSessionIdentification(String sessionId) {
		return sessionMappings.get(sessionId);
	}
	
	public Set<String> getActiveSessions() {
		return activeSessions;
	}
	
	public void logIn(String sessionId) {
		activeSessions.add(sessionId);
	}
	
	public void logOut(String sessionId) {
		activeSessions.remove(sessionId);
	}
	
	public boolean isUserLogged(String sessionId) {
		return activeSessions.contains(sessionId);
	}
}
