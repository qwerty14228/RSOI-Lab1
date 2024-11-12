FROM gcc:11

RUN apt-get update -y && apt-get install -y python3-pip cmake
RUN pip3 install conan

RUN mkdir /app
ADD CMakeLists.txt conanprofile.txt conanfile.txt /app/

WORKDIR /app

RUN conan profile detect --force
RUN conan install . --output-folder=. --build=gtest/1.15.0 --profile:build=conanprofile.txt

ADD src /app/src

RUN cd build && . Release/generators/conanbuild.sh && cmake .. --preset conan-release && cd Release && make

CMD ["/app/build/Release/lab_rsoi1"]
