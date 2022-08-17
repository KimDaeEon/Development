package user.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.mysql.cj.protocol.Resultset;

import user.domain.User;

public class UserDao {
	private static String dbUrl = "jdbc:mysql://localhost/sys?userUnicode=true&serverTimezone=Asia/Seoul&characterEncoding=utf8";
	private static String dbUserId = "root";
	private static String dbUserPwd = "1q2w3e4r!@";
	private static String dbDriverName = "com.mysql.cj.jdbc.Driver";
	
	public void add(User user) throws ClassNotFoundException, SQLException{
		Class.forName(dbDriverName);
		Connection c = DriverManager.getConnection(dbUrl,dbUserId,dbUserPwd);
		
		PreparedStatement ps = c.prepareStatement(
				"insert into user(id, name, level) values(?,?,?)");
		
		ps.setString(1, user.getId());
		ps.setString(2, user.getName());
		ps.setInt(3, user.getLevel());
		
		ps.executeUpdate();
		
		ps.close();
		c.close();
	}
	
	public User get(String id) throws ClassNotFoundException, SQLException {
		Class.forName(dbDriverName);
		Connection c = DriverManager.getConnection(dbUrl,dbUserId,dbUserPwd);
		
		PreparedStatement ps = c.prepareStatement("select * from user where id = ?");
		ps.setString(1, id);
		
		ResultSet rs = ps.executeQuery();
		rs.next();
		
		User user = new User();
		user.setId(rs.getString("id"));
		user.setName(rs.getString("name"));
		user.setLevel(rs.getInt("level"));
		
		rs.close();
		ps.close();
		c.close();
		
		return user;
	}
	
	public static void main(String[] args) throws ClassNotFoundException, SQLException {
		UserDao userDao = new UserDao();
		
		User user = new User();
		user.setId("kde11235");
		user.setName("김대언");
		user.setLevel(99);
		
//		userDao.add(user);
		
		System.out.println(user + " 등록 성공");
		
		user = userDao.get("kde11235");
		
		System.out.println(user + " 조회 성공");
	}
}
