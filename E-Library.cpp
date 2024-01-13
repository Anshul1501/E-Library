#include <iostream>
#include<fstream>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<ctime>
#include<sstream>
#include<string>
#include<cstring>
#include<map>
#include<mysql.h>
using namespace std;

// ANSI color codes for text color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int users = 0;
string loggedin_user = "";
int curr_user = 0;
int count_attempt = 0;
map <string,string> issued;

void showtime(){
    time_t now = time(0);
    char *date_time = ctime(&now);
   cout<<"\t\t\t\t"<<date_time;
    cout<<endl;
}

//User

class user{
    private:
    string username;
    string password;

    public:
    void adduser(){
 
       //intialize a MySQL connection using MySQL C API
        MYSQL *conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        if (conn) {
    cout <<ANSI_COLOR_GREEN<<"Connected"<<ANSI_COLOR_RESET;

    string username, password;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    // Use prepared statement to prevent SQL injection
    string query = "INSERT INTO users(username, password) VALUES(?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (!stmt) {
        cerr <<ANSI_COLOR_RED<<"Failed to initialize statement" << ANSI_COLOR_RESET;
        mysql_close(conn);
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to prepare statement: " << mysql_error(conn) << ANSI_COLOR_RESET;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING; // type of parameneter 
    bind[0].buffer = (void*)username.c_str(); // points to the memory location where data is stored
    bind[0].buffer_length = username.length();

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)password.c_str();
    bind[1].buffer_length = password.length();

    //error handing for binding of parameter into MYSQL statement 
    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to bind parameters: " << mysql_stmt_error(stmt) << ANSI_COLOR_RESET;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }
   
    if (mysql_stmt_execute(stmt) == 0) {
        cout <<ANSI_COLOR_GREEN<<"Record inserted successfully" << ANSI_COLOR_RESET;
    } else {
        cerr <<ANSI_COLOR_RED<<"Failed to execute query: " << mysql_stmt_error(stmt) << ANSI_COLOR_RESET;
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);

    users++;
}
 else {
    cout << "Not connected" << endl;
        }
    }

    //admin only 

   void no_users(){

            if(loggedin_user!="admin"){
                cout<<ANSI_COLOR_RED<<"\t\t\t\t You are not AUTHORIZED"<<ANSI_COLOR_RESET;
                return;
            }

            cout<<"The name of users: ";

            MYSQL *conn;
            MYSQL_ROW row;      //variable to store the row of data
            MYSQL_RES *res;     //variable to store the result set
            conn = mysql_init(0);
            conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);
           
            if(conn){

                int qstate = mysql_query(conn, "SELECT username FROM users");

                        if(!qstate){            //check if query execution was successful
                            res = mysql_store_result(conn); 
                            int i = 1;          // counter for displaying the row
                            while((row = mysql_fetch_row(res))){
                                cout<<i<<". "<<row[0];      // print the current row
                                i++;
                            }
                        }
             }
    }

    //Check user data against the data in the database to login to the system
     void login(){

                    if(curr_user == 1){
                        cout<<"\t\t\t\t Already Logged in";
                        return;
                    }
                    if(count_attempt == 3){
                        cout<<"\t\t\t\t No more attempts";
                        exit(0);
                    }

            string user_name;
            string pass_word;
            
            cout<<"Username: ";
            cin>>user_name;
            cout<<"Password ";
            cin>>pass_word;
            cin.ignore(); //avoid unexpeded behavious by clearing input buffer
            system("clear"); //clear window

            MYSQL *conn;
            MYSQL_ROW row;
            MYSQL_RES *res;
            conn = mysql_init(0);;
            conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

            if(conn) {
                int qstate = mysql_query(conn, "SELECT username,password FROM users");
                bool flag=false;
                      if(!qstate)
        {
            res = mysql_store_result(conn);
            while((row = mysql_fetch_row(res)))
                {
                if(row[0]==user_name && row[1]==pass_word)
                {
                        loggedin_user = row[0];
                        cout<<ANSI_COLOR_GREEN<<" \t \t \t \t \t \t Login Successful"<<ANSI_COLOR_RESET;
                        cout<<" \t \t \t \t \t \t WELCOME "<< loggedin_user<<endl;
                        curr_user=1;
                        flag=true;
                        break;
                }
                }
                if(!flag)
                {
                    cout<<ANSI_COLOR_RED<<" \t \t \t \t \t \t Incorrect Username or Password"<<ANSI_COLOR_RESET;
                    cout<<" \t \t \t \t \t \t Press Enter and Try again"<<endl;
                    cin.ignore();
                    system("cls");
                    count_attempt++;
                    login();
                }


        }
        else
        {
            cout<<" \t \t \t \t \t No Accounts Registered";
        }

    }
}
	void logout()
	{
		curr_user=0;
	}        
};

