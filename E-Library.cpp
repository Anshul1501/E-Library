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
#include "/usr/local/mysql/include/mysql.h"
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
    cout <<ANSI_COLOR_GREEN<<"Connected";

    string username, password;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    // Use prepared statement to prevent SQL injection
    string query = "INSERT INTO users(username, password) VALUES(?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (!stmt) {
        cerr <<ANSI_COLOR_RED<<"Failed to initialize statement" << endl;
        mysql_close(conn);
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to prepare statement: " << mysql_error(conn) << endl;
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
        cerr <<ANSI_COLOR_RED<<"Failed to bind parameters: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }
   
    if (mysql_stmt_execute(stmt) == 0) {
        cout <<ANSI_COLOR_GREEN<<"Record inserted successfully" << endl;
    } else {
        cerr <<ANSI_COLOR_RED<<"Failed to execute query: " << mysql_stmt_error(stmt) << endl;
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
                cout<<ANSI_COLOR_RED<<"\t\t\t\t You are not AUTHORIZED";
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
                        mysql_free_result(res);
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

            cout<<"Enter Username and Password: ";
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
                        cout<<ANSI_COLOR_GREEN<<" \t \t \t \t \t \t Login Successful"<<endl;
                        cout<<" \t \t \t \t \t \t WELCOME "<< loggedin_user<<endl;
                        curr_user=1;
                        flag=true;
                        break;
                }
                }
                if(!flag)
                {
                    cout<<ANSI_COLOR_RED<<" \t \t \t \t \t \t Incorrect Username or Password"<<endl;
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
    string deafult_issued = "None";

    //Method is for admin use only to add books in Database
     
     void addbook(){
        if(loggedin_user==""){
            cout<<"\t\t\t\t Login: ";
            return;
        }
        if(loggedin_user!="admin"){
            cout<<ANSI_COLOR_RED<<"\t\t\t\t\t Your are not Authorised, ONLY ADMIN!";
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
        cerr <<ANSI_COLOR_RED<<"Failed to initialize statement" << endl;
        mysql_close(conn);
        return;
     }

        if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        cerr <<ANSI_COLOR_RED<<"Failed to prepare statement: " << mysql_error(conn) << endl;
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
        cerr <<ANSI_COLOR_RED<<"Failed to bind parameters: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }
   
    if (mysql_stmt_execute(stmt) == 0) {
        cout <<ANSI_COLOR_GREEN<<"\t\t\t\t Book Added successfully" << endl;
    } else {
        cerr <<ANSI_COLOR_RED<<"Failed to Add Book: " << mysql_stmt_error(stmt) << endl;
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
                    cout<<ANSI_COLOR_RED<<"Book is not found \n";
                }
                else {
                    cout<<ANSI_COLOR_GREEN<<"Book is found \n";
                }
            }
               mysql_free_result(res);
               mysql_close(conn);
        }
     }
     
};




int main()
{

        cout<<"############################################################################## \n";
        cout<<"          **********      Welcome to E-Library     ************ \n";
        cout<<"############################################################################## \n";


    return 0;
}