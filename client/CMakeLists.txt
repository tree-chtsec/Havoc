cmake_minimum_required(VERSION 3.26)
project( HavocClient )

##
## cmake sets
##
set( BINARY_NAME Havoc )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )
set( QT_VERSION 5 )
set( REQUIRED_LIBS Core Gui Widgets Network WebSockets )
set( REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::WebSockets )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY .. )
set( APP_ICON_RESOURCE_WINDOWS "data/Havoc.qrc" )

##
## compiler definition
##
add_definitions( -DQT_NO_DEBUG_OUTPUT )

##
## compiler flags
##
set( CMAKE_CXX_FLAGS "-Wattributes -lssl -lcrypto" )


##
## include header directories
##
include_directories( include )
include_directories( extern/pybind11/include )
include_directories( extern/json/include )
include_directories( extern/spdlog/include )
include_directories( extern/toml )
include_directories( extern/http )

##
## Havoc include
##
set( HAVOC_INCLUDE
        include/Common.h
        include/Havoc.h
        include/Events.h

        include/core/Helper.h
        include/core/EventWorker.h

        include/ui/Connect.h
        include/ui/MainWindow.h
        include/ui/AgentConsole.h
        include/ui/PageAgent.h
        include/ui/PageListener.h
        include/ui/DialogListener.h
)

##
## Havoc src
##
set( HAVOC_SRC
        src/Main.cc
        src/Havoc.cc
        src/Events.cc

        src/core/Helper.cc
        src/core/EventWorker.cc

        src/ui/Connect.cc
        src/ui/MainWindow.cc
        src/ui/AgentConsole.cc
        src/ui/PageAgent.cc
        src/ui/PageListener.cc
        src/ui/DialogListener.cc
)

find_package(OpenSSL REQUIRED)

##
## build Havoc client executable
##
add_executable( ${BINARY_NAME} ${HAVOC_INCLUDE} ${HAVOC_SRC} ${APP_ICON_RESOURCE_WINDOWS} )

##
## Linking
##

##
## python finding and handling
##
find_package( Python 3 COMPONENTS Interpreter Development REQUIRED )
set( PYTHON_MAJOR $ENV{Python_VERSION_MAJOR} )
set( PYTHON_MINOR $ENV{Python_VERSION_MINOR} )
set( PYTHONLIBS_VERSION_STRING ${Python_VERSION} )
set( PYTHON_INCLUDE_DIR ${Python_INCLUDE_DIRS} )
set( PYTHON_LIBRARIES ${Python_LIBRARIES} )

##
## qt finding and handling
##
find_package( Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED )

##
## linking external libraries
##
target_link_libraries(
    ${BINARY_NAME}
    ${REQUIRED_LIBS_QUALIFIED}
    ${PYTHON_LIBRARIES}
    OpenSSL::Crypto
)
