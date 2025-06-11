#include <iostream>
#include <string>
#include <fstream>
using namespace std;
const int MAX_BOOKS = 100000;     // Maximum number of books
const int MAX_CUSTOMERS = 100000; // Maximum number of customers that can be resistered
const int MAX_ISSUED_BOOKS = 5;   // Maximum number of books a customer can borrow at a time
struct Book
{
    string name;
    int ISBN;
    string author;
};
struct Customer
{
    string name;
    int id;
};
struct IssuedBook
{ // a structure for books issued by customers
    string book_name;
    int borrowerID;
};
IssuedBook issuedBooks[MAX_ISSUED_BOOKS]; // Array to store issued books
int numIssuedBooks = 0;
Book books[MAX_BOOKS]; // it is an array of book structure
int numBooks = 100000;
Customer customers[MAX_CUSTOMERS];
int numCustomers = 100000;
void clearScreen()
{
    system("cls");
}
void writeIssuedBooksToFile()
{
    // clearScreen();
    ofstream file("issued_books.txt");
    if (file.is_open())
    {
        for (int i = 0; i < numIssuedBooks; ++i)
        {
            file << issuedBooks[i].book_name << "," << issuedBooks[i].borrowerID << endl;
        }
        file.close();
    }
    else
    {
        cout << "Unable to open issued_books.txt file to write issued books." << endl;
    }
}
void addBook()
{
    clearScreen();
    cout << "Enter book name: ";
    cin.ignore();
    getline(cin, books[numBooks].name);
    cout << "Enter ISBN number: ";
    cin >> books[numBooks].ISBN;
    cin.ignore();
    cout << "Enter author name: ";
    getline(cin, books[numBooks].author);
    numBooks++;
    cout << "Book added successfully!\n";
    // Save the book to file
    ofstream file("books.txt", ios::app); // file named books.txt to store books
    if (file.is_open())
    {
        file << books[numBooks - 1].name << "," << books[numBooks - 1].ISBN << "," << books[numBooks - 1].author << endl;
        file.close();
        cout << "Book saved to file successfully.\n";
    }
    else
    {
        cout << "Unable to open file to save book.\n";
    }
}
void displayBooks()
{
    clearScreen();
    ifstream file("books.txt");
    if (file.is_open())
    {
        int count = 0;
        string line;
        while (getline(file, line))
        {
            string name, ISBN, author;
            size_t pos = 0;
            pos = line.find(',');
            name = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(',');
            ISBN = line.substr(0, pos);
            line.erase(0, pos + 1);
            author = line;
            cout << "Book name: " << name << endl;
            cout << "ISBN: " << ISBN << endl;
            cout << "Author: " << author << endl
                << endl;
            count++;
        }
        file.close();
        if (count == 0)
        {
            cout << "No books available in the library." << endl;
        }
    }
    else
    {
        cout << "Unable to open file to read books." << endl;
    }
}
void deleteBook()
{
    clearScreen();
    int targetISBN;
    cout << "Enter ISBN number of the book to delete: ";
    cin >> targetISBN;
    ifstream inFile("books.txt");
    ofstream outFile("temp.txt"); // temporary file to store deleted books
    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "Unable to open file to read/write books." << endl;
        return;
    }
    bool found = false;
    string line;
    while (getline(inFile, line))
    {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos)
        {
            string name = line.substr(0, pos1);
            string isbnStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
            int ISBN = stoi(isbnStr);
            string author = line.substr(pos2 + 1);
            if (ISBN == targetISBN)
            {
                found = true;
            }
            else
            {
                outFile << line << endl; // write to temp file if not deleted
            }
        }
    }
    inFile.close();
    outFile.close();
    remove("books.txt");             // this will remove old file
    rename("temp.txt", "books.txt"); // this will rename temp file to original, means books.txt file
    if (found)
    {
        cout << "Book with ISBN " << targetISBN << " deleted successfully!" << endl;
    }
    else
    {
        cout << "Book with ISBN " << targetISBN << " not found in the library." << endl;
    }
    // read remaining books
    ifstream remainingFile("books.txt");
    if (!remainingFile.is_open())
    {
        cout << "Unable to open file to read remaining books." << endl;
        return;
    }
    cout << "\nRemaining books in the library:\n";
    while (getline(remainingFile, line))
    {

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos)
        {
            string name = line.substr(0, pos1);
            string isbn = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string author = line.substr(pos2 + 1);
            cout << "Name: " << name << endl;
            cout << "Author: " << author << endl;
            cout << "ISBN: " << isbn << endl;
            cout << "---------------------------" << endl;
        }
    }
    remainingFile.close();
}
void searchBookByName()
{
    clearScreen();
    string targetName;
    cout << "Enter book name to search: ";
    cin.ignore();
    getline(cin, targetName);
    if (targetName.empty())
    {
        cout << "No book name entered. Please enter a valid book name." << endl;
        return;
    }
    ifstream inFile("books.txt");
    if (!inFile.is_open())
    {
        cout << "Unable to open books.txt file to read books." << endl;
        return;
    }
    bool found = false;
    string line;
    while (getline(inFile, line))
    {
        size_t pos = line.find(',');
        if (pos != string::npos)
        {
            string name = line.substr(0, pos);
            if (name == targetName)
            {
                found = true;
                cout << "Book found:" << endl;
                cout << "Details:" << endl;
                size_t pos2 = line.find(',', pos + 1);
                string isbn = line.substr(pos + 1, pos2 - pos - 1);
                string author = line.substr(pos2 + 1);
                cout << "Name: " << name << endl;
                cout << "Author: " << author << endl;
                cout << "ISBN: " << isbn << endl;
                // to check if a books is already issued to a customer
                ifstream issuedFile("issued_books.txt");
                if (issuedFile.is_open())
                {
                    bool isIssued = false;
                    while (getline(issuedFile, line))
                    {
                        size_t issuedPos = line.find(',');
                        if (issuedPos != string::npos)
                        {
                            string bookName = line.substr(0, issuedPos);
                            if (bookName == targetName)
                            {
                                cout << "This book is currently issued to a customer." << endl;
                                isIssued = true;
                                break;
                            }
                        }
                    }
                    issuedFile.close();
                    if (!isIssued)
                    {
                        cout << "This book is available." << endl;
                    }
                }
                else
                {
                    cout << "Unable to open issued_books.txt file to read issued books." << endl;
                }
                inFile.close();
                return;
            }
        }
    }
    inFile.close();
    if (!found)
    {
        cout << "Book with name \"" << targetName << "\" not found in the library." << endl;
    }
}
void searchBookByISBN()
{
    clearScreen();
    int targetISBN;
    cout << "Enter ISBN to search: ";
    cin >> targetISBN;
    ifstream inFile("books.txt");
    if (!inFile.is_open())
    {
        cout << "Unable to open books.txt file to read books." << endl;
        return;
    }
    bool found = false;
    string line;
    while (getline(inFile, line))
    {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos)
        {
            string name = line.substr(0, pos1);
            string isbnStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
            int isbn = stoi(isbnStr);
            string author = line.substr(pos2 + 1);
            if (isbn == targetISBN)
            {
                found = true;
                cout << "Book found:" << endl;
                cout << "Name: " << name << endl;
                cout << "Author: " << author << endl;
                cout << "ISBN: " << isbn << endl;
                ifstream issuedFile("issued_books.txt");
                if (issuedFile.is_open())
                {
                    while (getline(issuedFile, line))
                    {
                        size_t pos = line.find(',');
                        string bookISBNStr = line.substr(pos + 1);
                        int bookISBN = stoi(bookISBNStr);
                        if (bookISBN == targetISBN)
                        {
                            cout << "This book is currently issued to a customer." << endl;
                            break;
                        }
                    }
                    issuedFile.close();
                }
                else
                {
                    cout << "Unable to open issued_books.txt file to read issued books." << endl;
                }

                inFile.close();
                return;
            }
        }
    }
    inFile.close();
    if (!found)
    {
        cout << "Book with ISBN " << targetISBN << " not found in the library." << endl;
    }
}

void searchBookByAuthor()
{
    clearScreen();
    string targetAuthor;
    cout << "Enter author name to search: ";
    cin.ignore();
    getline(cin, targetAuthor);

    ifstream inFile("books.txt");
    if (!inFile.is_open())
    {
        cout << "Unable to open books.txt file to read books." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(inFile, line))
    {

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos)
        {
            string name = line.substr(0, pos1);
            string isbnStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
            int isbn = stoi(isbnStr);
            string author = line.substr(pos2 + 1);
            if (author == targetAuthor)
            {
                found = true;
                cout << "Book found:" << endl;
                cout << "Name: " << name << endl;
                cout << "Author: " << author << endl;
                cout << "ISBN: " << isbn << endl;

                ifstream issuedFile("issued_books.txt");
                if (issuedFile.is_open())
                {
                    while (getline(issuedFile, line))
                    {
                        size_t pos = line.find(',');
                        string bookISBNStr = line.substr(pos + 1);
                        int bookISBN = stoi(bookISBNStr);
                        if (bookISBN == isbn)
                        {
                            cout << "This book is currently issued to a customer." << endl;
                            break;
                        }
                    }
                    issuedFile.close();
                }
                else
                {
                    cout << "Unable to open issued_books.txt file to read issued books." << endl;
                }

                inFile.close();
                return;
            }
        }
    }
    inFile.close();

    if (!found)
    {
        cout << "No book by author \"" << targetAuthor << "\" found in the library." << endl;
    }
}
void addCustomer() {
    clearScreen();
    if (numCustomers < MAX_CUSTOMERS) {
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customers[numCustomers].name);
        // Find the highest existing ID in the file
        ifstream inFile("customers.txt");
        int highestID = 0;
        if (inFile.is_open()) {
            int fileID;
            string line;
            while (getline(inFile, line)) {
                size_t pos = line.find(',');
                if (pos != string::npos) {
                    fileID = stoi(line.substr(0, pos));
                    if (fileID > highestID) {
                        highestID = fileID;
                    }
                }
            }
            inFile.close();
        }

        // Assign the new customer ID
        customers[numCustomers].id = highestID + 1;
        cout << "Customer added successfully!\n";

        // Save the customer to file customer.txt
        ofstream outFile("customers.txt", ios::app);
        if (outFile.is_open()) {
            outFile << customers[numCustomers].id << "," << customers[numCustomers].name << endl;
            outFile.close();
            cout << "Customer saved to file successfully.\n";
        }
        else {
            cout << "Unable to open file to save customer.\n";
            return; // Exit function if unable to save
        }

        // Increment the number of customers only after successful save
        numCustomers++;

        cout << "Customer Name: " << customers[numCustomers - 1].name << endl;
        cout << "Customer ID: " << customers[numCustomers - 1].id << endl;
    }
    else {
        cout << "Maximum customers reached. Cannot add more.\n";
    }
}
void displayCustomers()
{
    clearScreen();
    ifstream file("customers.txt");
    if (file.is_open())
    {
        int count = 0;
        string line;
        while (getline(file, line))
        {
            string id, name;
            size_t pos = 0;
            pos = line.find(',');
            if (pos != string::npos)
            {
                id = line.substr(0, pos);
                name = line.substr(pos + 1);
                cout << "Customer ID: " << id << endl;
                cout << "Name: " << name << endl
                    << endl;
                count++;
            }
        }
        file.close();
        if (count == 0)
        {
            cout << "No customers available." << endl;
        }
    }
    else
    {
        cout << "Unable to open file to read customers." << endl;
    }
}
void user_interface()
{

    int ID;
    cout << "Welcome to the Library: " << endl;
    cout << "Enter your ID: ";
    cin >> ID;

    ifstream file("customers.txt");
    if (!file.is_open())
    {
        cout << "Unable to open file to read customers." << endl;
        return;
    }

    bool memberFound = false;
    int fileID;
    string name;
    while (!memberFound && file >> fileID)
    {
        file.ignore();
        getline(file, name);
        if (fileID == ID)
        {
            memberFound = true;
            cout << "Welcome back, " << name << "!" << endl;
        }
    }

    file.close(); // Close the file properly

    if (!memberFound)
    {
        cout << "You are not a member yet. Please register to borrow books." << endl;
        addCustomer();
    }

    int borrowedBooksCount = 0;
    for (int i = 0; i < numIssuedBooks; ++i)
    {
        if (issuedBooks[i].borrowerID == ID)
        {
            borrowedBooksCount++;
        }
    }

    if (borrowedBooksCount >= MAX_ISSUED_BOOKS)
    {
        cout << "You have already borrowed the maximum number of books." << endl;
        return;
    }

    int choice;
    cout << "Choose an option:" << endl;
    cout << "1. Borrow a book" << endl;
    cout << "2. Return a book" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        string searchOption;
        cout << "Search by (ISBN): ";
        cin >> searchOption;
        if (searchOption != "ISBN")
        {
            cout << "Invalid search option." << endl;
            return; // Exiting the function if search option is invalid
        }
        int bookIndex = -1;         // Declaration here
        ifstream file("books.txt"); // Changed variable name to "file"
        if (!file.is_open())
        {
            cout << "Unable to open file to read books." << endl;
            return;
        }
        else if (searchOption == "ISBN")
        {
            int ISBN;
            cout << "Enter the ISBN of the book you want to borrow: ";
            cin >> ISBN;

            // Check if the ISBN exists in the books file
            string line;
            while (getline(file, line))
            {
                // Get the ISBN from the line
                string bookISBN = line.substr(line.find(',') + 1, line.find_last_of(',') - line.find(',') - 1);
                // Trim leading whitespace
                bookISBN.erase(0, bookISBN.find_first_not_of(" \t"));
                // Trim trailing whitespace
                bookISBN.erase(bookISBN.find_last_not_of(" \t") + 1);
                if (stoi(bookISBN) == ISBN)
                {
                    bookIndex = ISBN;
                    break;
                }
            }
        }
        file.close(); // Close the books file

        if (bookIndex == -1)
        {
            cout << "Book not found in the library." << endl;
            return;
        }
        // Check if the book is already issued to the user
        bool alreadyBorrowed = false;
        for (int i = 0; i < numIssuedBooks; ++i)
        {
            if (issuedBooks[i].book_name == books[bookIndex].name && issuedBooks[i].borrowerID == ID)
            {
                alreadyBorrowed = true;
                cout << "You have already borrowed this book." << endl;
                return;
            }
        }
        // Check if the book is available for borrowing
        bool alreadyIssued = false;
        for (int i = 0; i < numIssuedBooks; ++i)
        {
            if (issuedBooks[i].book_name == books[bookIndex].name)
            {
                alreadyIssued = true;
                cout << "Sorry, the book is already borrowed by someone else." << endl;
                return;
            }
        }
        // Issue the book to the user
        issuedBooks[numIssuedBooks].book_name = books[bookIndex].name;
        issuedBooks[numIssuedBooks].borrowerID = ID;
        numIssuedBooks++;

        cout << "Book successfully issued." << endl;

        // Calculate remaining days
        int remainingDays = 30;
        cout << "You have " << remainingDays << " days left to return the book." << endl;

        // Write issued books to file
        writeIssuedBooksToFile();
        break;
    }
    }
}
int main()
{
    clearScreen();
    char choice;
    do
    {
        int option;
        cout << "Library Management System: " << endl;
        cout << "Choose from the list:" << endl;
        cout << "1. Add book" << endl;
        cout << "2. Display available books" << endl;
        cout << "3. Delete book" << endl;
        cout << "4. Search book by name" << endl;
        cout << "5. Search book by author" << endl;
        cout << "6. Search book by ISBN" << endl;
        cout << "7. Add customer" << endl;
        cout << "8. Display customers" << endl;
        cout << "9. User interface for issuing/returning books" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        switch (option)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            deleteBook();
            break;
        case 4:
            searchBookByName();
            break;
        case 5:
            searchBookByAuthor();
            break;
        case 6:
            searchBookByISBN();
            break;
        case 7:
            addCustomer();
            break;
        case 8:
            displayCustomers();
            break;
        case 9:
            user_interface();
            break;
        default:
            cout << "Invalid choice. Please choose a valid option.\n";
        }
        cout << "Do you want to continue? (y/n): ";
        cin >> choice;
        clearScreen();
    }

    while (choice == 'y' || choice == 'Y');
    writeIssuedBooksToFile();
    clearScreen();

    return 0;
}