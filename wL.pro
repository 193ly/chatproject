#-------------------------------------------------
#
# Project created by QtCreator 2023-07-25T09:28:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wL
TEMPLATE = app

INCLUDEPATH += ./INET
INCLUDEPATH += ./INETMEDIATOR
INCLUDEPATH += ./cryptopp


LIBS += -lWs2_32

#include(./cryptopp/cryptopp.pri)

#INCLUDEPATH += $$PWD/cryptopp/cryptopp565-release/include

#LIBS += -L$$PWD/cryptopp/cryptopp565-release/bin/ -lcryptopp565

LIBS += -L$$PWD/openssl/lib/ -llibssl


LIBS += -L$$PWD/openssl/lib/ -llibcrypto


INCLUDEPATH += $$PWD/openssl/include
DEPENDPATH += $$PWD/openssl/include

SOURCES += main.cpp\
        dialog.cpp \
    INETMEDIATOR/MidINet.cpp \
    INETMEDIATOR/MidTcpNetClient.cpp \
    INETMEDIATOR/MidTcpNetSever.cpp \
    INETMEDIATOR/MidUdpNet.cpp \
    INET/INet.cpp \
    INET/TcpNetClient.cpp \
    INET/TcpNetSever.cpp \
    INET/UdpNet.cpp \
    ckernel.cpp \
    useritem.cpp \
    chatdialog.cpp \
    logindialog.cpp \
    groupitem.cpp \
    sendfilethread.cpp \
    filerecvdialog.cpp \
    fileshowdialog.cpp \
    mycrypt/myaes.cpp \
    mycrypt/myrsa.cpp \
    applink.c


HEADERS  += dialog.h \
    INETMEDIATOR/MidINet.h \
    INETMEDIATOR/MidTcpNetClient.h \
    INETMEDIATOR/MidTcpNetSever.h \
    INETMEDIATOR/MidUdpNet.h \
    INET/def.h \
    INET/INet.h \
    INET/TcpNetClient.h \
    INET/TcpNetSever.h \
    INET/UdpNet.h \
    ckernel.h \
    useritem.h \
    chatdialog.h \
    logindialog.h \
    groupitem.h \
    sendfilethread.h \
    filerecvdialog.h \
    fileshowdialog.h \
    mycrypt/myaes.h \
    mycrypt/myrsa.h

FORMS    += dialog.ui \
    useritem.ui \
    chatdialog.ui \
    logindialog.ui \
    groupitem.ui \
    filerecvdialog.ui \
    fileshowdialog.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    openssl/lib/MinGW/libcrypto.dll.a \
    openssl/lib/MinGW/libssl.dll.a \
    openssl/lib/VC/static/libcrypto32MD.lib \
    openssl/lib/VC/static/libcrypto32MDd.lib \
    openssl/lib/VC/static/libcrypto32MT.lib \
    openssl/lib/VC/static/libcrypto32MTd.lib \
    openssl/lib/VC/static/libssl32MD.lib \
    openssl/lib/VC/static/libssl32MDd.lib \
    openssl/lib/VC/static/libssl32MT.lib \
    openssl/lib/VC/static/libssl32MTd.lib \
    openssl/lib/VC/libcrypto32MD.lib \
    openssl/lib/VC/libcrypto32MDd.lib \
    openssl/lib/VC/libcrypto32MT.lib \
    openssl/lib/VC/libcrypto32MTd.lib \
    openssl/lib/VC/libssl32MD.lib \
    openssl/lib/VC/libssl32MDd.lib \
    openssl/lib/VC/libssl32MT.lib \
    openssl/lib/VC/libssl32MTd.lib \
    openssl/lib/capi.lib \
    openssl/lib/dasync.lib \
    openssl/lib/ec_internal_test.lib \
    openssl/lib/libapps.lib \
    openssl/lib/libcrypto.lib \
    openssl/lib/libcrypto_static.lib \
    openssl/lib/libssl.lib \
    openssl/lib/libssl_static.lib \
    openssl/lib/libtestutil.lib \
    openssl/lib/openssl.lib \
    openssl/lib/ossltest.lib \
    openssl/lib/padlock.lib \
    openssl/lib/uitest.lib \
    cryptopp/cryptopp565-release/bin/debug/cryptopp.dll \
    cryptopp/cryptopp565-release/bin/debug/cryptopp565.dll \
    cryptopp/cryptopp565-release/bin/release/cryptopp565.dll \
    network_3_11_IMÏµÍ³_ËØ²Ä.zip \
    wL.zip \
    openssl/lib/MinGW/libcrypto.def \
    openssl/lib/MinGW/libssl.def \
    openssl/lib/libcrypto.def \
    openssl/lib/libssl.def \
    download/0bc3aqap6aaabqalr2tuffrvabgd74cab7ya.f0.mp4 \
    download/43df428e_Limit_of_expantion_v3.mp4 \
    download/Dva1080.mp4 \
    WLOP Aeolian3 by Eliza Final.mp4
