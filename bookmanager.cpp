#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
class Book {
public:
    // id， 书籍的唯一标识
    int book_id;
    // 书籍的名字
    std::string title;
    // 书籍的作者
    std::string author;
    // 出版年限
    int year;
    Book(int id, const std::string& t, const std::string& a, int y)
        : book_id(id), title(t), author(a), year(y) {}
};

class Library {
private:
    // 使用动态数组储存书籍，方便查阅的同时可以高效删减
    std::vector<Book> books;
    // 存储所有书籍的文件path
    std::string storage_file;

public:
    Library(const std::string& file) : storage_file(file) {
        load_data();
    }
    // 显示菜单
    void show_help() {
        std::cout << "===== Library Management System =====\n"
                  << "1. Add Book\n"
                  << "2. Remove Book\n"
                  << "3. Update Book\n"
                  << "4. Search Books\n"
                  << "5. Sort Books by Author\n"
                  << "0. Exit\n";
    }
    // 从storage_file读取所有数据进入ram
    void load_data() {
        std::ifstream file(storage_file);
        if (file.is_open()) {
            int id, year;
            std::string title, author;
            while (file >> id >> title >> author >> year) {
                books.emplace_back(id, title, author, year);
            }
            file.close();
        }
    }
    // 将当前所有图书信息输入到存储文件里
    void save_data() {
        std::ofstream file(storage_file);
        if (file.is_open()) {
            for (const auto& book : books) {
                file << book.book_id << " " << book.title << " " << book.author << " " << book.year << "\n";
            }
            file.close();
        }
    }
    // 添加书籍，记得存储信息
    void add_book(const Book& book) {
        // 不考虑重复书籍的问题，现实中的图书馆也可以放置多本同样的书
        if(book.book_id != books.size()) {
            std::cout << "invalid id :(\n";
            return ;
        }
        books.push_back(book);
        save_data();
    }
    // 移除书籍，记得存储信息
    void remove_book(int book_id) {
    auto it = std::remove_if(books.begin(), books.end(),
        [book_id](const Book& book) { return book.book_id == book_id; });

    if (it != books.end()) {
        // 找到了要删除的书籍
        books.erase(it, books.end());

        // 调整之后的书籍的 ID
        for (auto& book : books) {
            if (book.book_id > book_id) {
                book.book_id -= 1;
            }
        }

        save_data();
    } else {
        std::cout << "Book with ID " << book_id << " not found.\n";
    }
    }
    // 更新书籍信息，为了减少复杂性，直接更新所有属性
    void update_book(int book_id, const std::string& new_title, const std::string& new_author, int new_year) {
        for (auto& book : books) {
            if (book.book_id == book_id) {
                book.title = new_title;
                book.author = new_author;
                book.year = new_year;
            }
        }
        save_data();
    }
    // 查找书籍，按照书名或者作者查找
    void search_book(const std::string& keyword) {
        std::cout << "Search Results:\n";
        for (const auto& book : books) {
            if (book.title.find(keyword) != std::string::npos || book.author.find(keyword) != std::string::npos) {
                std::cout << "ID: " << book.book_id << " Title: " << book.title << " Author: " << book.author << " Year: " << book.year << "\n";
            }
        }
    }
    void get_book(int id){
        if(id < 0 || id >= books.size()){
            std::cout << "Invalid id" << std::endl;
            return ;
        }
        auto book = books[id];
        std::cout << "Search Results:\n";
        std::cout << "ID: " << book.book_id << " Title: " << book.title << " Author: " << book.author << " Year: " << book.year << "\n";
    }
    // 按照作者名对所有书籍进行排序，然后将结果显示出来
    void sort_books_by_author() {
        // 排序
        std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.author < b.author;
        });
        // 显示
        for(auto book : books){
            std::cout << "id: " << book.book_id << "autor : " << book.author 
            << " name : " << book.title << " publish year " << book.year << std::endl;
        }
        save_data();
    }
};

int main() {

    Library library("books.txt");

    int choice;
    library.show_help();

    do {
        std::cout << "Enter your choice :) :";
        std::cin >> choice;
        switch (choice) {
            case 1: {
                // 添加书籍
                int id, year;
                std::string title, author;

                std::cout << "Enter book details:\n";
                std::cout << "ID: ";
                std::cin >> id;
                // clear缓冲区
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Title: ";
                std::getline(std::cin, title);
                std::cout << "Author: ";
                std::getline(std::cin, author);
                std::cout << "Year: ";
                std::cin >> year;

                library.add_book(Book(id, title, author, year));
                break;
            }
            case 2: {
                // 移除书籍
                int book_id;
                std::cout << "Enter the ID of the book to remove :) : ";
                std::cin >> book_id;
                library.remove_book(book_id);
                break;
            }
            case 3: {
                // 更新信息
                int book_id, year;
                std::string title, author;

                std::cout << "Enter the ID of the book to update: ";
                std::cin >> book_id;
                std::cout << "Enter updated book details:\n";
                std::cout << "Title: ";
                // clear缓冲区
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, title);
                std::cout << "Author: ";
                std::getline(std::cin, author);
                std::cout << "Year: ";
                std::cin >> year;

                library.update_book(book_id, title, author, year);
                break;
            }
            case 4: {
                // 搜索书籍
                std::string keyword;
                std::cout << "Enter search keyword: ";
                std::cin >> keyword;
                library.search_book(keyword);
                break;
            }
            case 5: {
                // 按照作者进行排序并显示
                library.sort_books_by_author();
                break;
            }
            case 6: {
                // 按照作者进行排序并显示
                int id;
                std::cout << "id: ";
                std::cin >> id;
                library.get_book(id);
                break;
            }
            case 0: {
                // Exit
                std::cout << "Exiting the program.\n";
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
        library.show_help();
    } while (choice != 0);
    return 0;
}