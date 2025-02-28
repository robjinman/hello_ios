Hello World iOS
===============

Barebones iOS app.

Build
-----

```
    cmake --workflow --preset=ios-debug
```

Install
-------

Plug the device into a USB port and obtain the device ID with

```
    xcrun devicectl list devices
```

Install the app on the device

```
    xcrun devicectl device install app --device <device-id> ./build/ios/debug/Debug-iphoneos/hello.app
```
