# Add more folders to ship with the application, here
folder_01.source = qml/QmlTracker
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Avoid auto screen rotation
#DEFINES += ORIENTATIONLOCK

# Needs to be defined for Symbian
#DEFINES += NETWORKACCESS

VERSION = 0.1.303

symbian {
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
    TARGET.UID3 = 0xE024B05A
    TARGET.CAPABILITY += Location
    TARGET.EPOCHEAPSIZE = 0x30000 0x3000000
}
win32 {
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
}

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += location \
    sensors \
    systeminfo

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    cpp/folderlistmodel.cpp \
    cpp/altitudemodel.cpp \
    cpp/speedmodel.cpp \
    cpp/clockmodel.cpp \
    cpp/compassmodel.cpp \
    cpp/monitormodel.cpp \
    cpp/positionmodel.cpp \
    cpp/deviceinfomodel.cpp \
    cpp/main.cpp \
    cpp/mapview.cpp \
    cpp/satellitemodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=

HEADERS += \
    cpp/folderlistmodel.h \
    cpp/altitudemodel.h \
    cpp/speedmodel.h \
    cpp/clockmodel.h \
    cpp/compassmodel.h \
    cpp/monitormodel.h \
    cpp/positionmodel.h \
    cpp/deviceinfomodel.h \
    cpp/mapview.h \
    cpp/satellitemodel.h \
    cpp/helpers.h

RESOURCES += \
    resources.qrc
