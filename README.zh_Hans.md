<p align="center"><a href="#"><img width="100" src="https://s2.loli.net/2023/09/22/WsZuGM3S49DA8we.jpg" alt="Python logo"></a></p>
<h2 align="center">Flypen</h2>
<p align="center">
  <!-- <a href="https://travis-ci.com/github/ybygjylj/habits-apriori/"><img src="https://travis-ci.com/ybygjylj/habits-apriori.svg?branch=master" alt="Build Status"></a> -->
  <a href="https://raw.githubusercontent.com/ybygjylj/habits-apriori/master/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="LICENSE"></a>
  <a href="https://github.com/topics/cpp"><img src="https://img.shields.io/badge/language-c++-blue.svg" alt="languange"></a>
  <!-- <a href="https://gitter.im/habits-apriori/community"><img src="https://badges.gitter.im/Join%20Chat.svg" alt="chat"></a> -->
</p>
*以其他语言阅读： [英文](README.md), [简体中文](README.zh_Hans.md)。

### C++ 课程设计
### 协作软件后端
这是办公协作软件Flypen的C++后端。

### 功能
- 实时协作编辑文档
- 聊天和通知
- 用户管理和访问控制
- 文档管理和存储（markdown）
## 架构
- 使用 C++ drogon famework 构建的服务器应用程序
- 用于存储的 MariaDB 数据库
- 客户端通信使用 REST API
- 使用 MySQL Connector/C++ 8.1.0 连接数据库
### 入门

#### 安装

```
git clone https://github.com/stepbystepcode/flypen.git
cd flypen
git submodule update --init
cd drogon
git submodule update --init
sudo apt install git gcc g++ cmake libjsoncpp-dev uuid-dev zlib1g-dev openssl libssl-dev
mkdir build
cd build
cmake .
make -j 8
sudo make install
cd ./...
cd jwt-cpp
mkdir build
cd build
cmake .
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
## API 文档
请参阅 [API.md](API.md) 了解有关身份验证、用户管理、文档等 API 的详细信息。

## 投稿
请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)，了解贡献指南。

## 许可证
本项目采用 MIT 许可。详见 [LICENSE](LICENSE)。

## 联系
如有任何问题，请联系 202202501110@email.sxu.edu.cn
