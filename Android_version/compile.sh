PATH="/opt/android/android-sdk:$PATH"
PATH="/opt/android/android-sdk/tools:$PATH"
PATH="/opt/android/android-sdk/platform-tools:$PATH"
PATH="/opt/android/android-sdk/android-ndk-r18b-linux-x86_64:$PATH"
rm -r ./gen ./obj ./bin

ndk-build -j7
./gradlew build
