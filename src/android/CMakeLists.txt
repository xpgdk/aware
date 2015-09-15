project(aware-android)

add_library(aware-backend STATIC
  monitor_socket.cpp
  announce_socket.cpp
  service.cpp
  detail/client.cpp
  detail/aware_jni.cpp
)

set(AWARE_ANDROID_JAVA_SOURCE
  java/dk/xpg/aware/Aware.java
  java/dk/xpg/aware/AwareContact.java
  java/dk/xpg/aware/AwareAdapter.java
)

add_jar(AwareAndroid ${AWARE_ANDROID_JAVA_SOURCE})

add_library(AwareAndroidJNI SHARED 
    jni/AwareAndroid.cpp
)
target_link_libraries(AwareAndroidJNI aware-backend)
