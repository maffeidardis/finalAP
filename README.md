# Advanced Programming
Final Project of the Advanced Programming class at <i><b>ESPCI Paris</b></i>, 2023.

## Introduction
The goal was to take picutres from each client connected to a server, that hosts a webserver. Then, show the picture of all conecteds clients in this website.

## Setup

To run this project, you should open, at least, three terminals. One is going to be used for the website deploy, the other one to run the server and, the third one, to run a client, but you can run as many as you want.

So, on the first terminal, we are going to run the server.

```
$ git clone https://github.com/maffeidardis/finalAP.git
$ cd finalAP
$ cd server_structure
$ make
$ ./server
```

If you have any problem running the make, you should probably take a look on the server.cpp file. If you are running the code on a Linux server, you should uncomment the following code line:

```
//#include <bits/stdc++.h>
```


On the second one, we are going to run our first client. But, again, you can run as many as you want.

```
$ cd server_structure
$ ./client localhost
```

On the third one, we are going to run our website.
```
$ cd website
$ pip install -r requirements.txt
$ python website/main.py
```
