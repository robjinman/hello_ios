Hello World iOS
===============

Barebones iOS app.

Get the iOS toolchain
---------------------

This was set up like this

```
    git remote add -f ios-cmake https://github.com/leetal/ios-cmake.git
    git subtree add --prefix ios_cmake ios-cmake master --squash
```

To update the toolchain

```
    git fetch ios-cmake master
    git subtree pull --prefix ios_cmake ios-cmake master --squash
```
