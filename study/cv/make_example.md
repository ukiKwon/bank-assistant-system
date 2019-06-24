#컴파일 방법
//매번 CMakeLists.txt 만들기는 불편
(1) 방법1. g++ facedetect.cpp -o facedetect `pkg-config --libs opencv4`
(2) 방법2. g++ -o facedetect /usr/local/share/opencv4/samples/cpp/facedetect.cpp $(pkg-config opencv4 --libs --cflags) -std=c++11
초기 OpenCV 컴파일할 때 준 옵션에서 `-D OPENCV_GENERATE_PKGCONFIG=ON \
`이런 옵션에 의해서 가능함.

#BUILD 폴더 내에 아래 명령어 실행 이후부터 opencv 소스 코드 빌드 가능해짐
@ver1_8
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=OFF \
-D INSTALL_C_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=/home/uki408/opencv/opencv_contrib-4.0.0/modules  \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D ENABLE_CXX11=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.5m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so \
../opencv-4.0.0  

@ver1_7
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=/home/uki408/opencv/opencv_contrib-4.0.0/modules  \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D ENABLE_CXX11=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.5m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so \
..  


@ver1_6
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=/home/uki408/opencv/opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D ENABLE_CXX11=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.5m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include/\;  \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so \
..

@ver1_5
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.5m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include/  \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so \
..

@ver1_4
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
..

@ver1_3
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=/home/uki408/opencv/opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D CMAKE_CXX_FLAGS="-std=c++11" \
-D WITH_TBB=ON \
../opencv-4.0.0/

@ver1_2
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=/home/uki408/opencv/opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D WITH_TBB=ON \
../opencv-4.0.0/


@ver1_1
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D WITH_TBB=ON \
-D DENABLE_CXX11=ON \
../opencv-4.0.0/

@ver1_0
cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=ON \
-D BUILD_opencv_python3=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.0.0/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D WITH_TBB=ON \
../opencv-4.0.0/
