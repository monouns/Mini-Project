import java.io.*;

public class StudentInfo implements Serializable{//binary file에 삽입하기 위해 선언
	private String name;
	private char gender;
	private String id;
	private String department;
	private int age;//필드 선언
	
	StudentInfo(){
		
	}//default constructor
	
	StudentInfo(String name, char gender, String id, String department, int age){
		this.name = name;
		this.age = age;
		this.gender = gender;
		this.id = id;
		this.department = department;
	}//필드 선언 constructor
	
	public String toString(){
		return name+"("+age+")     "+gender+"        "+id+"       "+department;
	}//객체를 string으로 반환
	
	//private에 대한 get 함수
	public String getName(){
		return name;
	}
	
	public String getID(){
		return id;
	}
	
	public String getDepartment(){
		return department;
	}
	
	public int getAge(){
		return age;
	}
	
	public char getGender(){
		return gender;
	}
	
	//private에 대한 set 함수
	public void setName(String name){
		this.name = name;
	}
	
	public void setGender(char gender){
		this.gender = gender;
	}
	
	public void setID(String id){
		this.id = id;
	}
	
	public void setAge(int age){
		this.age=age;
	}
	
	public void setDepartment(String department){
		this.department= department;
	}
}
