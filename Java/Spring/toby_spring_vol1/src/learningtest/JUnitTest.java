package learningtest;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.not;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.CoreMatchers.sameInstance;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;

import java.util.*;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import static org.junit.matchers.JUnitMatchers.hasItem;
import static org.junit.matchers.JUnitMatchers.either;;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations="./junit.xml")
public class JUnitTest {
	@Autowired
	ApplicationContext context;
	
	static Set<JUnitTest> testObjects = new HashSet<JUnitTest>();
	static ApplicationContext previousContext = null;
	
	@Test
	public void test1() {
		assertThat(testObjects, is(not(hasItem(this))));
		testObjects.add(this);
		
		assertThat(previousContext == null || previousContext == this.context, is(true));
		previousContext = this.context;
	}

	@Test
	public void test2() {
		assertThat(testObjects, is(not(hasItem(this))));
		testObjects.add(this);
		
		// assertTrue 방식
		assertTrue(previousContext == null || previousContext == this.context);
		previousContext = this.context;
	}

	@Test
	public void test3() {
		assertThat(testObjects, is(not(hasItem(this))));
		testObjects.add(this);
		
		// assertThat 방식
		assertThat(previousContext, either(is(nullValue())).or(is(this.context)));
		previousContext = this.context;
	}
}
