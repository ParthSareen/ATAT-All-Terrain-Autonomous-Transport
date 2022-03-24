# Using Custom Libraries in Arduino

1. Determine the path/location of your Arduino folder (the installed instance of Arduino, tpypically when you go to save a sketch it will save it to this path). 
2. Under this path, there should be a `libraries` folder, copy and paste your files into a folder that represents the name of the library under this folder
3. Reset your Arduino IDE if you have one running
4. Open an instance of Arduino IDE, under Sketck > Include Libraries, then scroll to Contributed Libraries - you should see the name of the folder you added previously here. 
5. Once you included you should see an `#include <>` so at the top of your sketch

# Wemos D1 Info

Guide to adding the board to Arduino IDE; https://arduino.esp8266.com/stable/package_esp8266com_index.json

Driver: https://www.wemos.cc/en/latest/ch340_driver.html

Also - be sure to change the upload speed to 115200 under tools and use that baud rate for the serial monitor so that you don't have giberish on the serial monitor