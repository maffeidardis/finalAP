# Advanced Programming
Final Project of the Advanced Programming class at <i><b>ESPCI Paris</b></i>, 2023.

## Introduction
The goal was to take picutres from each client connected to a server, that hosts a webserver. Then, show the picture of all conecteds clients in this website.

There are some Python dependencies, which can be installed using the following command:

## Setup

To run this project, follow this:
```
$ git clone https://github.com/maffeidardis/finalAP.git
$ pip install -r requirements.txt
$ python website/main.py
$ cd server_structure
$ make
$ ./server
$ ./client localhost
```

It's necessary to run the <b>main.py</b> follow inside the <b>website</b> folder. Right after, you can run your <b>server.cpp</b> file to initiate a server and, then, run the <b>client.cpp</b> (try to run it for <b>localhost</b>).