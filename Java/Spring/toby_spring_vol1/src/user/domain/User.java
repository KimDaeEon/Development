package user.domain;

public class User {
    String id;
    String name;
    int level;
    
    public User(String id, String name, int level) {
    	this.id = id;
    	this.name = name;
    	this.level = level;
    }
    
    public User() {
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }
    
    @Override
	public boolean equals(Object obj) {
    	if(obj == this) {
    		return true;
    	}
    	
    	if( !(obj instanceof User) ) {
    		return false;
    	}
    	
    	User user = (User)obj;
    	
    	return this.id.equals(user.getId()) && this.name.equals(user.getName()); 
    }

    @Override
    public String toString() {
        return "id: " + id + " name: " + name + " level: " + level;
    }
}