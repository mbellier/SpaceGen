QT += core gui opengl

CONFIG += console

INCLUDEPATH += F:\Qt\glm

SOURCES += \
    main.cpp \
    morton.cpp \
    prng.cpp \
    starfield.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    fpscamera.cpp \
    texture.cpp \
    icosphere.cpp

HEADERS += \
    fpscamera.hpp \
    glwidget.hpp \
    mainwindow.hpp \
    prng.hpp \
    morton.hpp \
    starfield.hpp \
    texture.hpp \
    icosphere.hpp

OTHER_FILES += \
    assets/shaders/vertexShader.vsh \
    assets/shaders/fragmentShader.fsh

RESOURCES += \
    resources.qrc
