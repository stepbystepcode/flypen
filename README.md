<p align="center">
<img src="https://github-production-user-asset-6210df.s3.amazonaws.com/57084184/270096729-18a461fc-e7ed-4bda-881d-da808878d525.svg"/>
</p>
<h2 align="center">Flypen</h2>
<p align="center">
  <a href="https://raw.githubusercontent.com/stepbystepcode/flypen/main/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="LICENSE"></a>
  <a href="https://github.com/topics/cpp"><img src="https://img.shields.io/badge/language-c++-blue.svg" alt="languange"></a>
</p>

*Read this in other languages: [English](README.md), [简体中文](README.zh_Hans.md).*

## C++ Course Design
### An Office Collab Backend
This is the Flypen C++ backend service for the Office Collab web application.

Frontend Project for Flypen is [Here](https://github.com/stepbystepcode/flypen-frontend)

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

### Installation (Automatic)
```bash
sudo apt install git gcc g++ cmake libjsoncpp-dev uuid-dev zlib1g-dev openssl libssl-dev
git clone https://github.com/stepbystepcode/flypen.git --recursive
cd flypen
./run.sh
```

### Installation (Manual) 
<details>
<summary>Not Recommended</summary>
<br><br>
<pre>
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
</pre>
</details>



## API Documentation
See [API.md](API.md) for details on APIs for authentication, user management, documents, etc.

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Contact
For any questions, please reach out at 202202501110@email.sxu.edu.cn or 202201005113@email.sxu.edu.cn
