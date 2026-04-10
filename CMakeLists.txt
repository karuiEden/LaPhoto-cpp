cmake_minimum_required(VERSION 3.28)
project(LaPhoto_cpp)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs imgproc)




# 1. Поиск пакета OpenCV
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs imgproc)
# core: базовые функции
# highgui: для imread/imwrite (если вы используете функции imread/imwrite из highgui)
# imgcodecs: для imread/imwrite (современный модуль для кодеков изображений, предпочтительнее highgui для этих целей)
# imgproc: для cvtColor (обработка изображений)

# Проверка, найден ли OpenCV
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV or set OpenCV_DIR.")
endif()

# Вывод найденных библиотек и инклюдов (для отладки)
message(STATUS "OpenCV found. Include dir: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")


# --- Добавление вашего исполняемого файла ---

# Название вашей исполняемой программы
set(EXECUTABLE_NAME process_image)

add_library(CONVERT src/convert.cpp)
target_include_directories(CONVERT PRIVATE include)

add_library(ALGO src/algorithm.cpp)
target_include_directories(ALGO PRIVATE include)

add_library(CHANNEL_MIXER src/channel_mixer.cpp)
target_include_directories(CHANNEL_MIXER PRIVATE include)


add_executable(${EXECUTABLE_NAME} main.cpp)

# Привязка библиотек OpenCV к вашему исполняемому файлу
target_link_libraries(CONVERT ${OpenCV_LIBS})
target_link_libraries(CHANNEL_MIXER ${OpenCV_LIBS})
target_link_libraries(${EXECUTABLE_NAME} CHANNEL_MIXER CONVERT ALGO ${OpenCV_LIBS})

# Указание директорий для поиска заголовочных файлов OpenCV
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
