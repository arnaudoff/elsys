<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>

<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Admin JSP</title>
</head>
<body>
	<p>IP Address you used during login: ${userIdentificationIp}</p>
	<p>Login timestamp: ${userIdentificationTimestamp}</p>
	<table>
		<tr>
			<th>ID</th>
			<th>Username</th>
			<th>Email</th>
		</tr>
		<c:forEach var="user" items="${users}">
			<tr>
				<td>${user.id}</td>
				<td>${user.name}</td>
				<td>${user.email}</td>
				<td><a href="userServlet?name=${user.name}">Go to user page</a></td>
			</tr>
		</c:forEach>
	</table>
	<form action="logoutServlet">
	    <input type="submit" value="logout" />
	</form>
</body>
</html>