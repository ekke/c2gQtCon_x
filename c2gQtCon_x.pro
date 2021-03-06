# ekke (Ekkehard Gentz) @ekkescorner
TEMPLATE = app
TARGET = c2gQtCon_x

QT += qml quick core network
CONFIG += c++11

# QT_NO_DEPRECATED_WARNINGS will show no deprecated warnings
# comment out and build-clean-project to see them all
# should be done from time to time
# while preparing src for Qt6
DEFINES += QT_NO_DEPRECATED_WARNINGS

HEADERS += \
    cpp/applicationui.hpp \
    cpp/uiconstants.hpp \
    cpp/gen/SettingsData.hpp \ 
    cpp/gen/Bookmark.hpp \
    cpp/gen/Building.hpp \
    cpp/gen/Conference.hpp \
    cpp/gen/DataManager.hpp \
    cpp/gen/Day.hpp \
    cpp/gen/Favorite.hpp \
    cpp/gen/Floor.hpp \
    cpp/gen/PersonsAPI.hpp \
    cpp/gen/Room.hpp \
    cpp/gen/ScheduleItem.hpp \
    cpp/gen/Session.hpp \
    cpp/gen/SessionAPI.hpp \
    cpp/gen/SessionTrack.hpp \
    cpp/gen/Speaker.hpp \
    cpp/gen/SpeakerAPI.hpp \
    cpp/gen/SpeakerImage.hpp \
    cpp/datautil.hpp \
    cpp/imageloader.hpp \
    cpp/gen/SessionLink.hpp \
    cpp/gen/SessionLinkAPI.hpp \
    cpp/gen/SessionLists.hpp \
    cpp/dataserver.hpp

SOURCES += cpp/main.cpp \
    cpp/applicationui.cpp \
    cpp/gen/SettingsData.cpp \ 
    cpp/gen/Bookmark.cpp \
    cpp/gen/Building.cpp \
    cpp/gen/Conference.cpp \
    cpp/gen/DataManager.cpp \
    cpp/gen/Day.cpp \
    cpp/gen/Favorite.cpp \
    cpp/gen/Floor.cpp \
    cpp/gen/PersonsAPI.cpp \
    cpp/gen/Room.cpp \
    cpp/gen/ScheduleItem.cpp \
    cpp/gen/Session.cpp \
    cpp/gen/SessionAPI.cpp \
    cpp/gen/SessionTrack.cpp \
    cpp/gen/Speaker.cpp \
    cpp/gen/SpeakerAPI.cpp \
    cpp/gen/SpeakerImage.cpp \
    cpp/datautil.cpp \
    cpp/imageloader.cpp \
    cpp/gen/SessionLink.cpp \
    cpp/gen/SessionLinkAPI.cpp \
    cpp/gen/SessionLists.cpp \
    cpp/dataserver.cpp

