# Flypen
c++ 课程设计作业
## Requirement
### [Drogon](https://drogonframework.github.io/drogon-docs/#/CHN-01-%E6%A6%82%E8%BF%B0) 

Drogon是一个基于C++17/20的Http应用框架，使用Drogon可以方便的使用C++构建各种类型的Web应用服务端程序。
* Environment
  ``` 
  sudo pacman -S gcc g++ cmake git 
  ```
  
* jsoncpp
  ```
  sudo pacman -S jsoncpp
  ```
* openssl
  ```
  sudo pacman -S openssl
  ```
### MySQL
```
sudo pacman -S mysql
```
数据库表格样式为
| ID | username | password | createtime |
| :-----| ----: | :----: | :----: |
| 1 | sxdx-123456 | 123456 | 1234-5-6 |
### [mysql-connecter-cpp](https://dev.mysql.com/downloads/connector/cpp/)
```bash
#下载 
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit-debug.tar.gz
#可忽略相关文件已经包含在代码中
```


## Install
```bash
git clone https://github.com/lglglglgy/flypen.git
cd flypen
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
mkdir build
cmake ..
make -j 8
./flypen
```
