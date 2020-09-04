# Tiny-Serial
A small ≈100 loc serial monitor
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
