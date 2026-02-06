const express = require('express');
const cors = require('cors');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

// میدڵوێرەکان
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// ڕووتەکانی API
app.use('/api/patients', require('./routes/patients'));
app.use('/api/appointments', require('./routes/appointments'));

// ڕووتی سەرەکی
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/public/index.html');
});

app.get('/api/health', (req, res) => {
    res.json({
        status: 'کار',
        message: 'سێرڤەری سیستەمی عیادە',
        version: '1.0.0',
        timestamp: new Date().toISOString()
    });
});

// دەستپێکردنی سێرڤەر
app.listen(PORT, () => {
    console.log(`✅ سێرڤەر کاردەکات لە http://localhost:${PORT}`);
    console.log(`📊 داشبۆرد: http://localhost:${PORT}/dashboard.html`);
});
