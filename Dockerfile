# 1. 基础镜像
FROM ubuntu:22.04

# 设置为非交互模式
ENV DEBIAN_FRONTEND=noninteractive

# 2. 安装编译依赖
RUN apt-get update && apt-get install -y \
    git \
    tree \
    gcc \
    g++ \
    cmake \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libssl-dev \
    wget \
    && rm -rf /var/lib/apt/lists/*

# 3. 设置工作目录
WORKDIR /app

# 4. 根据构建架构下载并解压 MySQL Connector
ARG TARGETARCH
RUN MCON_ARCH="" && \
    if [ "$TARGETARCH" = "amd64" ]; then \
      MCON_ARCH="x86-64bit"; \
    elif [ "$TARGETARCH" = "arm64" ]; then \
      MCON_ARCH="aarch64"; \
    else \
      echo "Unsupported architecture: $TARGETARCH" && exit 1; \
    fi && \
    MCON_FILE="mysql-connector-c++-8.1.0-linux-glibc2.28-${MCON_ARCH}.tar.gz" && \
    wget "https://dev.mysql.com/get/Downloads/Connector-C++/${MCON_FILE}" && \
    tar zxvf "${MCON_FILE}" && \
    mv "mysql-connector-c++-8.1.0-linux-glibc2.28-${MCON_ARCH}" mysql-connector && \
    rm "${MCON_FILE}"

# 5. 【新增】将 MySQL Connector 库安装到系统路径
RUN cp /app/mysql-connector/lib64/*.so* /usr/local/lib/ \
    && ldconfig

# 6. 复制所有项目文件到容器
COPY . .

# 7. (可选) 声明git目录安全
RUN git config --global --add safe.directory /app

# 8. 初始化并拉取所有子模块
RUN git submodule update --init --recursive

# 9. 编译并安装 jwt-cpp 依赖
RUN cd jwt-cpp && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

# 10. 编译并安装 drogon 依赖
RUN cd drogon && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install

# 11. 运行主项目的 cmake
RUN cmake .

# 12. 编译主项目
RUN make -j$(nproc)

# 13. 暴露端口并设置启动命令
EXPOSE 8081
CMD ["/app/flypen"]
