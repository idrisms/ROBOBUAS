ROBOBUAS

Bagai mana nak mula

1 - Muat turun semua kod dalam satu folder untuk sumo auto, untuk RC guna kod dalam MOTOR_ESP32HOTRC_V01
2 - Install Arduino software kalau belum (https://www.arduino.cc/en/software)
- ![image](https://github.com/user-attachments/assets/d5e1ac11-e52f-4b1e-b002-7e3a9ed0469f)

3 - Install board manager ESP32 dari Espressif
  Buka perisian arduino, kemudian tekan Board manager
  ![image](https://github.com/user-attachments/assets/c13115bf-03bd-4aa9-bd49-6f9ec5910b2d)
  Kemudian pilih kawasan carian dan cari esp32
  ![image](https://github.com/user-attachments/assets/51c9f8da-faf9-4e49-84d8-6df88c9ce2f2)

4 - Lnstall CH341 untuk microcontroller esp32 kalau belum (https://www.wch-ic.com/downloads/CH341SER_ZIP.html)
- ![image](https://github.com/user-attachments/assets/d0805ede-bc56-406c-b553-5c014d1370c2)

5- Extract dan click setup
- ![image](https://github.com/user-attachments/assets/db6584c4-6d27-42fe-b66c-c5376149aeb5)

6 - Install CH341SER dengan menekan butang "INSTALL"
- ![image](https://github.com/user-attachments/assets/d0e25c16-c637-4360-a863-792ecece3bfb)

7 - Bagaimana nak muat naik code. Tekan >> Tools >> Esp32 >> ESP32-WROOM-DA Module
- ![image](https://github.com/user-attachments/assets/326f17ee-cffa-4a50-be4a-181087ee252b)

8 - Sambungkan arduino anda dan pilih port yang telah aktif. Akan ade bunyi sesuatu di PC/Laptop anda sekiranya ESp32-30 di pasangkan sudah di kesan. Tekan Tools >> PORT >> Nombor port anda
![image](https://github.com/user-attachments/assets/a3b698e5-4871-4168-9591-fe34de632508)

9 - pastikan pilihan board dan port ayng betul
- ![image](https://github.com/user-attachments/assets/7f73965b-8ea4-4c4f-b3d6-dc67cd00a48c)


10 - Waktu Muat Naik code di dalam ESP32,kadang-kadang perlu tekan butang Boot pada ESP32 waktu connecting keluar titik-titik......
- ![image](https://github.com/user-attachments/assets/910f7cf3-5f70-49f8-bba0-6b48ae54edd3)
- ![image](https://github.com/user-attachments/assets/92d780cf-f5a8-451f-8d12-6d1ca22ae6fb)



**Untuk RC**
- Download MOTOR_ESP32HOTRC_V01 dan upload ke dalam robobuas esp32 board 
- Ikut seperti pendawaian di dalam https://sites.google.com/view/robobuasbuyov1
- JIka Board anda bukan ROBOBUAS BUYO, sila tukar nombor bin dalam kod berdasarkan nombor pin anda.
- ![image](https://github.com/user-attachments/assets/3e7951b3-e09e-4ede-81b9-f3e0e3a2b73a)

- Hidupkan RC controller
- "BIND" receiver dengan transmitter bergantung kepada jenis RC controller anda
- Hidupkan Robot setelah semua pendawaian telah di semak. sebarang kerosakan board tidak ditanggung sekiranya semakan tidak di buat terlebih dahulu.**Pastikan RCcontroller tidak ditekan**
- Robot akan bergerak sendiri di awal suis dihidupkan selama beberapa saat namun ia akan berhenti sendiri sekiranyaia telah tersambung ke RC Controller
- Biasanya lampu receiver akan menyala tanpa berkelip sekiranya telah bersambung dengan RC controller
- Jika robot telah tersambung kepada controller, tetapi masih ada bunyi atau pergerakan yang perlahan, tinggikan nilai "OFFSET" secara gandaan 10 dan muat naik kod yang telah diubah
- ![image](https://github.com/user-attachments/assets/24fe0820-5f2f-4300-9850-a7cc85b97f7c)

- **AMARAN- Sila cabut ESP32 daripada ROBOBUAS untuk muat naikkod kerana board akan menyalurkan kuasa dari kabel USB ke Motor dan boleh merosakkan PC/LAPTOP sekiranya dibiarkan untuk tempoh yang lama**
- Tambah nilai offset sehinga tiada pergerakan walaupun RC controller tidak ditekan
- Sekirannya gerakan terbalik,tukar wayar dc motor kepada wayar pasangannya
- Sekiranya semua langkah-langkah ini diikuti dengan teliti, InsyaALLAH robot akan berfungsi

**UNtuk Auto**
- Buka SUMOROBOTAUTO_ROBOBUAS.ino menggunakan perisian ARDUINO
- Cara-cara ubah tetapan ada di dalam itu.
