# Zephyr NRF BLE Project 

## Zephyr Setup - Ubuntu

**Note:** This setup guide should work for any linux distribution and MacOS with minimal modifications.

Follow: 
https://docs.zephyrproject.org/latest/getting_started/index.html

After getting Zephyr and installing Python dependencies once you reached the installing toolchains section, go to: https://docs.zephyrproject.org/latest/getting_started/toolchain_3rd_party_x_compilers.html#gnu-arm-embedded

Going to use the GNU arm embedded toolchain since the NRF is an ARM based SoC.
Install gcc-arm-none-eabi-10-2020-q4-major from the GNU website. 
Extract and copy the files: 
```
tar -xf gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2
mkdir ~/toolchains
cp -r gcc-arm-none-eabi-10-2020-q4-major ~toolchains 
```

Once you have the toolchain, you can add it to your PATH, by setting the following enviroment variables.

Add to `~/.zshrc` or `~/.bashrc`:
```
export ZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb
export GNUARMEMB_TOOLCHAIN_PATH=~/toolchains/gcc-arm-none-eabi-10-2020-q4-major/
```

## Install J-link and NRF Command Line Tools
From: https://docs.zephyrproject.org/latest/guides/tools/nordic_segger.html#nordic-nrf5x-segger-j-link
Install: 
 - J-Link: https://www.segger.com/jlink-software.html
 - NRF Command Line Tools: https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools

 ### Board Specific 
 - NUCLEO_F401RE (Nucleo-F401RE) (**NOTE:** Need to use jlink to flash - https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/#compatible-evaluation-boards)

### Build and flash Zephyr sample app
For the nrf52833 DK board: 
```
cd /zephyrproject/zephyr/ 
source venv/bin/activate
west build -p auto -b nrf52833dk_nrf52833 samples/basic/blinky
west flash 
```

Sample App for Nucleo F401RE:
```
west build -b nucleo_f401re zephyr/samples/hello_world --pristine
west flash --runner jlink
```

### Zephyr App Example
https://github.com/zephyrproject-rtos/example-application

## Project Setup 
Note: Depending on the OS, you maybe need to resolve pip depenacy conflicts.
```
python -m venv venv
source venv/bin/activate
pip install west 
west init
west update
pip install -r zephyr/scripts/requirements.txt
```

## Build and Flash App
```
west build -b nrf52833dk_nrf52833 app --pristine
west flash
```

### Debug and Logging
In another terminal, run:
```
JLinkRTTClient
```

Then debug with:
```
west debug
```
This will let you use GDB to debug the app and the output will be printed to the terminal.

## Minicom

You can use minicom to connect to log output in a terminal.
Install: 
```
sudo apt-get install minicom
```

Connect to the serial port:
```
minicom -D /dev/ttyACM0
```
If you need to figure out which port to connect to: 
```
ls /dev
```

**Note:** For key shortcuts, press [ctrl-A] then [Z] to see the list of shortcuts. For any command, press [ctrl-A] then [-] to execute that command. 

## Troubleshooting
On ubuntu or debian systems, you may need to install: 
```
sudo apt install python3.8-venv
```
In order to create a virtual environment:
```
python -m venv venv
source venv/bin/activate
```

Note: If running conda on your system, ensure the base enviroment is not activated. 
```
CONDA_AUTO_ACTIVATE_BASE=false
```

### west debug error 
If when running `west debug` you get the following error:
```
FATAL ERROR: command exited with status 127: /home/urbanpistek/toolchains/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-gdb /home/urbanpistek/Urban/embedded_projects/zephyr_nrf_ble/build/zephyr/zephyr.elf -ex 'target remote :2331' -ex 'monitor halt' -ex 'monitor reset' -ex load
```

Install the following packages:
```
sudo apt-get install libncurses5
```

### CMake Build Error

If you get the following error:
```
`\CMake Error: Error processing file: /home/urbanpistek/Urban/at_at_mte380/ATAT-All-Terrain-Autonomous-Transport/zephyr_sample_app/zephyr/cmake/pristine.cmake
FATAL ERROR: command exited with status 1: /usr/bin/cmake -DBINARY_DIR=/home/urbanpistek/Urban/at_at_mte380/ATAT-All-Terrain-Autonomous-Transport/zephyr_sample_app/build -DSOURCE_DIR=/home/urbanpistek/Urban/at_at_mte380/ATAT-All-Terrain-Autonomous-Transport/zephyr_sample_app/app -P /home/urbanpistek/Urban/at_at_mte380/ATAT-All-Terrain-Autonomous-Transport/zephyr_sample_app/zephyr/cmake/pristine.cmake
```

Try deleting the build directory and re-running the build: