
#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Book Class
class Book {
private:
    string isbn;
    string title;
    int totalCopies;
    int copiesLeft;

public:
    Book(const string& i, const string& t, int c) : isbn(i), title(t), totalCopies(c), copiesLeft(c) {}

    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    int getCopiesLeft() const { return copiesLeft; }
    int getTotalCopies() const { return totalCopies; }

    bool borrowCopy() {
        if (copiesLeft > 0) {
            copiesLeft--;
            return true;
        }
        return false;
    }

    void returnCopy() {
        if (copiesLeft < totalCopies) {
            copiesLeft++;
        }
    }
};

// User Class
class User {
private:
    string id;
    string name;
    vector<string> borrowedISBNs;
    static const int MAX_BORROW = 3;

public:
    User(const string& i, const string& n) : id(i), name(n) {}

    string getID() const { return id; }
    string getName() const { return name; }
    const vector<string>& getBorrowedISBNs() const { return borrowedISBNs; }

    bool canBorrow() const {
        return borrowedISBNs.size() < MAX_BORROW;
    }

    bool hasBook(const string& isbn) const {
        for (vector<string>::const_iterator it = borrowedISBNs.begin(); it != borrowedISBNs.end(); ++it) {
            if (*it == isbn) return true;
        }
        return false;
    }

    void addBorrowedBook(const string& isbn) {
        borrowedISBNs.push_back(isbn);
    }

    bool removeBook(const string& isbn) {
        for (vector<string>::iterator it = borrowedISBNs.begin(); it != borrowedISBNs.end(); ++it) {
            if (*it == isbn) {
                borrowedISBNs.erase(it);
                return true;
            }
        }
        return false;
    }
};

// Library Class
class Library {
private:
    vector<Book> books;
    vector<User> users;

public:
    int findUserIndex(const string& id) const {
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i].getID() == id) return static_cast<int>(i);
        }
        return -1;
    }

    int findBookIndex(const string& isbn) const {
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getISBN() == isbn) return static_cast<int>(i);
        }
        return -1;
    }

    void addBook() {
        string isbn, title;
        int copies;

        cout << "ISBN: ";
        cin >> isbn;

        if (findBookIndex(isbn) != -1) {
            cout << "Book with this ISBN already exists.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Title: ";
        getline(cin, title);

        cout << "Copies: ";
        while (!(cin >> copies) || copies <= 0) {
            cout << "Invalid: Must be a positive number. Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        books.push_back(Book(isbn, title, copies));
        cout << "Book added.\n";
    }

    void addStudent() {
        string id, name;

        cout << "Admission No: ";
        cin >> id;

        if (findUserIndex(id) != -1) {
            cout << "Student with this ID already exists.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Name: ";
        getline(cin, name);

        users.push_back(User(id, name));
        cout << "Student added.\n";
    }

    void removeBook() {
        string isbn;
        cout << "Enter ISBN to remove: ";
        cin >> isbn;

        int idx = findBookIndex(isbn);
        if (idx == -1) {
            cout << "Book not found.\n";
            return;
        }

        if (books[idx].getCopiesLeft() != books[idx].getTotalCopies()) {
            cout << "Cannot remove: Some copies are currently borrowed.\n";
            return;
        }

        books.erase(books.begin() + idx);
        cout << "Book removed successfully.\n";
    }

    void borrowBook() {
        string id, isbn;
        cout << "Admission No: ";
        cin >> id;

        cout << "ISBN: ";
        cin >> isbn;

        int uIdx = findUserIndex(id);
        int bIdx = findBookIndex(isbn);

        if (uIdx == -1) { cout << "Student not found.\n"; return; }
        if (bIdx == -1) { cout << "Book not found.\n"; return; }

        User& u = users[uIdx];
        Book& b = books[bIdx];

        if (!u.canBorrow()) { cout << "Max borrow limit reached.\n"; return; }
        if (u.hasBook(isbn)) { cout << "Already has this book.\n"; return; }
        if (b.getCopiesLeft() == 0) { cout << "No copies available.\n"; return; }

        b.borrowCopy();
        u.addBorrowedBook(isbn);
        cout << "Borrowed successfully.\n";
    }

    void returnBook() {
        string id, isbn;
        cout << "Admission No: ";
        cin >> id;

        cout << "ISBN: ";
        cin >> isbn;

        int uIdx = findUserIndex(id);
        int bIdx = findBookIndex(isbn);

        if (uIdx == -1) { cout << "Student not found.\n"; return; }
        if (bIdx == -1) { cout << "Book not found.\n"; return; }

        if (users[uIdx].removeBook(isbn)) {
            books[bIdx].returnCopy();
            cout << "Returned successfully.\n";
        } else {
            cout << "User does not have this book.\n";
        }
    }

    void showAllBooks() const {
        cout << "\nLibrary Books:\n";
        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }
        for (vector<Book>::const_iterator it = books.begin(); it != books.end(); ++it) {
            cout << it->getISBN() << " - " << it->getTitle()
                 << " [" << it->getCopiesLeft() << "/" << it->getTotalCopies() << "]\n";
        }
    }

    void showAllUsers() const {
        cout << "\nRegistered Students:\n";
        if (users.empty()) {
            cout << "No students registered.\n";
            return;
        }
        for (vector<User>::const_iterator uIt = users.begin(); uIt != users.end(); ++uIt) {
            cout << uIt->getID() << " - " << uIt->getName() << " (Borrowed: ";
            const vector<string>& borrowed = uIt->getBorrowedISBNs();
            if (borrowed.empty()) {
                cout << "none";
            } else {
                for (vector<string>::const_iterator bIt = borrowed.begin(); bIt != borrowed.end(); ++bIt) {
                    cout << *bIt;
                    if ((bIt + 1) != borrowed.end()) cout << ", ";
                }
            }
            cout << ")\n";
        }
    }
};

int main() {
    Library myLib;
    int choice;

    while (true) {
        cout << "\n1. Add Book\n2. Add Student\n3. Borrow Book\n4. Return Book\n"
             << "5. Remove Book\n6. Show All Books\n7. Show All Students\n8. Exit\nChoice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input - please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 8) break;
        else if (choice == 1) myLib.addBook();
        else if (choice == 2) myLib.addStudent();
        else if (choice == 3) myLib.borrowBook();
        else if (choice == 4) myLib.returnBook();
        else if (choice == 5) myLib.removeBook();
        else if (choice == 6) myLib.showAllBooks();
        else if (choice == 7) myLib.showAllUsers();
        else cout << "Invalid choice - try again.\n";
    }
    return 0;
}
