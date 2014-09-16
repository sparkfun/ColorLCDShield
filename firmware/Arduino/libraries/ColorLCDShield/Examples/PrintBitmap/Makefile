# If your board is a mega 2560 uncomment the following three lines, adjust port
BOARD = mega2560
GCS = -Wl,--gc-sections
PORT = /dev/ttyACM2
# If your board is a mega 1280 uncomment the following two lines, adjust port
#BOARD = mega
#PORT = /dev/ttyUSB0

# If your board is using the SPI hack, uncomment the next line
EXTRA_FLAGS = -DLCD_USING_SPI=1 

# If your board is using the PWM hack, uncomment the next line 
EXTRA_FLAGS += -DHAVE_PWM_BRIGHTNESS_CONTROL=1

# And finally, the part that brings everything together for you.
include ../Arduino_Makefile_master/_Makefile.master
