add_library(android_test_jni SHARED EXCLUDE_FROM_ALL
    android_test.cpp
)
target_link_libraries(android_test_jni LINK_PUBLIC AwareAndroidJNI aware)

set(CMAKE_JAR_CLASSES_PREFIX dk/xpg/aware/test)
set(CMAKE_JAVA_JAR_ENTRY_POINT dk.xpg.aware.test.AwareTest)
add_jar(android_test
    SOURCES AwareTest.java
    INCLUDE_JARS AwareAndroid
)

add_dependencies(android_test android_test_jni)

add_custom_target(run_android_test
    COMMAND ${Java_JAVA_EXECUTABLE} -Djava.library.path=${CMAKE_BINARY_DIR}/${LIBRARY_OUTPUT_PATH}:${CMAKE_CURRENT_BINARY_DIR}/lib -classpath $<TARGET_PROPERTY:AwareAndroid,JAR_FILE>:$<TARGET_PROPERTY:android_test,JAR_FILE> dk.xpg.aware.test.AwareTest
    DEPENDS android_test)
