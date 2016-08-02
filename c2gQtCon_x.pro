# ekke (Ekkehard Gentz) @ekkescorner
TEMPLATE = app
TARGET = c2gQtCon_x

QT += qml quick core network
CONFIG += c++11

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
    cpp/gen/SessionLists.hpp

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
    cpp/gen/SessionLists.cpp

lupdate_only {
    SOURCES +=  qml/main.qml \
    qml/common/*.qml \
    qml/navigation/*.qml \
    qml/pages/*.qml \
    qml/popups/*.qml \
    qml/tabs/*.qml
}

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
    gen-model/README.md
