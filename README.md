ROBOBUAS

Bagai mana nak mula

- Muat turun semua kod dalam satu folder untuk sumo auto
- Install Arduino software kalau belum (https://www.arduino.cc/en/software)
- ![image](https://github.com/user-attachments/assets/d5e1ac11-e52f-4b1e-b002-7e3a9ed0469f)

- Install board manager ESP32 dari Espressif
  Buka perisian arduino, kemudian tekan Board manager
  ![image](https://github.com/user-attachments/assets/c13115bf-03bd-4aa9-bd49-6f9ec5910b2d)
  Kemudian pilih kawasan carian dan cari esp32
  ![image](https://github.com/user-attachments/assets/51c9f8da-faf9-4e49-84d8-6df88c9ce2f2)

- Lnstall CH341 untuk microcontroller esp32 kalau belum (https://www.wch-ic.com/downloads/CH341SER_ZIP.html)
- ![image](https://github.com/user-attachments/assets/d0805ede-bc56-406c-b553-5c014d1370c2)

- Extract dan click setup
- ![image](https://github.com/user-attachments/assets/db6584c4-6d27-42fe-b66c-c5376149aeb5)

- Install CH341SER dengan menekan butang "INSTALL"
- ![image](https://github.com/user-attachments/assets/d0e25c16-c637-4360-a863-792ecece3bfb)

**Untuk RC**
-Download MOTOR_ESP32HOTRC_V01 dan upload ke dalam robobuas esp32 board 
- Ikut seperti pendawaian di dalam https://sites.google.com/view/robobuasbuyov1
- Hidupkan RC controller
- "BIND" receiver dengan transmitter bergantung kepada jenis RC controller anda
- Hidupkan Robot setelah semua pendawaian telah di semak. sebarang kerosakan board tidak ditanggung sekiranya semakan tidak di buat terlebih dahulu.**Pastikan RCcontroller tidak ditekan**
- Robot akan bergerak sendiri di awal suis dihidupkan selama beberapa saat namun ia akan berhenti sendiri sekiranyaia telah tersambung ke RC Controller
- Biasanya lampu receiver akan menyala tanpa berkelip sekiranya telah bersambung dengan RC controller
- Jika robot telah tersambung kepada controller, tetapi masinh ada bunyi atau pergerakan yang perlahan, tinggikan nilai "OFFSET" secara gandaan 10 dan muat naik kod yang telah diubah
- **AMARAN- Sila cabut ESP32 daripada ROBOBUAS untuk muat naikkod kerana board akan menyalurkan kuasa dari kabel USB ke Motor dan boleh merosakkan PC/LAPTOP sekiranya dibiarkan untuk tempoh yang lama**
- Tambah nilai offset sehinga tiada pergerakan walaupun RC controller tidak ditekan
- Sekirannya gerakan terbalik,tukar wayar dc motor kepada wayar pasangannya
- Sekiranya semua langkah-langkah ini diikuti dengan teliti, InsyaALLAH robot akan berfungsi

**UNtuk Auto**
- Buka SUMOROBOTAUTO_ROBOBUAS.ino menggunakan perisian ARDUINO
- Cara-cara ubah tetapan ada di dalam itu.
