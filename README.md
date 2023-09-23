<p align="center"><svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 595.276 841.89" fill-rule="evenodd" height="841.89pt" width="595.276pt">
<g>
<path style="stroke:none;fill: #3771c2;" d="M 135 609.89C 145.303 609.89 164.309 614.249 164.309 614.249C 164.31 614.25 146 631.89 146 631.89C 146 631.89 148.687 626.236 146 627.89C 132.876 635.964 116.891 658.157 116.891 658.157L 114 651.89C 108.804 657.769 104.739 664.541 102 671.89C 99.1406 679.561 98 695.89 98 695.89C 98 695.89 92.9535 684.471 92 687.89C 88.8569 699.16 91.7148 719.736 96 734.89C 97.0132 738.473 80.8168 714.488 82 717.89C 91.5921 745.471 107.619 765.954 108 767.89C 100.807 719.487 130 681.89 130 681.89C 137.849 672.484 144.859 662.471 151 651.89C 155.495 644.144 159.497 636.14 163 627.89C 159.631 637.903 155.304 647.583 150 656.89C 146.529 662.98 142.834 668.021 139 673.89C 128.877 689.385 121.166 705.361 118 715.89C 113.663 730.314 111.397 747.564 113 768.89C 113.803 779.579 115.673 790.141 118.592 800.522C 94.675 795.618 73.1203 782.647 58.3622 763.212C 47.9625 749.516 39.8604 730.956 38 705.89C 38 652.87 81.4284 609.89 135 609.89z"/>
<path style="stroke:none; fill:#50ade6" d="M 173 589.89C 171.291 590.539 181.231 613.201 178 638.89C 176.778 648.607 174.054 657.943 170 666.89L 173 666.89C 173 666.89 156.051 695.439 141 706.89L 151 703.89C 151 703.89 143.849 723.542 125 751.89C 125 751.89 145.898 728.179 146 729.89C 139.473 738.999 133.724 748.396 129 757.89C 125.022 765.885 122.738 772.172 122 779.89C 120.99 790.451 123.168 801.002 128 810.89C 125.836 815.617 125.86 820.802 125 825.89C 124.133 831.02 130.409 816.901 136 807.89C 134.667 805.89 133.333 803.89 132 801.89C 176.074 802.57 214.954 773.871 227 732.89C 238.242 694.643 224.126 652.381 191 627.89C 188.588 621.073 185.944 614.462 183 607.89C 179.747 600.628 174.255 589.413 173 589.89z"/>
</g>
</svg></p>
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
