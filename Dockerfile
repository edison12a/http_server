# Use Ubuntu latest as the base image
FROM ubuntu:latest

# Set environment variables for Conan
ENV CONAN_USER_HOME=/root/.conan
ENV CONAN_TEMP_HOME=/tmp/.conan

# Get the dependencies for Conan
RUN apt-get update && apt-get install -y \
    curl \
    git \
    cmake \
    g++ \
    python3 \
    python3-pip

RUN apt-get -y update && apt-get -y install git g++ make cmake libssl-dev

#Install Conan
RUN pip install conan \
    && conan profile new default --detect \
    && conan profile update settings.compiler.cppstd=17 default \
    && conan profile update settings.compiler.libcxx=libstdc++11 default \
    && conan install -r conancenter poco/1.12.4@ --build=missing

# Update apps on the base image
RUN apt-get -y update && apt-get install -y
# install python3 and pip
RUN apt-get update && apt-get install -y python3 python3-pip
# install curl
RUN apt-get update && apt-get install -y curl
# install nano
RUN apt-get update && apt-get install -y nano
# install golang
RUN apt-get update && apt-get install -y golang-go
# list all files in current directory
# RUN ls -la
# list all directories in global path
# RUN python3 -c 'import sys;print(sys.argv[1].replace(":","\n"))' "$PATH"
# install more tools
RUN apt-get install -y curl zip unzip tar wget
# Install the Clang compiler
RUN apt-get -y install clang

# install s3 sdk for c++
RUN apt-get update && apt-get install -y cmake
RUN apt-get update && apt-get install -y git
RUN apt-get update && apt-get install -y zlib1g-dev libssl-dev libcurl4-openssl-dev
RUN git clone --depth=1 --recurse-submodules --remote-submodules https://github.com/aws/aws-sdk-cpp.git
RUN cd aws-sdk-cpp && mkdir build && cd build && cmake .. -DBUILD_ONLY="s3" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF && make && make install
# install go-migrate
RUN curl -L https://github.com/golang-migrate/migrate/releases/download/v4.15.2/migrate.linux-amd64.tar.gz --output migrate_dir
RUN tar -xvf migrate_dir
RUN mv migrate /usr/local/bin/migrate

# install python3 requirements
COPY requirements.txt .
# Install the dependencies
RUN pip install -r requirements.txt

#Copy the required files into a folder in the container
COPY http_server.cc .
COPY conanfile.txt  .
COPY CMakeLists.txt .
RUN mkdir build \
    && cd build \
    && conan install .. --build=missing \
    && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release \
    && cmake --build .

#Not required for the project. Only for testing
EXPOSE 9090
CMD ["./build/bin/http_server"]
