cmake_minimum_required(VERSION 3.22.1)

project("studioengine")

set(BSE_DIR ../../../../../)
include_directories(${BSE_DIR})

add_library( # Sets the name of the library.
        studioengine
        SHARED
        ${BSE_DIR}/Sources/BaseLibrary/LogManager.cpp
        ${BSE_DIR}/Sources/BaseLibrary/TaskQueue.cpp
        ${BSE_DIR}/Sources/BaseLibrary/DeviceManager.cpp
        ${BSE_DIR}/Sources/Project/MediaFileManager.cpp
        ${BSE_DIR}/Sources/Project/EffectLibrary.cpp
        ${BSE_DIR}/Sources/Project/ProjectManager.cpp
        ${BSE_DIR}/Sources/Actions/ProjectActions.cpp
        ${BSE_DIR}/Sources/Actions/AudioDeviceActions.cpp
        ${BSE_DIR}/Sources/Actions/TransportActions.cpp
        ${BSE_DIR}/Sources/Actions/AudioTrackActions.cpp
        ${BSE_DIR}/Sources/Actions/AudioItemActions.cpp
        BSEProject.cpp
        BSEAudioDevice.cpp
        BSETransport.cpp
        BSEAudioTrack.cpp
        BSEAudioItem.cpp)

find_library(log-lib log)

target_link_libraries(studioengine aaudio mediandk ${log-lib})
