#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T18:50:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EOSBenchTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    utility/httpclient.cpp \
    utility/utils.cpp \
    account/createaccount.cpp \
    chain/action.cpp \
    chain/chainmanager.cpp \
    chain/eosbytewriter.cpp \
    chain/eosnewaccount.cpp \
    chain/packedtransaction.cpp \
    chain/signedtransaction.cpp \
    chain/transaction.cpp \
    chain/transactionheader.cpp \
    chain/typeaccountpermissionweight.cpp \
    chain/typeauthority.cpp \
    chain/typekeypermissionweight.cpp \
    chain/typename.cpp \
    chain/typepermissionlevel.cpp \
    Crypto/aes.c \
    Crypto/base58.c \
    Crypto/rmd160.c \
    Crypto/sha2.c \
    Crypto/sha3.c \
    Crypto/uECC.c \
    ec/eos_key_encode.cpp \
    ec/sha512.cpp \
    ec/typechainid.cpp \
    account/accountmanager.cpp \
    push/pushmanager.cpp \
    work/datamanager.cpp \
    misc/customtabstyle.cpp \
    misc/aboutframe.cpp \
    work/transactionpool.cpp \
    work/resultcounter.cpp \
    work/packer.cpp

HEADERS  += mainwindow.h \
    setting/config.h \
    utility/httpclient.h \
    utility/utils.h \
    account/createaccount.h \
    chain/action.h \
    chain/chainbase.h \
    chain/chainmanager.h \
    chain/eosbytewriter.h \
    chain/eosnewaccount.h \
    chain/packedtransaction.h \
    chain/signedtransaction.h \
    chain/transaction.h \
    chain/transactionheader.h \
    chain/typeaccountpermissionweight.h \
    chain/typeauthority.h \
    chain/typekeypermissionweight.h \
    chain/typename.h \
    chain/typepermissionlevel.h \
    Crypto/aes.h \
    Crypto/aes.hpp \
    Crypto/libbase58.h \
    Crypto/macros.h \
    Crypto/options.h \
    Crypto/rmd160.h \
    Crypto/sha2.h \
    Crypto/sha3.h \
    Crypto/uECC.h \
    ec/eos_key_encode.h \
    ec/sha512.h \
    ec/typechainid.h \
    account/accountmanager.h \
    push/pushmanager.h \
    work/datamanager.h \
    misc/customtabstyle.h \
    misc/aboutframe.h \
    work/transactionpool.h \
    work/resultcounter.h \
    work/packer.h

FORMS    += mainwindow.ui \
    misc/aboutframe.ui

RESOURCES += \
    resource.qrc
