#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <chrono>
#include <ctime>
#include <cstdlib>

using namespace std;

class Book {
public:
    string title;
    string author;
    string ISBN;
    string category;
    int totalCopies;
    int borrowedCopies;
    int borrowCount;

    Book(string t, string a, string i, string c, int tc)
        : title(t), author(a), ISBN(i), category(c), totalCopies(tc), borrowedCopies(0), borrowCount(0) {}
};

class Member {
public:
    string name;
    string familyName;
    string memberId;
    string password;
    int borrowedBooks;
    int transactionCount;

    Member(string n, string fn, string id, string pw)
        : name(n), familyName(fn), memberId(id), password(pw), borrowedBooks(0), transactionCount(0) {}
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    unordered_map<string, vector<string>> borrowedBooksByMember;
    multimap<time_t, string> transactions;

    void loadBooks();
    void saveBooks();
    void loadMembers();
    void saveMembers();
    void loadBorrowedBooks();
    void saveBorrowedBooks();
    void loadTransactions();
    void saveTransactions();

public:
    Library() {
        loadBooks();
        loadMembers();
        loadBorrowedBooks();
        loadTransactions();
    }

    ~Library() {
        saveBooks();
        saveMembers();
        saveBorrowedBooks();
        saveTransactions();
    }

    void addBook();
    void removeBook();
    void editBook();
    void searchBook();
    void addMember();
    void removeMember();
    void borrowBook();
    void returnBook();
    void reserveBook();
    void listBooks();
    void listMembers();
    void listBorrowedBooksByMember(const string& memberId);
    void generateReports();
    void listOverdueBooks();
    void listPopularBooks();
    void listActiveMembers();
    void listTransactionsInRange(time_t start, time_t end);

    void adminMenu();
    void memberMenu(string memberId);
    void login();
}library;

