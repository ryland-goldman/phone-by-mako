# Phone by Mako
## Here to create and inspire the next generation of makers.
https://www.phonebymako.com/

Meet the Phone by Mako. It's a simple, do it yourself phone that teaches you how cellphones work. It comes in a premium, hand-crafted wooden box, with a hand-crafted wooden housing. The phone is powered by the Adafruit Feather FONA, which is an Arduino with a cellular card. The kit also includes a keypad to type in phone numbers, and a 2500mAh battery to last a good long while.

Our mission is to teach people how everyday electronics work by having them build their own version of a device. In the kit, people learn how phones work, how keypads work, and how to code in the Arduino programming language.


The code can be found in the file `MakoSystem.ino`, and you can upload, edit, or hack the code as much as you want! If you add a new feature to your Phone by Mako, add a file in the Features folder.

<p align="center">
  <br>
  <img width="100px" src="http://phonebymako.com/logo-4.png">
</p>


# How to install the code
## Downloading the code
First, install the Arduino IDE from https://www.arduino.cc/en/Main/Software. Download and install the Arduino file for your operating system. Download the file `MakoSystem.ino` above, and open the Arduino IDE. In the IDE, click `File>Open`, and open the code for `MakoSystem.ino`. If prompted, move into a subfolder. 

## Installing the libraries and boards
To install the library for the `Adafruit Feather FONA`, go to `File>Preferences`, and where it says `Additional Boards Manager URLs`, type the following URL: https://adafruit.github.io/arduino-board-index/package_adafruit_index.json. Also, download the `U8Glib.zip` and `Keypad.zip` files from the `Libraries` folder above. Move these files into your documents folder, and into a folder titled `Arduino`. Inside, there should be a folder called `Libraries`. Put these files there and unzip them. Back in the Arduino IDE, click on `Sketch>Include Libraries>Manage Libraries`. Search for `FONA` and install the latest version of the `Adafruit FONA Library`.

## Uploading the code
To upload code, press `Tools>Board` and make sure `Adafruit Feather 32u4` is selected. `Tools>Programmer` should be set to `USBtinyISP`. Finally, the port in `Tools>Port` shouldn't be blank. Choose whichever option is available. To complete the upload, click `Sketch>Upload` and the code should start running on your Phone by Mako! If it doesn't, you can contact <a href="mailto:help@phonebymako.com">help@phonebymako.com</a>.
