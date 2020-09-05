# Tiny-Serial
A small ≈100 loc serial monitor, Green text signifies incoming data from the serial port the program has latched onto, any data the user inputs is in white text. Type a lower case q to exit the program. <br><br>
![Screenshot](https://github.com/Rocketbuney/Tiny-Serial/blob/master/img/Screen%20Shot%202020-09-03%20at%207.13.41%20PM.png)
# Compiling
I've only tested this on linux and mac, it doesnt require any dependencies besides the c standard library and linux headers.
```
make
make install
```
# Running
The program requires a a path to the tty device as well as optionally a baudrate (it defaults to 9600)
```
tiny-serial [path to tty device] [optional baudrate]
```

# Uninstalling
```
make uninstall
```
