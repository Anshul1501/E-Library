# E-Library Management System

## Overview

This E-Library Management System is a C++ application that allows users to manage a digital library. The system includes functionalities for adding users, adding books, issuing books, returning books, and viewing user details.

## Technologies Used

### XAMPP: Used as the backend server for MySQL database.
### MySQL C API: Integrated with the MySQL C API to interact with the MySQL database.
### C++: The main programming language used for the application.

## Getting Started

### Install XAMPP: 
Download and install [XAMPP] (https://www.apachefriends.org/index.html)

### MySQL Database Setup: 
Create a MySQL database named "library" using phpMyAdmin or any MySQL client.

### Update Library Path (Temporary Solution): 
When compiling your C++ program, ensure that you link it with the MySQL connector library:

```console 
g++ -o E-Library E-Library.cpp -I/usr/local/mysql/include -L/usr/local/mysql/lib -lmysqlclient -lmysqlcppconn
```
Run the compiled executable: