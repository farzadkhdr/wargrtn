-- دروستکردنی بنکەدراوەی سیستەمی نەخۆشخانە
CREATE DATABASE IF NOT EXISTS hospital_system;
USE hospital_system;

-- خشتەی نەخۆشەکان
CREATE TABLE IF NOT EXISTS patients (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ناوی_تەواو VARCHAR(100) NOT NULL,
    تەمەن INT NOT NULL,
    نەژاد VARCHAR(50),
    نەخۆشی VARCHAR(200),
    ڕێکەوتی_ناسێنران DATE NOT NULL,
    دۆخی_نەخۆش VARCHAR(50) DEFAULT 'چاکبوونەوە',
    ژمارەی_تەلەفۆن VARCHAR(20),
    ناونیشان TEXT,
    درووستکراوە_لە TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- خشتەی پزیشکەکان
CREATE TABLE IF NOT EXISTS doctors (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ناوی_تەواو VARCHAR(100) NOT NULL,
    پسپۆڕی VARCHAR(100) NOT NULL,
    ژمارەی_مۆبایل VARCHAR(20),
    کارکردن_لە TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- خشتەی چاوپێکەوتنەکان
CREATE TABLE IF NOT EXISTS appointments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    نەخۆش_id INT,
    پزیشک_id INT,
    ڕێکەوتی_چاوپێکەوتن DATETIME NOT NULL,
    هۆکار TEXT,
    دۆخی_چاوپێکەوتن VARCHAR(50) DEFAULT 'ڕێکخراوە',
    FOREIGN KEY (نەخۆش_id) REFERENCES patients(id),
    FOREIGN KEY (پزیشک_id) REFERENCES doctors(id)
);

-- داتای نموونەیی بۆ تاقیکردنەوە
INSERT INTO patients (ناوی_تەواو, تەمەن, نەژاد, نەخۆشی, ڕێکەوتی_ناسێنران, دۆخی_نەخۆش, ژمارەی_تەلەفۆن, ناونیشان) VALUES
('هەڤال عەبدوڵڵا', 35, 'کورد', 'سەرئێشەی بەهێز', '2024-01-15', 'چاکبوونەوە', '07501234567', 'هەولێر، گۆڕەپان'),
('سارا محەممەد', 28, 'کورد', 'ئازاری گەروو', '2024-01-20', 'چاکبوونەوە', '07507654321', 'سلێمانی، شەقامی سەید'),
('عەلی حوسێن', 45, 'کورد', 'پەستانەی خوێن', '2024-02-05', 'لەژێر چاودێریدایە', '07509876543', 'دهۆک، ناوچەی نەورۆز'),
('نازنین عەلی', 32, 'کورد', 'زەکام', '2024-02-10', 'چاکبوونەوە', '07501239876', 'هەولێر، ئەنکاو');

INSERT INTO doctors (ناوی_تەواو, پسپۆڕی, ژمارەی_مۆبایل) VALUES
('د. کەریم ئەحمەد', 'دەرزی نەشتەرگەری', '07501112233'),
('د. شیلان عەبدوڵڵا', 'پزیشکی گشتی', '07504445566'),
('د. ڕەنج بارزان', 'پزیشکی دڵ', '07507778899');

INSERT INTO appointments (نەخۆش_id, پزیشک_id, ڕێکەوتی_چاوپێکەوتن, هۆکار) VALUES
(1, 2, '2024-02-20 10:00:00', 'پشکنینی دووبارە'),
(2, 1, '2024-02-21 14:30:00', 'نەشتەرگەری بچووک'),
(3, 3, '2024-02-22 11:15:00', 'پشکنینی دڵ');
