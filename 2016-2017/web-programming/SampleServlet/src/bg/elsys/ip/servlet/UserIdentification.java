package bg.elsys.ip.servlet;

import java.util.Date;

public class UserIdentification {
	private String ipAddress;
	private Date sessionStarted;

	public UserIdentification() {}
	
	public UserIdentification(String ipAddress, Date sessionStarted) {
		this.ipAddress = ipAddress;
		this.sessionStarted = sessionStarted;
	}
	
	public String getIpAddress() {
		return ipAddress;
	}

	public void setIpAddress(String ipAddress) {
		this.ipAddress = ipAddress;
	}

	public Date getSessionStarted() {
		return sessionStarted;
	}

	public void setSessionStarted(Date sessionStarted) {
		this.sessionStarted = sessionStarted;
	}
}
