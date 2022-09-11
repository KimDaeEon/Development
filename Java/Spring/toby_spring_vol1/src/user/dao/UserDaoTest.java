package user.dao;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.Before;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

import java.sql.SQLException;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.datasource.SingleConnectionDataSource;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import user.domain.User;

@RunWith(SpringJUnit4ClassRunner.class)	
@ContextConfiguration(locations="/test-applicationContext.xml")
//@DirtiesContext 테스트용 context를 만들어서 DirtiesContext 제거
public class UserDaoTest {
	@Autowired
	private ApplicationContext context; // 생성은 되는데 아래처럼 userDao를 직접 @Autowired 하면 의미가 없을 수 있다.
	
	@Autowired
	UserDao userDao;
	
	User user1;
	User user2;
	User user3;
	
	@Before
	public void setUp() {
		user1 = new User("qwer1", "David1", 1);
		user2= new User("qwer2", "David2", 1);
		user3 = new User("qwer3", "David3", 1);
		
		DataSource dataSource = new SingleConnectionDataSource(
				"jdbc:mysql://localhost/sys?useUnicode=true&serverTimezone=Asia/Seoul&characterEncoding=utf8",
				"root",
				"1q2w3e4r!@",
				true);
		
		userDao.setDataSource(dataSource);
	}
	
	@Test(expected = EmptyResultDataAccessException.class)
	public void getUserFailure() throws SQLException{
		userDao.deleteAll();
		assertThat(userDao.getCount(), is(0));
		
		userDao.get("unknown_id");
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
	
	
	@Test
	public void isSameDao() {
		// Bean으로 가져온 객체가 Autowired 로 만들어진 객체와 같은지 확인
		UserDao dao = context.getBean("userDao", UserDao.class);
//		UserDao dao = context.getBean(UserDao.class);
		
		assertThat(userDao, is(dao));
	}
}
