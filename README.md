<p align="center">
![svg](https://github.com/stepbystepcode/flypen/assets/57084184/18a461fc-e7ed-4bda-881d-da808878d525)<svg width="271" height="271" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd">

 <g>
  <title>Layer 1</title>
  <g id="svg_1">
   <path id="svg_2" fill=" #3771c2" d="m136.09493,36.96145c10.303,0 29.309,4.359 29.309,4.359c0.001,0.001 -18.309,17.641 -18.309,17.641c0,0 2.687,-5.654 0,-4c-13.124,8.074 -29.109,30.267 -29.109,30.267l-2.891,-6.267c-5.196,5.879 -9.261,12.651 -12,20c-2.8594,7.671 -4,24 -4,24c0,0 -5.0465,-11.419 -6,-8c-3.1431,11.27 -0.2852,31.846 4,47c1.0132,3.583 -15.1832,-20.402 -14,-17c9.5921,27.581 25.619,48.064 26,50c-7.193,-48.403 22,-86 22,-86c7.849,-9.406 14.859,-19.419 21,-30c4.495,-7.746 8.497,-15.75 12,-24c-3.369,10.013 -7.696,19.693 -13,29c-3.471,6.09 -7.166,11.131 -11,17c-10.123,15.495 -17.834,31.471 -21,42c-4.337,14.424 -6.603,31.674 -5,53c0.803,10.689 2.673,21.251 5.592,31.632c-23.917,-4.904 -45.4717,-17.875 -60.2298,-37.31c-10.3997,-13.696 -18.5018,-32.256 -20.3622,-57.322c0,-53.02 43.4284,-96 97,-96z"/>
   <path id="svg_3" fill="#50ade6" d="m174.09493,16.96145c-1.709,0.649 8.231,23.311 5,49c-1.222,9.717 -3.946,19.053 -8,28l3,0c0,0 -16.949,28.549 -32,40l10,-3c0,0 -7.151,19.652 -26,48c0,0 20.898,-23.711 21,-22c-6.527,9.109 -12.276,18.506 -17,28c-3.978,7.995 -6.262,14.282 -7,22c-1.01,10.561 1.168,21.112 6,31c-2.164,4.727 -2.14,9.912 -3,15c-0.867,5.13 5.409,-8.989 11,-18c-1.333,-2 -2.667,-4 -4,-6c44.074,0.68 82.954,-28.019 95,-69c11.242,-38.247 -2.874,-80.509 -36,-105c-2.412,-6.817 -5.056,-13.428 -8,-20c-3.253,-7.262 -8.745,-18.477 -10,-18z"/>
  </g>
 </g>
</svg>

</p>
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
See [API.md](API.md) for details on APIs for authentication, user management, documents, etc.

## Contributing
See CONTRIBUTING.md for contribution guidelines.

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Contact
For any questions, please reach out at 202202501110@email.sxu.edu.cn
