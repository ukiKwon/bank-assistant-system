#2.ocv_modules Unknown
issue#1을 수정한답시고 지워버린 코드로 인하여 추후에 ocv_modules_unkwon 에러가 발생했다고 짐작?정도
그래서 원상복귀하기로 하고. 조금 더 신경써서 고쳐보기로. 결국에는 그냥 도찐 개찐. 그래도 그냥 이렇게 수정을 함.

- opencv_contrib-4.0.0/moduls/CMakeLists.txt  
```
# opencv_contrib-4.0.0
add_definitions(-D__OPENCV_BUILD=1)

message("src_dir : ${OpenCV_SOURCE_DIR}")
if(NOT OPENCV_MODULES_PATH)
  set(OPENCV_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if(NOT DEFINED OPENCV_MODULES_PATH)
  ocv_glob_modules(${OPENCV_MODULES_PATH})
else()
  message("opencv_contrib has already OPENCV_MODULES_PATH")
endif()

if(NOT DEFINED OPENCV_EXTRA_MODULES_PATH)
  ocv_glob_modules(${OPENCV_EXTRA_MODULES_PATH})
else()
  message("opencv_contrib has already OPENCV_EXTRA_MODULES_PATH")
endif()

#ocv_glob_modules(${OPENCV_MODULES_PATH} ${OPENCV_EXTRA_MODULES_PATH})

# build lists of modules to be documented
set(OPENCV_MODULES_MAIN "")
set(OPENCV_MODULES_EXTRA "")

foreach(mod ${OPENCV_MODULES_BUILD} ${OPENCV_MODULES_DISABLED_USER} ${OPENCV_MODULES_DISABLED_AUTO} ${OPENCV_MODULES_DISABLED_FORCE})
  string(REGEX REPLACE "^opencv_" "" mod "${mod}")
  if("${OPENCV_MODULE_opencv_${mod}_LOCATION}" STREQUAL "${OpenCV_SOURCE_DIR}/modules/${mod}")
    list(APPEND OPENCV_MODULES_MAIN ${mod})
  else()
    list(APPEND OPENCV_MODULES_EXTRA ${mod})
  endif()
endforeach()
ocv_list_sort(OPENCV_MODULES_MAIN)
ocv_list_sort(OPENCV_MODULES_EXTRA)
set(FIXED_ORDER_MODULES core imgproc imgcodecs videoio highgui video calib3d features2d objdetect dnn ml flann photo stitching)
list(REMOVE_ITEM OPENCV_MODULES_MAIN ${FIXED_ORDER_MODULES})
set(OPENCV_MODULES_MAIN ${FIXED_ORDER_MODULES} ${OPENCV_MODULES_MAIN})

set(OPENCV_MODULES_MAIN ${OPENCV_MODULES_MAIN} CACHE INTERNAL "List of main modules" FORCE)
set(OPENCV_MODULES_EXTRA ${OPENCV_MODULES_EXTRA} CACHE INTERNAL "List of extra modules" FORCE)

message("opencv_contrib_main modules: ${OPENCV_MODULES_MAIN} ")
message("opencv_contrib_extra modules: ${OPENCV_MODULES_EXTRA} ")

```
#uki408-test
if (OPENCV_EXTRA_MODULES_PATH)
  message(STATUS "opencv_contrib_extra modules_path : ${OPENCV_EXTRA_MODULES_PATH}")
else()
  message("opencv has no variable,")
endif()

if (OPENCV_MODULES_PATH)
  message(STATUS "opencv_contrib_modules_path : ${OPENCV_EXTRA_MODULES_PATH}")
else()
  message("opencv has no variable,")
endif()
`
- 결과
```
-- opencv extra modules_path : /home/uki408/opencv/opencv_contrib-4.0.0/modules
-- opencv_modules_path : /home/uki408/opencv/opencv-4.0.0/modules

-- opencv_contrib_extra modules_path : /home/uki408/opencv/opencv_contrib-4.0.0/modules
-- opencv_contrib_modules_path : /home/uki408/opencv/opencv_contrib-4.0.0/modules

```  

- 프로세스
  (1) opencv를 먼저 cmake를 돌린 후, external_path에 있는 모듈들을 컴파일 하기 위하여 접근함.

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