OTHER_FILES +=  qml/main.qml \
    qml/common/*.qml \
    qml/navigation/*.qml \
    qml/pages/*.qml \
    qml/popups/*.qml \
    qml/tabs/*.qml

OTHER_FILES += images/black/*.png \
    images/black/x18/*.png \
    images/black/x36/*.png \
    images/black/x48/*.png \
    images/white/*.png \
    images/white/x18/*.png \
    images/white/x36/*.png \
    images/white/x48/*.png \
    images/extra/*.png \
    translations/*.* \
    data-assets/*.json \
    data-assets/prod/*.json \
    data-assets/test/*.json \
    data-assets/conference/*.json \
    data-assets/conference/speakerImages/*.* \
    data-assets/conference/floorplan/*.png \
    images/LICENSE \
    LICENSE \
    *.md

RESOURCES += qml.qrc \
    translations.qrc \
    images.qrc \
    data-assets.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# T R A N S L A T I O N S

# if languages are added:
# 1. rebuild project to generate *.qm
# 2. add existing .qm files to translations.qrc

# if changes to translatable strings:
# 1. Run Tools-External-Linguist-Update
# 2. Run Linguist and do translations
# 3. Build and run on iOS and Android to verify translations
# 4. Optional: if translations not done: Run Tools-External-Linguist-Release

# Supported languages
LANGUAGES = de en

# used to create .ts files
 defineReplace(prependAll) {
     for(a,$$1):result += $$2$${a}$$3
     return($$result)
 }
# Available translations
tsroot = $$join(TARGET,,,.ts)
tstarget = $$join(TARGET,,,_)
TRANSLATIONS = $$PWD/translations/$$tsroot
TRANSLATIONS += $$prependAll(LANGUAGES, $$PWD/translations/$$tstarget, .ts)
# run LRELEASE to generate the qm files
qtPrepareTool(LRELEASE, lrelease)
 for(tsfile, TRANSLATIONS) {
     command = $$LRELEASE $$tsfile
     system($$command)|error("Failed to run: $$command")
 }

DISTFILES += \
    gen-model/README.md \
    gen-model/*.pdf \
    gen-model/*.txt \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    winrt/*.appxmanifest \
    winrt/assets/*.png

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    # deploying 32-bit and 64-bit APKs you need different VersionCode
    # here's my way to solve this - per ex. Version 1.2.3
    # aabcddeef aa: 21 (MY_MIN_API), b: 0 (32 Bit) or 1 (64 Bit)  c: 0 (unused)
    # dd: 01 (Major Release), ee: 02 (Minor Release), f:  3 (Patch Release)
    # VersionName 1.2.3
    # VersionCode 32 Bit: 210001023
    # VersionCode 64 Bit: 211001023
    defineReplace(droidVersionCode) {
        segments = $$split(1, ".")
        for (segment, segments): vCode = "$$first(vCode)$$format_number($$segment, width=2 zeropad)"
        equals(ANDROID_ABIS, arm64-v8a): \
            prefix = 1
        else: equals(ANDROID_ABIS, armeabi-v7a): \
            prefix = 0
        else: prefix = 2
        # add more cases as needed
        return($$first(prefix)0$$first(vCode))
    }
    MY_VERSION = 1.4
    MY_PATCH_VERSION = 0
    MY_MIN_API = 21
    ANDROID_VERSION_NAME = $$MY_VERSION"."$$MY_PATCH_VERSION
    ANDROID_VERSION_CODE = $$MY_MIN_API$$droidVersionCode($$MY_VERSION)$$MY_PATCH_VERSION

    # find this in shadow build android-build gradle.properties
    ANDROID_MIN_SDK_VERSION = "21"
    ANDROID_TARGET_SDK_VERSION = "29"
}

ios {
    QMAKE_INFO_PLIST = ios/Info.plist
    ios_icon.files = $$files($$PWD/ios/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    ios_artwork.files = $$files($$PWD/ios/iTunesArtwork*.png)
    QMAKE_BUNDLE_DATA += ios_artwork
    app_launch_images.files = $$files($$PWD/ios/LaunchImage*.png)
    QMAKE_BUNDLE_DATA += app_launch_images
    app_launch_screen.files = $$files($$PWD/ios/MyLaunchScreen.xib)
    QMAKE_BUNDLE_DATA += app_launch_screen

    QMAKE_IOS_DEPLOYMENT_TARGET = 12.0

    disable_warning.name = GCC_WARN_64_TO_32_BIT_CONVERSION
    disable_warning.value = NO
    QMAKE_MAC_XCODE_SETTINGS += disable_warning

    # Note for devices: 1=iPhone, 2=iPad, 1,2=Universal.
    QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2
}

winrt {
    WINRT_MANIFEST.name = Qt Con 2016
    WINRT_MANIFEST.background = white
    WINRT_MANIFEST.default_language = en
    WINRT_MANIFEST.description = QtCon 2016 Conference App
    # getting manifest error: WINRT_MANIFEST.identity = org.ekkescorner.c2g.qtcon
    WINRT_MANIFEST.version = 1.4.0.0
	
    WINRT_MANIFEST = winrt/myPackage.appxmanifest
}

# do not move to other location in .pro
# otherwise build settings can loose the info
android: include(/Applications/daten/_android/android-sdk-26_1_1/android_openssl/openssl.pri)
