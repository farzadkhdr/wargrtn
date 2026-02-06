const mysql = require('mysql2');
require('dotenv').config();

const connection = mysql.createConnection({
    host: process.env.DB_HOST || 'localhost',
    user: process.env.DB_USER || 'root',
    password: process.env.DB_PASSWORD || '',
    multipleStatements: true
});

const sql = `
-- دروستکردنی داتابەیس
CREATE DATABASE IF NOT EXISTS ${process.env.DB_NAME || 'simple_clinic'};
USE ${process.env.DB_NAME || 'simple_clinic'};

-- 1. خشتەی نەخۆشەکان
CREATE TABLE IF NOT EXISTS patients (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(20),
    age INT,
    gender ENUM('نێر', 'مێ', 'تر'),
    address TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 2. خشتەی چاوپێکەتنەکان
CREATE TABLE IF NOT EXISTS appointments (
    id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    appointment_date DATE NOT NULL,
    appointment_time TIME NOT NULL,
    doctor VARCHAR(100),
    type VARCHAR(50),
    notes TEXT,
    status ENUM('چاوەڕوانی', 'کۆتای هات', 'هەڵوەشایەوە') DEFAULT 'چاوەڕوانی',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE,
    INDEX idx_date (appointment_date),
    INDEX idx_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 3. زیادکردنی داتای نموونەیی
INSERT IGNORE INTO patients (id, name, phone, age, gender, address) VALUES
(1, 'عەلی کەریم', '07501234567', 35, 'نێر', 'هەولێر - ناوەندی شار'),
(2, 'سارا عەبدوڵڵا', '07507654321', 28, 'مێ', 'سلێمانی - ئەنکاو'),
(3, 'حەسەن محەممەد', '07701112233', 45, 'نێر', 'دهۆک - گەڕەکی نوێ');

INSERT IGNORE INTO appointments (patient_id, appointment_date, appointment_time, doctor, type, notes, status) VALUES
(1, '2024-03-20', '10:00:00', 'د. حەمەد عەلی', 'ئاسایی', 'سەرئێشە و جەڵتەی دەم', 'چاوەڕوانی'),
(2, '2024-03-21', '14:30:00', 'د. کەریم سەعید', 'پشکنینی ساڵانە', 'پشکنینی تەندروستی', 'چاوەڕوانی'),
(3, '2024-03-22', '09:00:00', 'د. نەسرین عەباس', 'پزیشکی تایبەت', 'کێشەی دڵ', 'چاوەڕوانی'),
(1, '2024-03-25', '11:00:00', 'د. حەمەد عەلی', 'دووبارەکردنەوە', 'دووبارەکردنەوەی چاوپێکەوتن', 'چاوەڕوانی');

-- 4. پرۆسێجەری کۆگا بۆ ئاماری داشبۆرد
DELIMITER $$
CREATE PROCEDURE IF NOT EXISTS GetDashboardStats()
BEGIN
    -- کۆی نەخۆشەکان
    SELECT COUNT(*) as total_patients FROM patients;
    
    -- چاوپێکەتنەکانی ئەمڕۆ
    SELECT COUNT(*) as today_appointments FROM appointments 
    WHERE appointment_date = CURDATE();
    
    -- چاوپێکەتنی چاوەڕوانی
    SELECT COUNT(*) as pending_appointments FROM appointments 
    WHERE status = 'چاوەڕوانی';
    
    -- چاوپێکەتنی کۆتای هات
    SELECT COUNT(*) as completed_appointments FROM appointments 
    WHERE status = 'کۆتای هات';
END$$
DELIMITER ;
`;

console.log('🚀 دەستپێکردنی دامەزراندنی داتابەیس...');

connection.query(sql, (error, results) => {
    if (error) {
        console.error('❌ هەڵە لە دامەزراندنی داتابەیس:', error.message);
        process.exit(1);
    }
    
    console.log('✅ داتابەیس بە سەرکەوتوویی دروستکرا!');
    console.log('📊 ناوەکانی خشتەکان:');
    console.log('   - patients');
    console.log('   - appointments');
    console.log(`🌐 داتابەیس: ${process.env.DB_NAME || 'simple_clinic'}`);
    
    connection.end();
});
