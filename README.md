<p align="center"><a href="#"><img width="100" src="https://s2.loli.net/2023/09/22/WsZuGM3S49DA8we.jpg" alt="Python logo"></a></p>
<h2 align="center">Flypen</h2>
<p align="center">
  <!-- <a href="https://travis-ci.com/github/ybygjylj/habits-apriori/"><img src="https://travis-ci.com/ybygjylj/habits-apriori.svg?branch=master" alt="Build Status"></a> -->
  <a href="https://raw.githubusercontent.com/ybygjylj/habits-apriori/master/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="LICENSE"></a>
  <a href="https://github.com/topics/cpp"><img src="https://img.shields.io/badge/language-c++-blue.svg" alt="languange"></a>
  <!-- <a href="https://gitter.im/habits-apriori/community"><img src="https://badges.gitter.im/Join%20Chat.svg" alt="chat"></a> -->
</p>

*Read this in other languages: [English](README.md), [简体中文](README.zh_Hans.md).*

## C++ Course Design
### An Office Collab Backend
This is the Flypen C++ backend service for the Office Collab web application.

## Features
- Real-time collaborative editing of documents
- Chat and notifications
- User management and access control
- Document management and storage (markdown)
## Architecture
- Server application built with C++ drogon famework
- MariaDB database for storage
- REST APIs for client communication
- Use MySQL Connector/C++ 8.1.0 connect database
## Getting Started

### Installing

```
git clone https://github.com/stepbystepcode/flypen.git
cd flypen
git submodule update --init
cd drogon
git submodule update --init
sudo apt install git gcc g++ cmake libjsoncpp-dev uuid-dev zlib1g-dev openssl libssl-dev
mkdir build
cd build
cmake ..
make -j 8
sudo make install
cd ../..
cd jwt-cpp
mkdir build
cd build
cmake ..
make -j 8
sudo make install
cd ../..
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz
tar zxvf mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz 
mv mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit mysql-connector
rm mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz
mkdir build
./run.sh
```
## API Documentation
See API.md for details on APIs for authentication, user management, documents, etc.

## Contributing
See CONTRIBUTING.md for contribution guidelines.

## License
This project is licensed under the MIT License. See LICENSE for details.

## Contact
For any questions, please reach out at 202202501110@email.sxu.edu.cn