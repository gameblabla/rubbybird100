Rubby Bird 100
=============

This is a sequel to Rubby Bird 100. In this one, rather than rubbing 1 bird, you've got to rub 100 of them instead !
Do you think you're up to the challenge ?


Dev (Android)
=============


Make sure you have SDL2, SDL2_mixer extracted in their own respective folders in
Android_version/jni/

as SDL2 and SDL2_mixer respectively.

The game requires the SDL_OpenURL patch for Android. 

If you don't have it or can't apply it, just disable the offending code. (or put a stub)

Also, for Android yet again, it only works with OpenJDK 8 for now because Android dev sucks a lot and i'm gay.

Make sure for Android compilation that :

- in Android_version/jni, have 3 folders (SDL2, SDL2_image and SDL2_mixer) and their respective source code.

SDL2 needs to be patched for SDL_OpenURL.

- Make sure the Android-sdk and NDK are installed. Installing the NDK through the SDK tools is recommended (/opt/android/android-sdk by default)

That should do it. Linux compiling is easier, just run make in the folder.