//BOOKS

class book : public user{

    long long int book_no;
    string book_name;
    string author_name;

    //Method is for admin use only to add books in Database

    public:
     
     void addbook(){
        if(loggedin_user==""){
            cout<<"\t\t\t\t Login: ";
            return;
        }
        if(loggedin_user!="admin"){
            cout<<ANSI_COLOR_RED<<"\t\t\t\t\t Your are not Authorised, ONLY ADMIN!"<<ANSI_COLOR_RESET;
            return;
        }

        cout<<"Enter Book No \n";
        cin>>book_no;

        cout<<"Enter Book Name \n";
        cin>>book_name;

        cout<<"Enter Author Name \n";
        cin>>author_name;

        //connect to databse 
        MYSQL *conn;
        MYSQL_ROW row;
        MYSQL_RES *res;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        string query = "SELECT BookNo, Book_Name, Author FROM book VALUES(?, ?, ?)";
        MYSQL_STMT *stmt = mysql_stmt_init(conn);
        if (!stmt) {
        cerr <<ANSI_COLOR_RED<<"Failed to initialize statement" << ANSI_COLOR_RESET;
        mysql_close(conn);
        return;
     }

        if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to prepare statement: " << mysql_error(conn) <<ANSI_COLOR_RESET;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG; 
    bind[0].buffer = (void*)book_no; 
    bind[0].buffer_length = 0; //set buffer length to zero

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)book_name.c_str();
    bind[1].buffer_length = book_name.length();

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void*)author_name.c_str();
    bind[2].buffer_length = author_name.length();

    //error handing for binding of parameter into MYSQL statement 
    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to bind parameters: " << mysql_stmt_error(stmt) << ANSI_COLOR_RESET;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }
   
    if (mysql_stmt_execute(stmt) == 0) {
        cout <<ANSI_COLOR_GREEN<<"\t\t\t\t Book Added successfully" << ANSI_COLOR_RESET;
    } else {
        cerr <<ANSI_COLOR_RED<<"Failed to Add Book: " << mysql_stmt_error(stmt) << ANSI_COLOR_RESET;
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
     }
 
     //Search book in Database
     bool searchBook(string searchParam){
        if(loggedin_user == ""){
            cout<<"\t\t\t\t Please Login: ";
            return false;
        }
       
       cout<< "Enter BookNo, Book Name, or Author to search: ";
       cin>>searchParam;

        bool flag = false;
        MYSQL *conn;
        MYSQL_ROW row;
        MYSQL_RES *res;

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        if(conn){
            
            string query = "SELECT BookNo, Book_Name, Author FROM book WHERE BookNo = '" + searchParam + "' OR Book_Name = '" + searchParam + "' OR Author = '" + searchParam + "'";
            int qstate = mysql_query(conn, query.c_str());

            if(!qstate){
                res = mysql_store_result(conn);
                while((row = mysql_fetch_row(res))){
                    
                    cout<<"Book Details: "<<row[0]<<" "<<row[1]<<" "<<row[2];
                    flag = true;
                }
                if(!flag){
                    cout<<ANSI_COLOR_RED<<"Book is not found \n"<<ANSI_COLOR_RESET;
                }
                else {
                    cout<<ANSI_COLOR_GREEN<<"Book is found \n"<<ANSI_COLOR_RESET;
                }
            }
               mysql_free_result(res);
               mysql_close(conn);
        }
        return false;
     }

     //view all the books in the database
      void allbooks(){
        
        MYSQL *conn;
        MYSQL_ROW row;
        MYSQL_RES *res;

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        if(conn){
            
            int qstate = mysql_query(conn, "SELECT BookNo, Book_Name, Author, Issued_By from book");
            if(!qstate){
                res = mysql_store_result(conn);
                while((row=mysql_fetch_row(res))){
                    cout<<"\t\t\t\t"<<row[0]<<"(No)"<<" "<<row[1]<<"(Name)"<<" "<<row[2]<<"(Author)"<<" "<<row[3]<<"(issued by)";
                }
            }
        }
        else {
            cout<<"Failed";
        }
      }

      //update the issue coloum of book in database

     void issue() {
    if (loggedin_user == "") {
        cout << "Please Login";
        return;
    }

    std::string book_no;
    cout << "Enter Book No to be issued \t";
    std::cin >> book_no;  // Corrected variable name

    bool found = searchBook(book_no);

    if (found) {
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        stringstream ss;
        int qstate = 0;

        if (conn) {
            string updateQuery = "UPDATE book SET Issued_By=?";  // Use parameterized query

            MYSQL_STMT* stmt = mysql_stmt_init(conn);
            if (stmt && mysql_stmt_prepare(stmt, updateQuery.c_str(), updateQuery.size()) == 0) {
                MYSQL_BIND bind[1];

                memset(bind, 0, sizeof(bind));

                bind[0].buffer_type = MYSQL_TYPE_STRING;
                bind[0].buffer = (void*)loggedin_user.c_str();
                bind[0].buffer_length = loggedin_user.size();

                mysql_stmt_bind_param(stmt, bind);

                if (mysql_stmt_execute(stmt) == 0) {
                    cout << ANSI_COLOR_GREEN << "\t\t\t\t Book Issued Successfully" << ANSI_COLOR_RESET;
                    cout << "Please return the book within 15 Days, otherwise a fine of Rs.100 will be applied";
                } else {
                    cout << ANSI_COLOR_RED << "Failed" << ANSI_COLOR_RESET;
                }

                issued.insert(make_pair(loggedin_user, book_no));
                return;
            }
        }
    }
    else {
        cout<<ANSI_COLOR_RED<<"Book not found"<<ANSI_COLOR_RESET;
    }
  }

