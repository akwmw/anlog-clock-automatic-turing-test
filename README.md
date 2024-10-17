# anlog-clock-automatic-turing-test
a automatic turing test write in c
# linux compile command
```
gcc -o clock clock.c `pkg-config --cflags --libs gtk+-3.0` -lm
```
dependency:gtk-3-0

tested in c11 

# Window compile guide

make sure you have mingw64 & gcc install

# Install dependencies
```
pacman -S mingw-w64-x86_64-gtk3

pkg-config --cflags gtk+-3.0
gcc -o clock clock.c `pkg-config --cflags --libs gtk+-3.0` -lm
```
dot bother with symbo link just add.c file to home dir C:\msys64\home

at this point i am confince someone at mircosoft hate C programming cause it was stupidly hard to compile windows

I use this project as practice and challange to write low level c code again and learn wrinting gui in c
casue code wayy too much ai in python

i kind of over estmated my own abilty and compixity of the project

pro: simple to solve
con:some zoomer may not able to pass the test cause [they cannot read anlog cock anymore](https://www.telegraph.co.uk/education/2018/04/24/schools-removing-analogue-clocks-exam-halls-teenagers-unable/).

and who is Kurisu Makise again ?
idk i only watch animate unless brocast on tv,

to do(dead)
- [x]have basic anlog clock
- [x]give up button
- [x]reset button
- [ ]screen shot clock window
- [x]window release
- [ ]write a unit test
- [ ]improve textfield to accept more time format like 9:00 nine clock
- [ ]colour clock face
- [ ]purge unuse depency
