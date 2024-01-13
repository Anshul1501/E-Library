# E-Library Management System

## Overview

This E-Library Management System is a C++ application that allows users to manage a digital library. The system includes functionalities for adding users, adding books, issuing books, returning books, and viewing user details.

## Technologies Used

### XAMPP:
 Used as the backend server for MySQL database.
### MySQL C API: 
Integrated with the MySQL C API to interact with the MySQL database.
### C++: 
The main programming language used for the application.

## Getting Started

### Install XAMPP: 
[Install XAMPP](https://www.apachefriends.org/index.html)

### MySQL Database Setup: 
Create a MySQL database named "library" using phpMyAdmin or any MySQL client.

### Update Library Path (Temporary Solution): 
When compiling your C++ program, ensure that you link it with the MySQL connector library:

```console 
g++ -o E-Library E-Library.cpp -I/usr/local/mysql/include -L/usr/local/mysql/lib -lmysqlclient -lmysqlcppconn
```
Run the compiled executable:

```console 
export DYLD_LIBRARY_PATH=/usr/local/mysql/lib:$DYLD_LIBRARY_PATH
./E-Library
```
### Update Library Path (Permanent Solution): 

Use the install_name_tool to update the library path directly in your executable:

```console 
 install_name_tool -change @rpath/libmysqlclient.22.dylib /path/to/libmysqlclient.22.dylib E-Library
./E-Library
```
## Features
Add users to the library system.
Add books to the digital collection.
Issue books to users.
Return books and manage fines.
View details of all users (admin access only).

## Contributing
Feel free to contribute to the project by submitting bug reports or feature requests.