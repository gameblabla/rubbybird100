PATH="/opt/android/android-ndk-r12b:$PATH"
PATH="/opt/android/android-sdk-linux/tools:$PATH"
PATH="/opt/android/android-sdk-linux/platform-tools:$PATH"
rm -r ./gen ./obj ./bin

ndk-build -j7
./gradlew build
#ant release 
