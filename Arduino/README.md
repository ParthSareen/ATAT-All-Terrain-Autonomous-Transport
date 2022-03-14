# Using Custom Libraries in Arduino

1. Determine the path/location of your Arduino folder (the installed instance of Arduino, tpypically when you go to save a sketch it will save it to this path). 
2. Under this path, there should be a `libraries` folder, copy and paste your files into a folder that represents the name of the library under this folder
3. Reset your Arduino IDE if you have one running
4. Open an instance of Arduino IDE, under Sketck > Include Libraries, then scroll to Contributed Libraries - you should see the name of the folder you added previously here. 
5. Once you included you should see an `#include <>` so at the top of your sketch