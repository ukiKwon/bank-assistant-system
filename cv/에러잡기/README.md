#1.OpenCV has already loaded its modules
```
-- OpenCV Python: during development append to PYTHONPATH: /home/uki408/opencv/build/python_loader
CMake Error at /home/uki408/opencv/opencv-4.0.0/cmake/OpenCVModule.cmake:364 (message):
  OpenCV has already loaded its modules.  Calling ocv_glob_modules second
  time is not allowed.
Call Stack (most recent call first):
  /home/uki408/opencv/opencv_contrib-4.0.0/modules/CMakeLists.txt:7 (ocv_glob_modules)
```

- 내 해석
  ocv_glob_modules 가 두번 불렸기 떄문에 컴파일 에러가 났음. 그래서
  opencv' cmake에서 후작업을 하닥 에러를 호출한 것인데, Call Stack을 따라가보니 the ocv_glob_modules이
  opencv-contrib' CmakeList 에서 선언을 해놨다.

-Try1 -> 성공
  /home/uki408/opencv/opencv_contrib-4.0.0/modules/CMakeLists.txt:7 에서 선언되어 있는
  ocv_glob_modules(${OPENCV_MODULES_PATH} ${OPENCV_EXTRA_MODULES_PATH})
  삭제할 거임
