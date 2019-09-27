QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc \
    image.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# libClamAV
LIBS += -L$$PWD/./ -llibclamav

HEADERS += \
    clamav/clamav-config.h \
    clamav/clamav-types.h \
    clamav/clamav.h \
    openssl/aes.h \
    openssl/asn1.h \
    openssl/asn1_mac.h \
    openssl/asn1t.h \
    openssl/bio.h \
    openssl/blowfish.h \
    openssl/bn.h \
    openssl/buffer.h \
    openssl/camellia.h \
    openssl/cast.h \
    openssl/cmac.h \
    openssl/cms.h \
    openssl/comp.h \
    openssl/conf.h \
    openssl/conf_api.h \
    openssl/crypto.h \
    openssl/des.h \
    openssl/des_old.h \
    openssl/dh.h \
    openssl/dsa.h \
    openssl/dso.h \
    openssl/dtls1.h \
    openssl/e_os2.h \
    openssl/ebcdic.h \
    openssl/ec.h \
    openssl/ecdh.h \
    openssl/ecdsa.h \
    openssl/engine.h \
    openssl/err.h \
    openssl/evp.h \
    openssl/hmac.h \
    openssl/idea.h \
    openssl/krb5_asn.h \
    openssl/kssl.h \
    openssl/lhash.h \
    openssl/md4.h \
    openssl/md5.h \
    openssl/mdc2.h \
    openssl/modes.h \
    openssl/obj_mac.h \
    openssl/objects.h \
    openssl/ocsp.h \
    openssl/opensslconf.h \
    openssl/opensslv.h \
    openssl/ossl_typ.h \
    openssl/pem.h \
    openssl/pem2.h \
    openssl/pkcs12.h \
    openssl/pkcs7.h \
    openssl/pqueue.h \
    openssl/rand.h \
    openssl/rc2.h \
    openssl/rc4.h \
    openssl/ripemd.h \
    openssl/rsa.h \
    openssl/safestack.h \
    openssl/seed.h \
    openssl/sha.h \
    openssl/srp.h \
    openssl/srtp.h \
    openssl/ssl.h \
    openssl/ssl2.h \
    openssl/ssl23.h \
    openssl/ssl3.h \
    openssl/stack.h \
    openssl/symhacks.h \
    openssl/tls1.h \
    openssl/ts.h \
    openssl/txt_db.h \
    openssl/ui.h \
    openssl/ui_compat.h \
    openssl/whrlpool.h \
    openssl/x509.h \
    openssl/x509_vfy.h \
    openssl/x509v3.h
