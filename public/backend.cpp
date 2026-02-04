#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

class Patient {
public:
    int id;
    std::string name;
    std::string father;
    std::string phone;
    std::string disease;
    std::string createdAt;

    json toJson() const {
        return {
            {"id", id},
            {"name", name},
            {"father", father},
            {"phone", phone},
            {"disease", disease},
            {"createdAt", createdAt}
        };
    }
};

class Treatment {
public:
    int id;
    int patientId;
    std::string treatment;
    std::string date;

    json toJson() const {
        return {
            {"id", id},
            {"patientId", patientId},
            {"treatment", treatment},
            {"date", date}
        };
    }
};

class Appointment {
public:
    int id;
    int patientId;
    std::string date;
    std::string time;
    std::string notes;

    json toJson() const {
        return {
            {"id", id},
            {"patientId", patientId},
            {"date", date},
            {"time", time},
            {"notes", notes}
        };
    }
};

class HospitalBackend {
private:
    std::string dataDir;
    std::vector<Patient> patients;
    std::vector<Treatment> treatments;
    std::vector<Appointment> appointments;

    void loadPatients() {
        std::ifstream file(dataDir + "/patients.json");
        if (file.is_open()) {
            json j;
            file >> j;
            file.close();
            
            for (const auto& item : j) {
                Patient p;
                p.id = item["id"];
                p.name = item["name"];
                p.father = item["father"];
                p.phone = item["phone"];
                p.disease = item["disease"];
                p.createdAt = item["createdAt"];
                patients.push_back(p);
            }
        }
    }

    void savePatients() {
        json j = json::array();
        for (const auto& patient : patients) {
            j.push_back(patient.toJson());
        }
        
        std::ofstream file(dataDir + "/patients.json");
        file << j.dump(4);
        file.close();
    }

    void loadTreatments() {
        std::ifstream file(dataDir + "/treatments.json");
        if (file.is_open()) {
            json j;
            file >> j;
            file.close();
            
            for (const auto& item : j) {
                Treatment t;
                t.id = item["id"];
                t.patientId = item["patientId"];
                t.treatment = item["treatment"];
                t.date = item["date"];
                treatments.push_back(t);
            }
        }
    }

    void saveTreatments() {
        json j = json::array();
        for (const auto& treatment : treatments) {
            j.push_back(treatment.toJson());
        }
        
        std::ofstream file(dataDir + "/treatments.json");
        file << j.dump(4);
        file.close();
    }

    void loadAppointments() {
        std::ifstream file(dataDir + "/appointments.json");
        if (file.is_open()) {
            json j;
            file >> j;
            file.close();
            
            for (const auto& item : j) {
                Appointment a;
                a.id = item["id"];
                a.patientId = item["patientId"];
                a.date = item["date"];
                a.time = item["time"];
                a.notes = item["notes"];
                appointments.push_back(a);
            }
        }
    }

    void saveAppointments() {
        json j = json::array();
        for (const auto& appointment : appointments) {
            j.push_back(appointment.toJson());
        }
        
        std::ofstream file(dataDir + "/appointments.json");
        file << j.dump(4);
        file.close();
    }

public:
    HospitalBackend(const std::string& dir = "./data") : dataDir(dir) {
        // دڵنیابوون لە بوونی دایرێکتۆری داتا
        if (!fs::exists(dataDir)) {
            fs::create_directory(dataDir);
        }
        
        // دڵنیابوون لە بوونی فایلەکان
        std::vector<std::string> files = {
            "/patients.json",
            "/treatments.json",
            "/appointments.json"
        };
        
        for (const auto& file : files) {
            std::string fullPath = dataDir + file;
            if (!fs::exists(fullPath)) {
                std::ofstream createFile(fullPath);
                createFile << "[]";
                createFile.close();
            }
        }
        
        loadPatients();
        loadTreatments();
        loadAppointments();
    }

    int addPatient(const Patient& patient) {
        patients.push_back(patient);
        savePatients();
        return patient.id;
    }

    std::vector<Patient> searchPatients(const std::string& query) {
        std::vector<Patient> results;
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
        
        for (const auto& patient : patients) {
            std::string name = patient.name;
            std::string father = patient.father;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            std::transform(father.begin(), father.end(), father.begin(), ::tolower);
            
            if (name.find(lowerQuery) != std::string::npos || 
                father.find(lowerQuery) != std::string::npos) {
                results.push_back(patient);
            }
        }
        
        return results;
    }

    Patient* getPatientById(int id) {
        for (auto& patient : patients) {
            if (patient.id == id) {
                return &patient;
            }
        }
        return nullptr;
    }

    void addTreatment(const Treatment& treatment) {
        treatments.push_back(treatment);
        saveTreatments();
    }

    std::vector<Treatment> getPatientTreatments(int patientId) {
        std::vector<Treatment> result;
        for (const auto& treatment : treatments) {
            if (treatment.patientId == patientId) {
                result.push_back(treatment);
            }
        }
        
        // ڕیزکردن بە پێی بەروار (نوێترین یەکەم)
        std::sort(result.begin(), result.end(), 
            [](const Treatment& a, const Treatment& b) {
                return a.date > b.date;
            });
        
        return result;
    }

    void addAppointment(const Appointment& appointment) {
        appointments.push_back(appointment);
        saveAppointments();
    }

    std::vector<Appointment> getAppointmentsByDate(const std::string& date) {
        std::vector<Appointment> results;
        for (const auto& appointment : appointments) {
            if (appointment.date == date) {
                results.push_back(appointment);
            }
        }
        return results;
    }

    int getTodayAppointmentsCount() {
        // بۆ نموونە، ئەمڕۆ بە 2024-01-01 دانەنراوە
        return getAppointmentsByDate("2024-01-01").size();
    }

    // فەنکشنەکانی گێتەر
    int getTotalPatients() const { return patients.size(); }
    int getTotalTreatments() const { return treatments.size(); }
};

int main() {
    HospitalBackend backend;
    
    // نموونەی بەکارهێنان
    Patient newPatient;
    newPatient.id = 1;
    newPatient.name = "عەلی";
    newPatient.father = "مەحموود";
    newPatient.phone = "07501234567";
    newPatient.disease = "سەرئێشە";
    newPatient.createdAt = "2024-01-01T10:00:00Z";
    
    backend.addPatient(newPatient);
    
    std::cout << "ژمارەی گشتی نەخۆشەکان: " << backend.getTotalPatients() << std::endl;
    std::cout << "ژمارەی گشتی چارەسەرەکان: " << backend.getTotalTreatments() << std::endl;
    std::cout << "ژمارەی چاوپێکەوتنەکانی ئەمڕۆ: " << backend.getTodayAppointmentsCount() << std::endl;
    
    return 0;
}
