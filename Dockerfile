FROM ubuntu:20.04 AS qt_from_repo
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y \
    git \
    g++-10 \ 
    qt5-default \
    wget
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-10 100
RUN wget -O - https://raw.githubusercontent.com/alec-chicherini/development-scripts/refs/heads/main/cmake/install_cmake.sh 2>/dev/null | bash

COPY . /calculator-task
RUN cd calculator-task && \
    mkdir build && cd build && \
    cmake .. && \
    cmake --build . && \
    cpack . && \
    mkdir /result && cp *.deb /result/
  
ENTRYPOINT ["ls", "/result"]
