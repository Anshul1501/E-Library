#include <iostream>
#include<fstream>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<ctime>
#include<sstream>
#include<string>
#include<cstring>
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

        MYSQL *conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);
        
        if(conn){
            cout<<"connected";
        }
        else {
            cout<<"not connected";
        }
        cout<<"Username: ";
        cin>>username;

        cout<<"Password: ";
        cin>>password;

        int qstate= 0;  // query state 
        stringstream ss;
        ss<<"INSERT INTO users(username, password) VALUES("<<username<<", "<<password<<")";
        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

        if(qstate==0){
            cout<<" \t\t\t\t Record inserted successfully";
        }
        else {
            cout<<"Failed";
        }
        users++;
    }

//admin only 
void no_users(){

    if(loggedin_user!="admin"){
        cout<<"\t\t\t\t Not Authorized. Only admin can view all users details";
        return;
    }

    cout<<"The users are: ";

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res; // Declare MYSQL_RES* for the result set

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

    if (conn) {
        int qstate = mysql_query(conn, "SELECT username FROM users");

        if (!qstate) {
            res = mysql_store_result(conn);

            if (res) {
                int i = 1;
                while ((row = mysql_fetch_row(res))) {
                    cout<< i << ": " << row[0] ;
                    i++;
                }
                mysql_free_result(res); // Free the result set
            } else {
                cout<< "Error retrieving results: " << mysql_error(conn) ;
            }
        } else {
            cout<< "Error in query execution: " << mysql_error(conn) ;
        }
    } else {
        cout<< "Connection is not established!" ;
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