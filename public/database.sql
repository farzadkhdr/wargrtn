-- 1. دروستکردنی بنکەدراوە
CREATE DATABASE healthcare_system;
USE healthcare_system;

-- 2. خشتەی نەخۆشەکان
CREATE TABLE patients (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    age INT,
    gender ENUM('M', 'F', 'Other'),
    phone VARCHAR(20),
    email VARCHAR(100),
    address TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 3. خشتەی چاوپێکەوتنەکان
CREATE TABLE appointments (
    id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT,
    appointment_date DATE,
    appointment_time TIME,
    doctor_name VARCHAR(100),
    reason TEXT,
    status ENUM('Scheduled', 'Completed', 'Cancelled') DEFAULT 'Scheduled',
    FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
);

-- 4. خشتەی مێژووی پزیشکی
CREATE TABLE medical_history (
    id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT,
    visit_date DATE,
    diagnosis TEXT,
    treatment TEXT,
    prescription TEXT,
    doctor_name VARCHAR(100),
    FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
);

-- 5. خشتەی بەکارهێنەرەکان
CREATE TABLE users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('admin', 'doctor', 'receptionist') DEFAULT 'receptionist'
);

-- 6. تۆمارکردنی بەکارهێنەری بنەڕەتی
INSERT INTO users (username, password, role) 
VALUES ('admin', 'admin123', 'admin');
