#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

// بەڕێوەبەری بنکەدراوە
class DatabaseManager {
private:
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    
public:
    DatabaseManager() : conn(nullptr), res(nullptr) {}
    
    ~DatabaseManager() {
        if (res != nullptr) mysql_free_result(res);
        if (conn != nullptr) mysql_close(conn);
    }
    
    bool connect() {
        // دەستپێکردنی MySQL
        conn = mysql_init(NULL);
        if (!conn) {
            std::cerr << "هەڵە: نەتوانرا MySQL دەستپێبکرێت" << std::endl;
            return false;
        }
        
        // پەیوەندیکردن
        conn = mysql_real_connect(
            conn,           // Connection
            "localhost",    // Host
            "root",         // Username
            "",             // Password (بەتاڵ بێت ئەگەر نییە)
            "hospital_system", // Database
            3306,           // Port
            NULL,           // Unix socket
            0               // Client flag
        );
        
        if (!conn) {
            std::cerr << "هەڵە لە پەیوەندیکردن: " << mysql_error(conn) << std::endl;
            return false;
        }
        
        std::cout << "سەرکەوتوو بوو لە پەیوەندیکردن لەگەڵ MySQL!" << std::endl;
        
        // دانانی کۆدەکانی زمانی
        mysql_query(conn, "SET NAMES 'utf8'");
        mysql_query(conn, "SET CHARACTER SET utf8");
        
        return true;
    }
    
    // تاقیکردنەوەی پەیوەندی
    bool testConnection() {
        if (mysql_query(conn, "SELECT 1")) {
            std::cerr << "هەڵە لە تاقیکردنەوەی پەیوەندی: " << mysql_error(conn) << std::endl;
            return false;
        }
        return true;
    }
    
    // هێنانەوەی نەخۆشەکان
    void getPatients() {
        std::string query = "SELECT id, ناوی_تەواو, تەمەن, نەژاد, نەخۆشی, "
                           "ڕێکەوتی_ناسێنران, دۆخی_نەخۆش FROM patients";
        
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "هەڵە لە داواکاری: " << mysql_error(conn) << std::endl;
            return;
        }
        
        res = mysql_store_result(conn);
        if (!res) {
            std::cerr << "هەڵە لە وەرگرتنی ئەنجامەکان" << std::endl;
            return;
        }
        
        int num_fields = mysql_num_fields(res);
        
        std::cout << "\n=== لیستی نەخۆشەکان ===" << std::endl;
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(25) << "ناو"
                  << std::setw(10) << "تەمەن"
                  << std::setw(10) << "نەژاد"
                  << std::setw(25) << "نەخۆشی"
                  << std::setw(15) << "ڕێکەوتی ناسێنران"
                  << std::setw(20) << "دۆخ" << std::endl;
        
