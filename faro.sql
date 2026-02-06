-- 1. دروستکردنی داتابەیس
CREATE DATABASE IF NOT EXISTS clinic_management;
USE clinic_management;

-- 2. خشتەی نەخۆشەکان
CREATE TABLE patients (
    patient_id INT PRIMARY KEY AUTO_INCREMENT,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    date_of_birth DATE NOT NULL,
    gender ENUM('نێر', 'مێ', 'تر') NOT NULL,
    phone_number VARCHAR(20),
    email VARCHAR(100),
    address TEXT,
    emergency_contact_name VARCHAR(100),
    emergency_contact_phone VARCHAR(20),
    blood_type ENUM('A+', 'A-', 'B+', 'B-', 'AB+', 'AB-', 'O+', 'O-'),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_name (last_name, first_name),
    INDEX idx_phone (phone_number)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 3. خشتەی چاوپێکەتنەکان
CREATE TABLE appointments (
    appointment_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    appointment_date DATE NOT NULL,
    appointment_time TIME NOT NULL,
    doctor_name VARCHAR(100) NOT NULL,
    appointment_type ENUM('ئاسایی', 'ڕووناکی', 'پزیشکی تایبەت', 'وەرگرتنی خوێن', 'تاقیکردنەوە', 'تر') DEFAULT 'ئاسایی',
    reason TEXT,
    status ENUM('چاوەڕوانی', 'ڕێکخراو', 'کۆتای هات', 'هەڵوەشایەوە', 'بێدەرکەوت') DEFAULT 'چاوەڕوانی',
    diagnosis TEXT,
    prescription TEXT,
    notes TEXT,
    follow_up_date DATE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE,
    INDEX idx_date (appointment_date),
    INDEX idx_status (status),
    INDEX idx_patient_date (patient_id, appointment_date)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 4. خشتەی بەکارهێنەران (بۆ سیستەمی بەڕێوەبردنی ئەگەر پێویست بێت)
CREATE TABLE users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(100) NOT NULL,
    role ENUM('پزشک', 'کارمەند', 'بەڕێوەبەر') DEFAULT 'کارمەند',
    is_active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 5. داتای نموونەیی بۆ تاقیکردنەوە
-- نەخۆشەکانی نموونەیی
INSERT INTO patients (first_name, last_name, date_of_birth, gender, phone_number, email, address, blood_type) VALUES
('عەلی', 'مەحموود', '1985-03-15', 'نێر', '07501234567', 'ali.mahmood@example.com', 'هەولێر - ناوەندی شار', 'A+'),
('سارا', 'عەبدوڵڵا', '1992-07-22', 'مێ', '07507654321', 'sara.abdullah@example.com', 'هەولێر - ئەنکاو', 'O+'),
('کەریم', 'حوسێن', '1978-11-30', 'نێر', '07701234567', 'karim.hussein@example.com', 'سلێمانی - تاکۆیه', 'B-'),
('لەنا', 'عەلی', '1989-05-18', 'مێ', '07707654321', 'lana.ali@example.com', 'دهۆک - ناوەندی شار', 'AB+');

-- چاوپێکەتنەکانی نموونەیی
INSERT INTO appointments (patient_id, appointment_date, appointment_time, doctor_name, appointment_type, reason, status, diagnosis, prescription) VALUES
(1, '2024-03-20', '09:30:00', 'دکتۆر حەمەد عەلی', 'ئاسایی', 'سەرئێشە و جەڵتەی دەم', 'کۆتای هات', 'جەڵتەی دەم', 'پەناسیلین 500mg ٣ جار لە ڕۆژێکدا بۆ ٧ ڕۆژ'),
(2, '2024-03-21', '14:00:00', 'دکتۆر سەلاح کەریم', 'ڕووناکی', 'پشکنینی ساڵانە', 'ڕێکخراو', NULL, NULL),
(3, '2024-03-19', '11:00:00', 'دکتۆر نەسرین عەباس', 'تاقیکردنەوە', 'کێشەی دڵ', 'چاوەڕوانی', NULL, NULL),
(1, '2024-04-10', '10:00:00', 'دکتۆر حەمەد عەلی', 'پزیشکی تایبەت', 'دووبارەکردنەوەی چاوپێکەوتن', 'چاوەڕوانی', NULL, NULL);

-- بەکارهێنەرانی نموونەیی (وشەی نهێنی: password123)
INSERT INTO users (username, password_hash, full_name, role) VALUES
('admin', '$2y$10$YourHashedPasswordHere', 'بەڕێوەبەری سیستەم', 'بەڕێوەبەر'),
('doctor1', '$2y$10$YourHashedPasswordHere', 'دکتۆر حەمەد عەلی', 'پزشک'),
('staff1', '$2y$10$YourHashedPasswordHere', 'ئەحمەد کەریم', 'کارمەند');

-- 6. ڕاپۆرتەکان و بینینەکان (Views)
-- بینینی نەخۆشەکان لەگەڵ چاوپێکەتنە دوایینەکانیان
CREATE VIEW patient_last_appointment AS
SELECT 
    p.patient_id,
    p.first_name,
    p.last_name,
    p.phone_number,
    MAX(a.appointment_date) as last_appointment_date,
    COUNT(a.appointment_id) as total_appointments
FROM patients p
LEFT JOIN appointments a ON p.patient_id = a.patient_id
GROUP BY p.patient_id;

-- بینینی چاوپێکەتنەکانی ئەمڕۆ و داهاتوو
CREATE VIEW upcoming_appointments AS
SELECT 
    a.appointment_id,
    a.appointment_date,
    a.appointment_time,
    a.doctor_name,
    a.status,
    CONCAT(p.first_name, ' ', p.last_name) as patient_name,
    p.phone_number
FROM appointments a
JOIN patients p ON a.patient_id = p.patient_id
WHERE a.appointment_date >= CURDATE()
ORDER BY a.appointment_date, a.appointment_time;

-- 7. پرۆسێجرەکانی کۆگا (Stored Procedures)
-- پرۆسێجەر بۆ زیادکردنی نەخۆشێکی نوێ
DELIMITER $$
CREATE PROCEDURE AddNewPatient(
    IN p_first_name VARCHAR(50),
    IN p_last_name VARCHAR(50),
    IN p_date_of_birth DATE,
    IN p_gender ENUM('نێر', 'مێ', 'تر'),
    IN p_phone VARCHAR(20),
    IN p_email VARCHAR(100),
    IN p_address TEXT,
    IN p_blood_type ENUM('A+', 'A-', 'B+', 'B-', 'AB+', 'AB-', 'O+', 'O-')
)
BEGIN
    INSERT INTO patients (
        first_name, 
        last_name, 
        date_of_birth, 
        gender, 
        phone_number, 
        email, 
        address, 
        blood_type
    ) VALUES (
        p_first_name,
        p_last_name,
        p_date_of_birth,
        p_gender,
        p_phone,
        p_email,
        p_address,
        p_blood_type
    );
    
    SELECT LAST_INSERT_ID() as new_patient_id;
END$$
DELIMITER ;

-- پرۆسێجەر بۆ زیادکردنی چاوپێکەوتنێکی نوێ
DELIMITER $$
CREATE PROCEDURE ScheduleAppointment(
    IN p_patient_id INT,
    IN p_appointment_date DATE,
    IN p_appointment_time TIME,
    IN p_doctor_name VARCHAR(100),
    IN p_appointment_type ENUM('ئاسایی', 'ڕووناکی', 'پزیشکی تایبەت', 'وەرگرتنی خوێن', 'تاقیکردنەوە', 'تر'),
    IN p_reason TEXT
)
BEGIN
    INSERT INTO appointments (
        patient_id,
        appointment_date,
        appointment_time,
        doctor_name,
        appointment_type,
        reason,
        status
    ) VALUES (
        p_patient_id,
        p_appointment_date,
        p_appointment_time,
        p_doctor_name,
        p_appointment_type,
        p_reason,
        'چاوەڕوانی'
    );
    
    SELECT LAST_INSERT_ID() as new_appointment_id;
END$$
DELIMITER ;

-- 8. ترێجەرەکان (Triggers)
-- ترێجەر بۆ تۆمارکردنی گۆڕانکارییەکان لە خشتەی نەخۆشەکان
DELIMITER $$
CREATE TRIGGER before_patient_update
BEFORE UPDATE ON patients
FOR EACH ROW
BEGIN
    SET NEW.updated_at = CURRENT_TIMESTAMP;
END$$
DELIMITER ;

-- 9. نموونەیەک بۆ پرسیارەکانی SQL بەکارهێنەر
-- گەڕان بە ناوی نەخۆش
SELECT * FROM patients 
WHERE first_name LIKE '%عەلی%' OR last_name LIKE '%عەلی%';

-- بینینی هەموو چاوپێکەتنە چاوەڕوانیەکان
SELECT 
    a.appointment_id,
    a.appointment_date,
    a.appointment_time,
    a.doctor_name,
    CONCAT(p.first_name, ' ', p.last_name) as patient_name,
    p.phone_number
FROM appointments a
JOIN patients p ON a.patient_id = p.patient_id
WHERE a.status = 'چاوەڕوانی'
ORDER BY a.appointment_date, a.appointment_time;

-- ژمارەی چاوپێکەتنەکان بەپێی جۆر
SELECT 
    appointment_type,
    COUNT(*) as count,
    CASE 
        WHEN appointment_type = 'ئاسایی' THEN 'Normal'
        WHEN appointment_type = 'ڕووناکی' THEN 'Check-up'
        WHEN appointment_type = 'پزیشکی تایبەت' THEN 'Specialist'
        WHEN appointment_type = 'وەرگرتنی خوێن' THEN 'Blood Test'
        WHEN appointment_type = 'تاقیکردنەوە' THEN 'Examination'
        ELSE 'Other'
    END as type_english
FROM appointments
GROUP BY appointment_type
ORDER BY count DESC;

-- 10. ڕێکخستنەکانی پاڵپشتی
-- دروستکردنی بەکارهێنەرێکی تایبەت بۆ ئەپڵیکەیشنەکە
CREATE USER 'clinic_app'@'%' IDENTIFIED BY 'StrongPassword123!';
GRANT SELECT, INSERT, UPDATE, DELETE ON clinic_management.* TO 'clinic_app'@'%';
FLUSH PRIVILEGES;