//Return issued Book
void deposit(){

    		map<string ,string>::iterator itr;

		for(itr=issued.begin();itr!=issued.end();itr++)
		{
			if(itr->first==loggedin_user)
			{
			issued.erase(loggedin_user);
			cout<<" \t \t \t \t \t Book Returned Successfully";
			return ;
			}
        }
    }
       // This method is used to print the current user details
	void print_user()
	{   
		 map<string ,string>:: iterator itr;
		 bool flag=false;
				 for(itr=issued.begin();itr!=issued.end();itr++)
				 {
				 	if(itr->first==loggedin_user )
				 	{
					 cout<<" \t \t \t \t \t Current user "<<itr->first<<"   has issued book no:   "<<itr->second;
					 flag=true;
					 }

				}
				if(!flag)
					cout<<" \t \t \t \t \t "<<loggedin_user<<" has issued no book";
				 	cout<<endl;
	}

};

    
  

int main()
{       
        cout<<showtime();

        cout<<"############################################################################## \n";
        cout<<"          **********      Welcome to E-Library     ************ \n";
        cout<<"############################################################################## \n";

        int x;
        int choice = 0;
        user u1; //object for class `user` 
        book b1; // object for class `book`

     do
	{
		cout<<endl;
		cout<<"Menu:"<<endl;
		cout<<"1. Login"<<endl;
		cout<<"2. Register"<<endl;
		cout<<"3. All Books"<<endl;
		cout<<"4. Search Book"<<endl;
		cout<<"5. Issue Book"<<endl;
		cout<<"6. Return Book"<<endl;
		cout<<"7. Get all users"<<endl;
		cout<<"8. Add Book"<<endl;
		cout<<"9. Current user details"<<endl;
		cout<<"10. Logout"<<endl;
		cout<<"0 to exit"<<endl;
		cin>>x;

        	string no2;
	switch(x)
	{
		case 1 : u1.login();
				 break;
		case 2 : u1.adduser();
				 u1.login();
				 break;
		case 3 : b1.allbooks();
				 break;
		case 4 : b1.searchBook(no2);
                 break;
		case 5 : b1.issue();
				 break;
		case 6 : b1.deposit();
				 break;
		case 7 : u1.no_users();
				 break;
		case 8 :  b1.addbook();
				 break;
		case 9 : b1.print_user();
				 break;
		case 10:  u1.logout();
				 break;
		case 0 : choice=1;

	}
	}
	while(choice==0);
 return 0;
}