        std::cout << std::string(110, '-') << std::endl;
        
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                if (i == 0) std::cout << std::setw(5) << (row[i] ? row[i] : "NULL");
                else if (i == 1) std::cout << std::setw(25) << (row[i] ? row[i] : "NULL");
                else if (i == 2) std::cout << std::setw(10) << (row[i] ? row[i] : "NULL");
                else if (i == 3) std::cout << std::setw(10) << (row[i] ? row[i] : "NULL");
                else if (i == 4) std::cout << std::setw(25) << (row[i] ? row[i] : "NULL");
                else if (i == 5) std::cout << std::setw(15) << (row[i] ? row[i] : "NULL");
                else if (i == 6) std::cout << std::setw(20) << (row[i] ? row[i] : "NULL");
            }
            std::cout << std::endl;
        }
    }
    
    // زیادکردنی نەخۆش
    bool addPatient() {
        std::string name, gender, phone, address, disease, status;
        int age;
        
        std::cin.ignore();
        
        std::cout << "ناوی تەواوی نەخۆش: ";
        std::getline(std::cin, name);
        
        std::cout << "تەمەن: ";
        std::cin >> age;
        std::cin.ignore();
        
        std::cout << "نەژاد (نێر/مێ/تر): ";
        std::getline(std::cin, gender);
        
        std::cout << "ژمارەی تەلەفۆن: ";
        std::getline(std::cin, phone);
        
        std::cout << "ناونیشان: ";
        std::getline(std::cin, address);
        
        std::cout << "نەخۆشی: ";
        std::getline(std::cin, disease);
        
        std::cout << "دۆخی نەخۆش (چاکبوونەوە/لەژێر چاودێریدایە/ناچار): ";
        std::getline(std::cin, status);
        
        // بەدەستهێنانی ڕێکەوتی ئەمڕۆ
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char date[11];
        strftime(date, sizeof(date), "%Y-%m-%d", ltm);
        
        // دروستکردنی داواکاری
        std::string query = "INSERT INTO patients (ناوی_تەواو, تەمەن, نەژاد, ژمارەی_تەلەفۆن, "
                           "ناونیشان, نەخۆشی, ڕێکەوتی_ناسێنران, دۆخی_نەخۆش) VALUES ('" +
                           name + "', " + std::to_string(age) + ", '" + gender + "', '" +
                           phone + "', '" + address + "', '" + disease + "', '" +
                           date + "', '" + status + "')";
        
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "هەڵە لە زیادکردنی نەخۆش: " << mysql_error(conn) << std::endl;
            return false;
        }
        
        std::cout << "نەخۆش بە سەرکەوتوویی زیادکرا!" << std::endl;
        return true;
    }
    
    // ژمارەی گشتی نەخۆشەکان
    int getPatientCount() {
        if (mysql_query(conn, "SELECT COUNT(*) FROM patients")) {
            std::cerr << "هەڵە: " << mysql_error(conn) << std::endl;
            return -1;
        }
        
        res = mysql_store_result(conn);
        if (!res) return -1;
        
        row = mysql_fetch_row(res);
        int count = (row && row[0]) ? std::stoi(row[0]) : 0;
        
        mysql_free_result(res);
        res = nullptr;
        
        return count;
    }
};

// بەرنامەی سەرەکی
int main() {
    std::cout << "سیستەمی خواردنەوەی نەخۆشخانە - C++ MySQL Backend" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    DatabaseManager db;
    
    // پەیوەندیکردن
    if (!db.connect()) {
        std::cerr << "هەڵە لە پەیوەندیکردن لەگەڵ بنکەدراوە!" << std::endl;
        return 1;
    }
    
    // تاقیکردنەوەی پەیوەندی
    if (!db.testConnection()) {
        std::cerr << "پەیوەندی سەرکەوتوو نەبوو!" << std::endl;
        return 1;
    }
    
    std::cout << "پەیوەندی سەرکەوتوو بوو!" << std::endl;
    
    // پانێڵی بەڕێوەبردن
    int choice;
    do {
        std::cout << "\n=== پانێڵی بەڕێوەبردن ===" << std::endl;
        std::cout << "1. بینینی هەموو نەخۆشەکان" << std::endl;
        std::cout << "2. زیادکردنی نەخۆشی نوێ" << std::endl;
        std::cout << "3. ژمارەی گشتی نەخۆشەکان" << std::endl;
        std::cout << "4. تاقیکردنەوەی پەیوەندی" << std::endl;
        std::cout << "0. دەرچوون" << std::endl;
        std::cout << "هەڵبژاردن: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                db.getPatients();
                break;
            case 2:
                db.addPatient();
                break;
            case 3: {
                int count = db.getPatientCount();
                if (count >= 0) {
                    std::cout << "ژمارەی گشتی نەخۆشەکان: " << count << std::endl;
                }
                break;
            }
            case 4:
                if (db.testConnection()) {
                    std::cout << "پەیوەندی چالاکە!" << std::endl;
                }
                break;
            case 0:
                std::cout << "دەرچوون..." << std::endl;
                break;
            default:
                std::cout << "هەڵبژاردەی نادرووست!" << std::endl;
        }
    } while (choice != 0);
    
    return 0;
}
