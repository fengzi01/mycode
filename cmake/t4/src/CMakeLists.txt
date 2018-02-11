INCLUDE_DIRECTORIES(/tmp/include/hello)
LINK_DIRECTORIES(/tmp/usr/lib/)
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main hello)

