#HTTP INJECTOR SQUID

To build , you need install
```
apt install -y qt5-default build-essential qt5-qmake
```

Then to build
```
qmake
make
```

Then to forward port 80 to 22
```
./main 80 22
```

You can see more options with
```
./main --help
```

To custom first http response, edit file ./src/CustomLogo1.cpp.
And replace
```
return tr(....)
```
For anithing else.

