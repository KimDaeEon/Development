package user.dao;

import java.sql.Connection;
import java.sql.SQLException;

public class CountingConnectionMaker implements ConnectionMaker{
	int counter = 0; // 연결 횟수 측정
	private ConnectionMaker connectionMaker;
	
	CountingConnectionMaker(ConnectionMaker connectionMaker){
		this.connectionMaker = connectionMaker;
	}
	
	@Override
	public Connection makeConnection() throws ClassNotFoundException, SQLException {
		this.counter++;
		Connection c = connectionMaker.makeConnection();
		
		return c;
	}
	
	public int getCounter() {
		return this.counter;
	}
}
