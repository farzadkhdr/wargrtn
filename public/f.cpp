// database_manager.cpp
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class DatabaseManager {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    
public:
    DatabaseManager() {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
            con->setSchema("healthcare_system");
            cout << "Database connected successfully!" << endl;
        } catch (sql::SQLException &e) {
            cerr << "Connection error: " << e.what() << endl;
        }
    }
    
    ~DatabaseManager() {
        delete con;
    }
    
    // 1. زیادکردنی نەخۆشی نوێ
    bool addPatient(const string& name, int age, const string& gender, 
                    const string& phone, const string& email, const string& address) {
        try {
            sql::PreparedStatement *pstmt;
            pstmt = con->prepareStatement(
                "INSERT INTO patients (name, age, gender, phone, email, address) "
                "VALUES (?, ?, ?, ?, ?, ?)"
            );
            
            pstmt->setString(1, name);
            pstmt->setInt(2, age);
            pstmt->setString(3, gender);
            pstmt->setString(4, phone);
            pstmt->setString(5, email);
            pstmt->setString(6, address);
            
            pstmt->execute();
            delete pstmt;
            return true;
        } catch (sql::SQLException &e) {
            cerr << "Error adding patient: " << e.what() << endl;
            return false;
        }
    }
    
    // 2. گەڕان بەدوای نەخۆشەکان
    vector<vector<string>> searchPatients(const string& query) {
        vector<vector<string>> results;
        
        try {
            sql::PreparedStatement *pstmt;
            sql::ResultSet *res;
            
            pstmt = con->prepareStatement(
                "SELECT * FROM patients WHERE name LIKE ? OR phone LIKE ? OR email LIKE ?"
            );
            
            string searchPattern = "%" + query + "%";
            pstmt->setString(1, searchPattern);
            pstmt->setString(2, searchPattern);
            pstmt->setString(3, searchPattern);
            
            res = pstmt->executeQuery();
            
            while (res->next()) {
                vector<string> row;
                row.push_back(to_string(res->getInt("id")));
                row.push_back(res->getString("name"));
                row.push_back(to_string(res->getInt("age")));
                row.push_back(res->getString("gender"));
                row.push_back(res->getString("phone"));
                row.push_back(res->getString("email"));
                results.push_back(row);
            }
            
            delete res;
            delete pstmt;
        } catch (sql::SQLException &e) {
            cerr << "Search error: " << e.what() << endl;
        }
        
        return results;
    }
    
    // 3. بینینی هەموو نەخۆشەکان
    vector<vector<string>> getAllPatients() {
        vector<vector<string>> patients;
        
        try {
            sql::Statement *stmt;
            sql::ResultSet *res;
            
            stmt = con->createStatement();
            res = stmt->executeQuery("SELECT * FROM patients ORDER BY created_at DESC");
            
            while (res->next()) {
                vector<string> row;
                row.push_back(to_string(res->getInt("id")));
                row.push_back(res->getString("name"));
                row.push_back(to_string(res->getInt("age")));
                row.push_back(res->getString("gender"));
                row.push_back(res->getString("phone"));
                row.push_back(res->getString("email"));
                patients.push_back(row);
            }
            
            delete res;
            delete stmt;
        } catch (sql::SQLException &e) {
            cerr << "Error fetching patients: " << e.what() << endl;
        }
        
        return patients;
    }
    
    // 4. زیادکردنی چاوپێکەوتنی نوێ
    bool addAppointment(int patient_id, const string& date, const string& time,
                       const string& doctor, const string& reason) {
        try {
            sql::PreparedStatement *pstmt;
            pstmt = con->prepareStatement(
                "INSERT INTO appointments (patient_id, appointment_date, appointment_time, "
                "doctor_name, reason) VALUES (?, ?, ?, ?, ?)"
            );
            
            pstmt->setInt(1, patient_id);
            pstmt->setString(2, date);
            pstmt->setString(3, time);
            pstmt->setString(4, doctor);
            pstmt->setString(5, reason);
            
            pstmt->execute();
            delete pstmt;
            return true;
        } catch (sql::SQLException &e) {
            cerr << "Error adding appointment: " << e.what() << endl;
            return false;
        }
    }
    
    // 5. وەرگرتنی ئامارەکان
    vector<int> getDashboardStats() {
        vector<int> stats(4, 0);
        
        try {
            sql::Statement *stmt = con->createStatement();
            sql::ResultSet *res;
            
            // ژمارەی نەخۆشەکان
            res = stmt->executeQuery("SELECT COUNT(*) as total FROM patients");
            if (res->next()) stats[0] = res->getInt("total");
            
            // چاوپێکەوتنەکانی ئەمڕۆ
            res = stmt->executeQuery(
                "SELECT COUNT(*) as today FROM appointments "
                "WHERE appointment_date = CURDATE()"
            );
            if (res->next()) stats[1] = res->getInt("today");
            
            // چاوپێکەوتنە پلانکراوەکان
            res = stmt->executeQuery(
                "SELECT COUNT(*) as upcoming FROM appointments "
                "WHERE appointment_date >= CURDATE() AND status = 'Scheduled'"
            );
            if (res->next()) stats[2] = res->getInt("upcoming");
            
            delete stmt;
        } catch (sql::SQLException &e) {
            cerr << "Error getting stats: " << e.what() << endl;
        }
        
        return stats;
    }
    
    // 6. تایبەتمەندییەکانی تری پێویست...
};

// 7. CGI Handler بۆ پەیوەندی HTML و C++
void handleCGIRequest() {
    cout << "Content-type: text/html\n\n";
    
    DatabaseManager db;
    
    // لێرەدا CGI پارامیتەرەکان دەخوێنرێتەوە
    // و کردارەکان جێبەجێ دەکرێن
}
