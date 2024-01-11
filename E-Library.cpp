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
 
       //intialize a MySQL connection using MySQL C API
        MYSQL *conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0, NULL, 0);

        if (conn) {
    cout << "Connected" << endl;

    string username, password;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    // Use prepared statement to prevent SQL injection
    string query = "INSERT INTO users(username, password) VALUES(?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (!stmt) {
        cerr << "Failed to initialize statement" << endl;
        mysql_close(conn);
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
        cerr << "Failed to prepare statement: " << mysql_error(conn) << endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)username.c_str();
    bind[0].buffer_length = username.length();

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)password.c_str();
    bind[1].buffer_length = password.length();

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr << "Failed to bind parameters: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0) {
        cout << "Record inserted successfully" << endl;
    } else {
        cerr << "Failed to execute query: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);

    users++;
} else {
    cout << "Not connected" << endl;
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