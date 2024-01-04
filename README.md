Console Based Library Management System: 

Operations:  allow users to perform operations such as adding books, removing books, displaying available books, managing users accounts, borrowing/returning books. 

How to Utilise OPPS concepts in c++: 

Classes and objects: Define classes like `Books`, `Library`, `users` to represent real world entity. 

Inheritance and Polymorphism:  Implement inheritance where applicable. For instance you might have different type of books (eg. fiction, non-fiction) inheritance from a base `Book` class.

Encapsulation:  Use access specifier (`public`, `private` and `protected`) to encapsulate data and method within classes

4. Abstraction:  Hide complex implementation details from the user. For instance, you might have functions to borrow/return books without revealing the internal workings.

5. Overloading and Operator Overloading:   Overload operators like +, -, etc., for relevant operations. Overload functions for various purposes within your classes.

6. File Handling: Implement file handling to store book information, user data, borrowing records, etc., persistently.

7. Exception Handling: Implement exception handling to manage errors gracefully, such as when a book is already borrowed, an invalid input is provided, etc.