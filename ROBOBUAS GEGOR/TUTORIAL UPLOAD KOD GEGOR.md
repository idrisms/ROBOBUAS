ROBOBUAS GEGOR

Bagai mana nak mula

1 - Muat turun kod RC_GEGOR_V1_0_FULL_VOLTAGE_LOCK.ino
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

9 - pastikan pilihan board dan port yang betul
- ![image](https://github.com/user-attachments/assets/7f73965b-8ea4-4c4f-b3d6-dc67cd00a48c)

10 - Jika nak gunakan battery lipo 2s atau tak mahunguna safety battery, sila tukar nilai VMIN = 9.9 kepada 6.6 untuk lipo 2s (7.4V) atau kepada 0.0 jika anda tak mau safety pada battery

<img width="665" height="362" alt="image" src="https://github.com/user-attachments/assets/2f609b82-070c-4610-bbaa-0289ccb6fadc" />

11 - Install Semua library yang diperlukan

<img width="1002" height="503" alt="image" src="https://github.com/user-attachments/assets/f487eced-0d60-463d-a499-70cdde57eed6" />

<img width="915" height="742" alt="image" src="https://github.com/user-attachments/assets/e968ef02-49dc-4d3f-baa7-1f679f76fa79" />

<img width="838" height="620" alt="image" src="https://github.com/user-attachments/assets/f74990e1-9cda-45b3-8a26-481f32ea544f" />


12-Contoh cara nak install library. Tekan icon library, cari neopixel dan install library enopixel yang dibulatkan. Instal yang ada error sahaja
<img width="918" height="679" alt="579013854-72b96216-1686-4d35-9036-d260c8034658" src="https://github.com/user-attachments/assets/00171d11-8d2e-4ab1-b9cb-0ecace462004" />

13 - Waktu Muat Naik code di dalam ESP32,kadang-kadang perlu tekan butang Boot pada ESP32 waktu connecting keluar titik-titik......
- ![image](https://github.com/user-attachments/assets/910f7cf3-5f70-49f8-bba0-6b48ae54edd3)
- ![image](https://github.com/user-attachments/assets/92d780cf-f5a8-451f-8d12-6d1ca22ae6fb)



**Untuk RC**

- Hidupkan RC controller
- "BIND" receiver dengan transmitter bergantung kepada jenis RC controller anda
- Hidupkan Robot setelah semua pendawaian telah di semak. sebarang kerosakan board tidak ditanggung sekiranya semakan tidak di buat terlebih dahulu.**Pastikan RCcontroller tidak ditekan**
- Biasanya lampu receiver akan menyala tanpa berkelip sekiranya telah bersambung dengan RC controller
- Jika robot telah tersambung kepada controller, tetapi masih ada bunyi atau pergerakan yang perlahan, ejas setting dekat remote
- Sekiranya gerakan terbalik,tukar wayar dc motor kepada wayar pasangannya
- Sekiranya semua langkah-langkah ini diikuti dengan teliti, InsyaALLAH robot akan berfungsi


