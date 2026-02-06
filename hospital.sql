-- سیستەمی سادەی بەڕێوەبردنی عیادە
-- بۆ MySQL / MariaDB

-- دروستکردنی داتابەیس
CREATE DATABASE IF NOT EXISTS simple_clinic;
USE simple_clinic;

-- 1. خشتەی نەخۆشەکان
CREATE TABLE patients (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(20),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 2. خشتەی چاوپێکەتنەکان
CREATE TABLE appointments (
    id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    appointment_date DATE NOT NULL,
    appointment_time TIME NOT NULL,
    doctor VARCHAR(100),
    notes TEXT,
    status ENUM('چاوەڕوانی', 'کۆتای هات', 'هەڵوەشایەوە') DEFAULT 'چاوەڕوانی',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
);

-- 3. زیادکردنی داتای نموونەیی
INSERT INTO patients (name, phone) VALUES
('عەلی کەریم', '07501234567'),
('سارا عەبدوڵڵا', '07507654321'),
('حەسەن محەممەد', '07701112233');

INSERT INTO appointments (patient_id, appointment_date, appointment_time, doctor, notes) VALUES
(1, '2024-03-20', '10:00', 'د. حەمەد', 'سەرئێشە'),
(2, '2024-03-21', '14:30', 'د. کەریم', 'پشکنینی ساڵانە'),
(1, '2024-03-25', '09:00', 'د. حەمەد', 'دووبارەکردنەوە');

-- 4. پرسیارە سادەکان
-- هەموو نەخۆشەکان
SELECT * FROM patients ORDER BY created_at DESC;

-- چاوپێکەتنەکانی ئەمڕۆ
SELECT 
    p.name,
    p.phone,
    a.appointment_date,
    a.appointment_time,
    a.doctor,
    a.status
FROM appointments a
JOIN patients p ON a.patient_id = p.id
WHERE a.appointment_date = CURDATE()
ORDER BY a.appointment_time;

-- ژمارەی چاوپێکەتنەکان بەپێی ڕۆژ
SELECT 
    appointment_date,
    COUNT(*) as total,
    SUM(CASE WHEN status = 'کۆتای هات' THEN 1 ELSE 0 END) as completed
FROM appointments
GROUP BY appointment_date
ORDER BY appointment_date DESC;
