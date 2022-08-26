package user.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class SimpleConnectionMaker implements ConnectionMaker{
    private static String dbUrl = "jdbc:mysql://localhost/sys?userUnicode=true&serverTimezone=Asia/Seoul&characterEncoding=utf8";
    private static String dbUserId = "root";
    private static String dbUserPwd = "1q2w3e4r!@";
    private static String dbDriverName = "com.mysql.cj.jdbc.Driver";
    
    
	public Connection makeConnection() throws ClassNotFoundException, SQLException{
		  Class.forName(dbDriverName);
		  
	      Connection c = DriverManager.getConnection(dbUrl,dbUserId,dbUserPwd);
	      return c;
	}
	
}
