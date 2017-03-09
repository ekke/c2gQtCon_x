# includes openssl libs onto android build
android {
  ANDROID_EXTRA_LIBS += $$PWD/../../android-openssl-qt/prebuilt/armeabi-v7a/libcrypto.so
  ANDROID_EXTRA_LIBS += $$PWD/../../android-openssl-qt/prebuilt/armeabi-v7a/libssl.so
}