void Library::loadBooks() {
    ifstream file("books.txt");
    if (!file.is_open()) return;
    while (!file.eof()) {
        string title, author, ISBN, category;
        int totalCopies, borrowedCopies, borrowCount;
        if (file >> title >> author >> ISBN >> category >> totalCopies >> borrowedCopies >> borrowCount) {
            books.push_back(Book(title, author, ISBN, category, totalCopies));
            books.back().borrowedCopies = borrowedCopies;
            books.back().borrowCount = borrowCount;
        } else {
            file.clear();
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    file.close();
}

void Library::saveBooks() {
    ofstream file("books.txt");
    for (const auto& book : books) {
        file << book.title << " " << book.author << " " << book.ISBN << " " << book.category << " " << book.totalCopies << " " << book.borrowedCopies << " " << book.borrowCount << "\n";
    }
    file.close();
}

void Library::loadMembers() {
    ifstream file("members.txt");
    if (!file.is_open()) return;
    while (!file.eof()) {
        string name, familyName, memberId, password;
        int borrowedBooks, transactionCount;
        if (file >> name >> familyName >> memberId >> password >> borrowedBooks >> transactionCount) {
            members.push_back(Member(name, familyName, memberId, password));
            members.back().borrowedBooks = borrowedBooks;
            members.back().transactionCount = transactionCount;
        } else {
            file.clear();
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    file.close();
}

void Library::saveMembers() {
    ofstream file("members.txt");
    for (const auto& member : members) {
        file << member.name << " " << member.familyName << " " << member.memberId << " " << member.password << " " << member.borrowedBooks << " " << member.transactionCount << "\n";
    }
    file.close();
}

void Library::loadBorrowedBooks() {
    ifstream file("borrowed_books.txt");
    if (!file.is_open()) return;
    string memberId, ISBN;
    while (file >> memberId >> ISBN) {
        borrowedBooksByMember[memberId].push_back(ISBN);
    }
    file.close();
}

void Library::saveBorrowedBooks() {
    ofstream file("borrowed_books.txt");
    for (const auto& pair : borrowedBooksByMember) {
        for (const auto& ISBN : pair.second) {
            file << pair.first << " " << ISBN << "\n";
        }
    }
    file.close();
}

void Library::loadTransactions() {
    ifstream file("transactions.txt");
    if (!file.is_open()) return;
    time_t timestamp;
    string memberId;
    while (file >> timestamp >> memberId) {
        transactions.insert({timestamp, memberId});
    }
    file.close();
}

void Library::saveTransactions() {
    ofstream file("transactions.txt");
    for (const auto& transaction : transactions) {
        file << transaction.first << " " << transaction.second << "\n";
    }
    file.close();
}

void Library::addMember() {
    system("CLS");
    string name, familyName, memberId, password;
    cout << "Enter member name: ";
    cin >> name;
    cout << "Enter member family name: ";
    cin >> familyName;
    cout << "Enter member ID: ";
    cin >> memberId;
    cout << "Enter member password: ";
    cin >> password;

    members.push_back(Member(name, familyName, memberId, password));
    saveMembers();
    cout << "Member added successfully!" << endl;
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::removeMember() {
    string memberId;
    cout << "Enter member ID to remove: ";
    cin >> memberId;

    auto it = remove_if(members.begin(), members.end(), [&memberId](Member& member) {
        return member.memberId == memberId;
    });

    if (it != members.end()) {
        members.erase(it, members.end());
        borrowedBooksByMember.erase(memberId);
        saveMembers();
        saveBorrowedBooks();
        cout << "Member removed successfully!" << endl;
    } else {
        cout << "Member not found!" << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::addBook() {
    string title, author, ISBN, category;
    int totalCopies;
    cout << "Enter book title: ";
    cin >> title;
    cout << "Enter book author: ";
    cin >> author;
    cout << "Enter book ISBN: ";
    cin >> ISBN;
    cout << "Enter book category: ";
    cin >> category;
    cout << "Enter total copies: ";
    cin >> totalCopies;

    books.push_back(Book(title, author, ISBN, category, totalCopies));
    saveBooks();
    cout << "Book added successfully!" << endl;
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::removeBook() {
    string ISBN;
    cout << "Enter ISBN of the book to remove: ";
    cin >> ISBN;

    auto it = remove_if(books.begin(), books.end(), [&ISBN](Book& book) {
        return book.ISBN == ISBN;
    });

    if (it != books.end()) {
        books.erase(it, books.end());
        saveBooks();
        cout << "Book removed successfully!" <<endl;
    } else {
        cout << "Book not found!" << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::editBook() {
    string ISBN;
    cout << "Enter ISBN of the book to edit: ";
    cin >> ISBN;

    for (auto& book : books) {
        if (book.ISBN == ISBN) {
            cout << "Enter new title: ";
            cin >> book.title;
            cout << "Enter new author: ";
            cin >> book.author;
            cout << "Enter new category: ";
            cin >> book.category;
            cout << "Enter new total copies: ";
            cin >> book.totalCopies;
            saveBooks();
            cout << "Book edited successfully!" << endl;
            return;
        }
    }
    cout << "Book not found!" << endl;
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::searchBook() {
    string ISBN;
    cout << "Enter ISBN of the book to search: ";
    cin >> ISBN;

    for (const auto& book : books) {
        if (book.ISBN == ISBN) {
            cout << "Title: " << book.title << ", Author: " << book.author << ", Category: " << book.category << ", Total Copies: " << book.totalCopies << ", Borrowed Copies: " << book.borrowedCopies << std::endl;
            return;
        }
    }
    cout << "Book not found!" << endl;
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::borrowBook() {
    string memberId, ISBN;
    cout << "Enter member ID: ";
    cin >> memberId;
    cout << "Enter book ISBN: ";
    cin >> ISBN;

    auto memberIt = find_if(members.begin(), members.end(), [&memberId](Member& member) {
        return member.memberId == memberId;
    });

    auto bookIt = find_if(books.begin(), books.end(), [&ISBN](Book& book) {
        return book.ISBN == ISBN;
    });

    if (memberIt != members.end() && bookIt != books.end()) {
        if (bookIt->totalCopies > bookIt->borrowedCopies) {
            bookIt->borrowedCopies++;
            bookIt->borrowCount++;
            memberIt->borrowedBooks++;
            memberIt->transactionCount++;
            borrowedBooksByMember[memberId].push_back(ISBN);

            time_t now = time(nullptr);
            transactions.insert({now, memberId});

            saveBooks();
            saveMembers();
            saveBorrowedBooks();
            saveTransactions();

            cout << "Book borrowed successfully!" << endl;
        } else {
            cout << "No copies available to borrow!" << endl;
        }
    } else {
        cout << "Member or book not found!" <<endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::returnBook() {
    string memberId, ISBN;
    cout << "Enter member ID: ";
    cin >> memberId;
    cout << "Enter book ISBN: ";
    cin >> ISBN;

    auto memberIt = find_if(members.begin(), members.end(), [&memberId](Member& member) {
        return member.memberId == memberId;
    });

    auto bookIt = find_if(books.begin(), books.end(), [&ISBN](Book& book) {
        return book.ISBN == ISBN;
    });

    if (memberIt != members.end() && bookIt != books.end()) {
        auto& borrowedBooks = borrowedBooksByMember[memberId];
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), ISBN);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            bookIt->borrowedCopies--;
            memberIt->borrowedBooks--;
            saveBooks();
            saveMembers();
            saveBorrowedBooks();
            cout << "Book returned successfully!" <<endl;
        } else {
           cout << "This book was not borrowed by this member!" << endl;
        }
    } else {
        cout << "Member or book not found!" << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::reserveBook() {
    string memberId, ISBN;
    cout << "Enter member ID: ";
    cin >> memberId;
    cout << "Enter book ISBN: ";
    cin >> ISBN;

    auto memberIt = find_if(members.begin(), members.end(), [&memberId](Member& member) {
        return member.memberId == memberId;
    });

    auto bookIt = find_if(books.begin(), books.end(), [&ISBN](Book& book) {
        return book.ISBN == ISBN;
    });

    if (memberIt != members.end() && bookIt != books.end()) {
        if (bookIt->totalCopies > bookIt->borrowedCopies) {
            cout << "Book is available, no need to reserve." << endl;
        } else {
            cout << "Book reserved successfully!" << endl;
        }
    } else {
        cout << "Member or book not found!" << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listBooks() {
    cout << "Title                       | Author              | ISBN        | Category           | Total Copies | Borrowed Copies" << endl;
    cout << "---------------------------|---------------------|-------------|--------------------|--------------|-----------------" << endl;


    for (const auto& book : books) {
        cout << book.title << string(28 - book.title.length(), ' ') << " | "
             << book.author << string(19 - book.author.length(), ' ') << " | "
             << book.ISBN << string(12 - book.ISBN.length(), ' ') << " | "
             << book.category << string(19 - book.category.length(), ' ') << " | "
             << book.totalCopies << string(13 - to_string(book.totalCopies).length(), ' ') << " | "
             << book.borrowedCopies << string(15 - to_string(book.borrowedCopies).length(), ' ') << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listMembers() {
    cout << "Name                     | Family Name          | Member ID   | Borrowed Books" << endl;
    cout << "------------------------|----------------------|-------------|----------------" << endl;

    for (const auto& member : members) {
        cout << member.name << string(24 - member.name.length(), ' ') << " | "
             << member.familyName << string(22 - member.familyName.length(), ' ') << " | "
             << member.memberId << string(11 - member.memberId.length(), ' ') << " | "
             << member.borrowedBooks << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listBorrowedBooksByMember(const string& memberId) {
    auto memberIt = find_if(members.begin(), members.end(), [&memberId](Member& member) {
        return member.memberId == memberId;
    });

    if (memberIt != members.end()) {
        cout << "Borrowed books for member " << memberId << ":" << endl;
        for (const auto& ISBN : borrowedBooksByMember[memberId]) {
            auto bookIt = find_if(books.begin(), books.end(), [&ISBN](Book& book) {
                return book.ISBN == ISBN;
            });
            if (bookIt != books.end()) {
                cout << "Title: " << bookIt->title << ", Author: " << bookIt->author << ", ISBN: " << bookIt->ISBN << ", Category: " << bookIt->category << endl;
            }
        }
    } else {
        cout << "Member not found!" << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listOverdueBooks() {
    // Implement logic to list overdue books based on borrow date and return date.
    // This requires tracking the borrow and return dates for each book.
}

void Library::listPopularBooks() {
    // List books based on the borrow count
    vector<Book> popularBooks = books;
    sort(popularBooks.begin(), popularBooks.end(), [](const Book& a, const Book& b) {
        return a.borrowCount > b.borrowCount;
    });

    cout << "Popular books:" << endl;
    for (const auto& book : popularBooks) {
        cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN << ", Category: " << book.category << ", Borrow Count: " << book.borrowCount << std::endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listActiveMembers() {
    vector<Member> activeMembers = members;
    sort(activeMembers.begin(), activeMembers.end(), [](const Member& a, const Member& b) {
        return a.transactionCount > b.transactionCount;
    }
);

    cout << "Active members:" << endl;
    for (const auto& member : activeMembers) {
        cout << "Name: " << member.name << " " << member.familyName << ", Member ID: " << member.memberId << ", Transaction Count: " << member.transactionCount << endl;
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

void Library::listTransactionsInRange(time_t start, time_t end) {
    cout << "Transactions from " << ctime(&start) << " to " << ctime(&end) << ":" << endl;
    for (const auto& transaction : transactions) {
        if (transaction.first >= start && transaction.first <= end) {
            cout << "Transaction Time: " << ctime(&transaction.first) << ", Member ID: " << transaction.second << endl;
        }
    }
    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("CLS");
}

time_t convertToEpoch(int year, int month, int day, int hour = 0, int minute = 0, int second = 0) {
    tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = -1;
    return std::mktime(&t);
}

void Library::generateReports() {
    int reportType;
    cout << "Generate Reports:" << endl;
    cout << "\t\t-------------------------------------------------\n";
    cout << "\t1. Books with overdue returns" << endl;
    cout << "\t\t-------------------------------------------------\n";
    cout << "\t2. Popular books" << endl;
    cout << "\t\t-------------------------------------------------\n";
    cout << "\t3. Active members" << endl;
    cout << "\t\t-------------------------------------------------\n";
    cout << "\t4. Transactions in a time range" << endl;
    cout << "\t\t-------------------------------------------------\n";
    cout << "Enter your choice: ";
    cin >> reportType;

    switch (reportType) {
        case 1: listOverdueBooks(); break;
        case 2: listPopularBooks(); break;
        case 3: listActiveMembers(); break;
        case 4: {
            int startYear, startMonth, startDay, endYear, endMonth, endDay;
            cout << "Enter start date (YYYY MM DD): ";
            cin >> startYear >> startMonth >> startDay;
            cout << "Enter end date (YYYY MM DD): ";
            cin >> endYear >> endMonth >> endDay;

            time_t start = convertToEpoch(startYear, startMonth, startDay);
            time_t end = convertToEpoch(endYear, endMonth, endDay, 23, 59, 59);
            listTransactionsInRange(start, end);
            break;
        }
        case 5: break;
        default: cout << "Invalid choice!" << endl; break;
    }
}

void Library::adminMenu() {
    int choice;
    do {
        cout << "\t\t\t\t\t\t  WELCOME MANAGER \n\n\n";
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t1. ADD NEW BOOK \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t2. REMOVE THE BOOK  \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t3. EDIT THE BOOK  \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t4. SEARCH THE BOOK \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t5. ADD NEW MEMBER \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t6. REMOVE THE MEMBER  \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t7. LIST OF ALL THE BOOKS \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t8. LIST OF ALL THE MEMBERS \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t9. BORROW THE BOOK \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t10. Return Book\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t11. RESERVE THE BOOK \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t12. REPORTS \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t13. LOGOUT \n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("CLS");


        switch (choice) {

            case 1: cout << "add new book\n" ; addBook(); break;
            case 2: cout << "remove the book\n" ;removeBook(); break;
            case 3: cout << "edit the book\n" ; editBook(); break;
            case 4: cout << "search the book\n" ; searchBook(); break;
            case 5: cout << "radd new member\n" ; addMember(); break;
            case 6: cout << "remove the member\n" ; removeMember(); break;
            case 7: listBooks(); break;
            case 8: listMembers(); break;
            case 9: cout << "borrow the book\n" ; borrowBook(); break;
            case 10: cout << "return the book\n" ; returnBook(); break;
            case 11: cout << "reserve the book\n" ; reserveBook(); break;
            case 12 : cout << "reports\n" ; generateReports() ; break ;
            case 13:
				cout << "Logged out!" << endl;
				system("timeout /t 3");
				system("cls");
				library.login();
			break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 13);
}

void Library::memberMenu(string memberId) {
    int choice;

    do {
        cout << "\t\t\t\t\t\t  WELCOME TO THE LIBRARY MANAGEMENT \n\n\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t1. SEARCH THE BOOK\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t2. BORROW THE BOOK\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t3. Return Book\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t4. RESERVE THE BOOK\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t5. LIST OF BORROWED BOOKS\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n";
        cout << "\t\t6. LOGOUT\n" ;
        cout << "\t\t---------------------------------------------------------------------------------------\n\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("CLS");

        switch (choice) {
            case 1: borrowBook(); break;
            case 2: returnBook(); break;
            case 3: reserveBook(); break;
            case 4: listBooks(); break;
            case 5: listBorrowedBooksByMember(memberId); break;
            case 6:
				cout << "Logged out!" << endl;
				system("timeout /t 3");
				system("cls");
				library.login();
			break;
            default: cout << "Invalid choice!" << endl; break;
        }
    } while (choice != 6);
}

void Library::login() {
    string userId, password;
    cout << "Enter user ID: ";
    cin >> userId;
    cout << "Enter password: ";
    cin >> password;

    if (userId == "admin" && password == "admin") {
        system("CLS");
        adminMenu();

    } else {
        auto memberIt = find_if(members.begin(), members.end(), [&userId, &password](Member& member) {
            return member.memberId == userId && member.password == password;
        });

        if (memberIt != members.end()) {
            memberMenu(userId);
        } else {
            cout << "Invalid user ID or password!" << endl;
            system("timeout /t 3");
			system("cls");
			library.login();
        }
    }

}

void intro() {
    cout << "\t\t******************************************************\n";
    cout << "\n\t\t LIBRARY MANAGEMENT SYSTEM - Created by Maedeh Jebeli\n\n";
    cout << "\t\t******************************************************\n";
}

int main() {
    intro();
    library.login();
    return 0;
}
