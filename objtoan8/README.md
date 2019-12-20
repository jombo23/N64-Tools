Official release of objtoan8

Builds are located in /bin

If building yourself - 
The VS2008 Sln uses FBX 2014.1, and Assimp 3.  Unzip assimp.zip to build that version.
FBX SDK 2014.1: http://images.autodesk.com/adsk/files/fbx20141_fbxsdk_vs2008_win.exe
Copy C:\Program Files\Autodesk\FBX\FBX SDK\2014.1\lib\vs2008\x64\release\libfbxsdk.dll to obj2an8 folder. Or x86 folder if 32-bit.

The VS2013 V12 Sln uses FBX 2020.0.1, and Assimp 5.0.0.  Unzip assimp5_0_0.zip to build that version.  All exe generated and folders will have a v12 appended to name.
FBX SDK 2014.1: https://www.autodesk.com/content/dam/autodesk/www/adn/fbx/2020-0-1/fbx202001_fbxsdk_vs2013_win.exe
Copy C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1\lib\vs2013\x64\releaselibfbxsdk.dll to obj2an8 folder. Or x86 folder if 32-bit.

Both use the same source files just different assimp and FBX SDK.
