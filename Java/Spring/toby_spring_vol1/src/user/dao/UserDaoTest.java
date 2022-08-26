package user.dao;

import org.junit.Test;
import org.junit.Before;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

import java.sql.SQLException;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.GenericXmlApplicationContext;
import org.springframework.dao.EmptyResultDataAccessException;

import user.domain.User;

public class UserDaoTest {
	UserDao userDao;
	User user1;
	User user2;
	User user3;
	
	@Before
	public void setUp() {
		ApplicationContext context = new GenericXmlApplicationContext("applicationContext.xml");
//		ApplicationContext context = new AnnotationConfigApplicationContext(DaoFactory.class);
		userDao = context.getBean("userDao", UserDao.class);
		user1 = new User("qwer1", "David1", 1);
		user2= new User("qwer2", "David2", 1);
		user3 = new User("qwer3", "David3", 1);
		
	}

	@Test
	public void addAndGet() throws SQLException {

		userDao.deleteAll();
		assertThat(userDao.getCount(), is(0));

		userDao.add(user1);
		assertThat(userDao.getCount(), is(1));

		userDao.add(user2);
		assertThat(userDao.getCount(), is(2));
		
		userDao.add(user3);
		assertThat(userDao.getCount(), is(3));

//		System.out.println(user + " 입력 완료");

		User userGet1 = userDao.get(user1.getId());
		User userGet2 = userDao.get(user2.getId());
		User userGet3 = userDao.get(user3.getId());

		assertThat(user1, is(userGet1));
		assertThat(user2, is(userGet2));
		assertThat(user3, is(userGet3));
	}
	
	@Test(expected = EmptyResultDataAccessException.class)
	public void getUserFailure() throws SQLException{
		userDao.deleteAll();
		assertThat(userDao.getCount(), is(0));
		
		userDao.get("unknown_id");
	}
}
