Official release of objtoan8

Obj2an8 is a tool designed to allow for external programs such as the GoldenEye Setup Editor to support a simple, text-based import format that supports rigs and animation, compatible with obj, yet still allow export from sophisticated modeling tools using more modern formats such as FBX.  Obj2an8 converts between these formats: "geobj", fbx, bvh, and dae.  Note that dae support is not great due to the use of assimp library and the wide variation in dae format usage among modeling tools.

Builds are located in /bin

If building yourself - 
The VS2008 Sln uses FBX 2014.1, and Assimp 3.  Unzip assimp.zip to build that version.
FBX SDK 2014.1: http://images.autodesk.com/adsk/files/fbx20141_fbxsdk_vs2008_win.exe
Copy C:\Program Files\Autodesk\FBX\FBX SDK\2014.1\lib\vs2008\x64\release\libfbxsdk.dll to obj2an8 folder. Or x86 folder if 32-bit.

The VS2013 V12 Sln uses FBX 2020.0.1, and Assimp 5.0.0.  Unzip assimp5_0_0.zip to build that version.  All exe generated and folders will have a v12 appended to name.
FBX SDK 2014.1: https://www.autodesk.com/content/dam/autodesk/www/adn/fbx/2020-0-1/fbx202001_fbxsdk_vs2013_win.exe
Copy C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1\lib\vs2013\x64\release\libfbxsdk.dll to obj2an8 folder. Or x86 folder if 32-bit.

Both use the same source files, just different version for assimp and FBX SDK.  Only the VS2013 Binaries are released.



.geobj format

Builds upon the existing obj format, fully supporting comments (\#), vertices (v), vertice normals (vn), vertice uvs (vt), faces (f), lines (l), groups (o or g), materials (mtllib and usemtl), and material file.
The additions to the format are all enclosed in \# comments, so that the obj file itself will be openable in existing modeling tools (though will not include the new features).


Additions:

Vertex Coloring
Add vertex coloring to obj format.  Vertex coloring should override material color, if present for a point, by any ingest tool.

Vertex Color Index
Like v, vn, and vt, an indexed list of vertex colors, starting at index 1.  Values must appear before use (in an \#fvcolorindex command).
\#vcolor R G B A 

R G B A [0-255.0]

Example:
\#vcolor 255.000000 255.000000 255.000000 255.000000

Vertex Color Face
After each face command, maps the face to the vertex color index.  Should always have 1 entry matching each face point.

[After an f line]
\#fvcolorindex vc1 vc2 vc3...

Example:
f 1/1/1 2/2/2 3/3/3 
\#fvcolorindex 1 2 3



Rigging
Rigging should appear at the beginning of the geobj file.  It is a set of joints connected in a hierarchy.  There are two modes, relative, and absolute.  Absolute mode is the default, where all joints are specified as absolute coordinates.  Relative mode must be explicitly specified, and includes SRT for each joint, relative to parent joint.  Relative mode is not intended for animations, and just for model data.  Relative mode's vertice positions are defined relative to joint, and NOT absolute, like the obj format declares, so for relative mode geobj files, the groups will not appear in the right position in a model viewer if imported.  

Joint mode
Before any joint data, include the type of joint data.  Absolute jointmode is the default if unspecified.
\#jointmode relative
\#jointmode absolute

Joints
Joints are specified as two commands in a row
\#joint [Name]

For absolute joint mode
\#jointposition TX TY TZ
[Doubles for absolute position (not relative to parent)]

Example:
\#joint 0
\#jointposition 0.000000 0.000000 0.000000
\#joint 1
\#jointposition 1.229596 62.036057 0.000000

For relative joint mode
\#jointsrt SX SY SZ RX RY RZ TX TY TZ
[Doubles for relative values (relative to parent)]

Example:
\#joint 0
\#jointsrt 1.000000 1.000000 1.000000 0.000000 0.000000 0.000000 0.000000 249.000031 0.000000
\#joint 1
\#jointsrt 1.000000 1.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

Joint Connections
Hierarchy of joint connections.  Defined one connection at a time.  All joints do not need to be connected, but it is recommended.  

\#connection [ParentName] [ChildName]

Example:
\#connection 1 2
\#connection 1 3
\#connection 3 5

Vertex Joint Assignment
Assigns vertices to a joint defined in the rigging.  

Two commands in a row, to define the joint, then assign one or more vertices assigned to the joint.  Must appear after the vertex index.  Multiple commands are additive to the same joint (may have more than one \#joint/\#vjoint command to the same joint).  Vertices may only be applied to one joint (vertex weighting not supported).
\#joint [Name]
\#vjoint [vertex index1] [vertex index2]...

Example:
\#joint 1
\#vjoint 1 2 3 4 6 7




Animations
Animations are defined, much like FBX format, using Euler Angles, Translation, and Scaling relative to the parent part.  Though the format supports multiple animations, one animation per file is recommended.
There is a special implicit joint defined in geobj format, called TopJoint.  It is not defined in geobj structure, but will appear in the other formats such as FBX.  It is always the highest top level joint connected to the structure.  So the first joint's parent will be TopJoint's keyframe srt, and TopJoint is root.

Animation Name
\#animation [Name]

Example:
\#animation Animation00F1

Keyframe Index
Current index of keyframe.  Not all existing keyframe indexes are required to be present, but is recommended.  Currently only linear interpolation is supported.
\#keyframe [number]
[Number is an integer value starting at 0]

Example:
\#keyframe 0


Keyframe Animation

Keyframe Translation
Translation applied to the entire keyframe (Top Joint).  Must appear after the matching \#keyframe command.  If not present, defaults to 0,0,0.  
\#keyframetranslation TX TY TZ
[Double for keyframe absolute translation]

Example:
\#keyframetranslation 15009.000977 7612.000488 84673.007813

Keyframe Rotation
Rotation applied to the entire keyframe (Top Joint).  Must appear after the matching \#keyframe command.  If not present, defaults to 0,0,0.
\#keyframerotation RX RY RZ
[Double for Euler angles applied at the top]

Example:
\#keyframerotation 351.914032 79.541008 0.351563

Example:
\#keyframerotation 351.914032 79.541008 0.351563

Keyframe Scaling
Scaling applied to the entire keyframe (Top Joint).  Must appear after the matching \#keyframe command.  If not present, defaults to 1,1,1.
\#keyframescale SX SY SZ

Example:
\#keyframescale 1 1 1

Whether Keyframe scale, rotation, translation is interpolated.  Indication from the conversion that the input format did not have an explicit keyframe here, and was interpolated.  Useful for import for certain animation formats.  Should appear immediately after the \#keyframerotation, \#keyframescale, \#keyframetranslation commands and match in x, y, z components which are interpolated.
\#keyframeisinterpolatedtranslation B0 B1 B2
\#keyframeisinterpolatedrotation B0 B1 B2
\#keyframeisinterpolatedscale B0 B1 B2
[1 or 0 for values, 1 = interpolated, 0 = keyframed explicitly.  Default is 1 1 1, if unspecified]

Example:
\#keyframeisinterpolatedtranslation 1 1 1
\#keyframeisinterpolatedrotation 1 0 1
\#keyframeisinterpolatedscale 1 1 0


Part Animation
Animation per joint.  

Part Index
Part to apply animation to.  Names must match joints in the Joint Hierarchy.  Currently only linear interpolation is supported.
\#part [name]

Example:
\#part 1

Part Translation
Relative to parent Translation applied to the part.  Must appear after the matching \#keyframe and \#part command.  If not present, defaults to 0,0,0.
\#parttranslation TX TY TZ

Example:
\#parttranslation 140.000000 13.000000 -191.000000

Part Rotation
Relative to parent Rotation applied to the part.  Must appear after the matching \#keyframe and \#part command.  If not present, defaults to 0,0,0.
\#partrotation RX RY RZ

Example:
\#partrotation 12.128906 359.384766 3.076172

Part Scaling
\#partscale SX SY SZ
Relative to parent Scaling applied to the part.  Must appear after the matching \#keyframe and \#part command.  If not present, defaults to 1,1,1.

Example:
\#partscale 0.420000 0.420000 0.420000

Whether Part scale, rotation, translation is interpolated.  Indication from the conversion that the input format did not have an explicit keyframe here, and was interpolated.  Useful for import for certain animation formats.  Should appear immediately after the \#partrotation, \#partscale, \#parttranslation commands and match in x, y, z components which are interpolated.
\#partisinterpolatedtranslation B0 B1 B2
\#partisinterpolatedrotation B0 B1 B2
\#partisinterpolatedscale B0 B1 B2
[1 or 0 for values, 1 = interpolated, 0 = keyframed explicitly.  Default is 1 1 1, if unspecified]

Example:
\#partisinterpolatedtranslation 1 1 1
\#partisinterpolatedrotation 1 0 1
\#partisinterpolatedscale 1 1 0


Cameras
Cameras do not include any geometry or joints, and are purely keyframe based.

Camera Name
Name of the camera.  Although multiple cameras are technically supported by the format, only one is recommended.
\#camera [Name]

Example:
\#camera DefaultCamera

Camera Field of View
Camera FOV angle.  
\#keyframefieldofview FOV
[Double in degrees]

Example:
\#keyframefieldofview 40.823002

Camera Color
Color applied to camera.
\#keyframecolor R G B A
[Double 0-1.0 for each component]

Example:
\#keyframecolor 1.000000 1.000000 1.000000 0.098039

Camera Translation
Camera absolute position.
\#keyframetranslation TX TY TZ

Example:
\#keyframetranslation 21300.101562 789.100037 1835.000000

Sample Animation:
mtllib animationfr.mtl
\#joint 0
\#jointposition 0.000000 9984.220703 -4534.167969
\#joint 1
\#jointposition 0.000000 8987.823242 -4531.666504
\#joint 2
\#jointposition 0.000000 7985.967285 -4530.801270
\#joint 3
\#jointposition 0.000000 6992.300293 -4531.081543
\#joint 4
\#jointposition 0.000000 5991.261719 -4532.010254
\#joint 5
\#jointposition 0.000000 4993.219727 -4533.083008
\#joint 6
\#jointposition 0.000000 3995.171387 -4533.902832
\#joint 7
\#jointposition 0.000000 3000.506104 -4534.464844
\#joint 8
\#jointposition 0.000000 1999.074463 -4534.869141
\#joint 9
\#jointposition 0.000000 1004.414917 -4535.204590
\#joint 10
\#jointposition 0.000000 6.372192 -4535.570313
\#connection 0 1
\#connection 1 2
\#connection 2 3
\#connection 3 4
\#connection 4 5
\#connection 5 6
\#connection 6 7
\#connection 7 8
\#connection 8 9
\#connection 9 10
v 0.000000 4002.219482 -3537.082764
v 0.000000 4996.261230 -3539.010010
v 0.000000 4998.261230 -5533.009766
v 0.000000 4002.219482 -3537.082764
v 0.000000 4998.261230 -5533.009766
v 0.000000 3997.219482 -5531.082520
v 0.000000 4996.261230 -3539.010010
v 0.000000 5995.299805 -3538.081299
v 0.000000 5995.299805 -5532.081055
v 0.000000 4996.261230 -3539.010010
v 0.000000 5995.299805 -5532.081055
v 0.000000 4998.261230 -5533.009766
v 0.000000 8.414916 -3538.204346
v 0.000000 1006.074402 -3537.868896
v 0.000000 1006.074402 -5531.868652
v 0.000000 8.414916 -3538.204346
v 0.000000 1006.074402 -5531.868652
v 0.000000 8.414916 -5533.204102
v 0.000000 1006.074402 -3537.868896
v 0.000000 2006.505981 -3537.464600
v 0.000000 2000.505981 -5532.464355
v 0.000000 1006.074402 -3537.868896
v 0.000000 2000.505981 -5532.464355
v 0.000000 1006.074402 -5531.868652
v 0.000000 2006.505981 -3537.464600
v 0.000000 2997.171143 -3539.902588
v 0.000000 3005.171143 -5534.902344
v 0.000000 2006.505981 -3537.464600
v 0.000000 3005.171143 -5534.902344
v 0.000000 2000.505981 -5532.464355
v 0.000000 2997.171143 -3539.902588
v 0.000000 4002.219482 -3537.082764
v 0.000000 3997.219482 -5531.082520
v 0.000000 2997.171143 -3539.902588
v 0.000000 3997.219482 -5531.082520
v 0.000000 3005.171143 -5534.902344
v 0.000000 5995.299805 -3538.081299
v 0.000000 6991.966797 -3537.801025
v 0.000000 6993.299805 -5532.081055
v 0.000000 5995.299805 -3538.081299
v 0.000000 6993.299805 -5532.081055
v 0.000000 5995.299805 -5532.081055
v 0.000000 6991.966797 -3537.801025
v 0.000000 7990.822754 -3537.666260
v 0.000000 7988.822754 -5531.666016
v 0.000000 6991.966797 -3537.801025
v 0.000000 7988.822754 -5531.666016
v 0.000000 6993.299805 -5532.081055
v 0.000000 7990.822754 -3537.666260
v 0.000000 8988.822266 -3537.666260
v 0.000000 8988.219727 -5533.167480
v 0.000000 7990.822754 -3537.666260
v 0.000000 8988.219727 -5533.167480
v 0.000000 7988.822754 -5531.666016
v 0.000000 8988.822266 -3537.666260
v 0.000000 9986.219727 -3538.167725
v 0.000000 9986.219727 -5533.167480
v 0.000000 8988.822266 -3537.666260
v 0.000000 9986.219727 -5533.167480
v 0.000000 8988.219727 -5533.167480
\#joint 0
\#vjoint 51 53 56 57 59 60
\#joint 1
\#vjoint 44 45 47 49 50 52 54 55 58
\#joint 2
\#vjoint 38 43 46
\#joint 3
\#vjoint 8 9 11 37 39 40 41 42 48
\#joint 4
\#vjoint 2 3 5 7 10 12
\#joint 5
\#vjoint 1 4 6 32 33 35
\#joint 6
\#vjoint 26 27 29 31 34 36
\#joint 7
\#vjoint 20 21 23 25 28 30
\#joint 8
\#vjoint 14 15 17 19 22 24
\#joint 9
\#vjoint 13 16 18
vt 0.000000 0.000000
vt -0.000579 0.497685
vt 1.002315 0.501157
vt 0.000000 0.000000
vt 1.002315 0.501157
vt 1.002894 0.000000
vt -0.000579 0.497685
vt -0.002894 0.997106
vt 1.000000 1.000000
vt -0.000579 0.497685
vt 1.000000 1.000000
vt 1.002315 0.501157
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 210.000000 210.000000 210.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 210.000000 210.000000 210.000000 255.000000
\#vcolor 166.000000 166.000000 166.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 34.000000 101.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
\#vcolor 0.000000 14.000000 45.000000 255.000000
\#vcolor 0.000000 26.000000 79.000000 255.000000
g Room01_primary
usemtl tempImgEd0045
f 1/1 2/2 3/3 
\#fvcolorindex 1 2 3 
f 4/4 5/5 6/6 
\#fvcolorindex 4 5 6 
f 7/7 8/8 9/9 
\#fvcolorindex 7 8 9 
f 10/10 11/11 12/12 
\#fvcolorindex 10 11 12 
usemtl tempImgEd0001
f 13/13 14/14 15/15 
\#fvcolorindex 13 14 15 
f 16/16 17/17 18/18 
\#fvcolorindex 16 17 18 
f 19/19 20/20 21/21 
\#fvcolorindex 19 20 21 
f 22/22 23/23 24/24 
\#fvcolorindex 22 23 24 
f 25/25 26/26 27/27 
\#fvcolorindex 25 26 27 
f 28/28 29/29 30/30 
\#fvcolorindex 28 29 30 
f 31/31 32/32 33/33 
\#fvcolorindex 31 32 33 
f 34/34 35/35 36/36 
\#fvcolorindex 34 35 36 
f 37/37 38/38 39/39 
\#fvcolorindex 37 38 39 
f 40/40 41/41 42/42 
\#fvcolorindex 40 41 42 
f 43/43 44/44 45/45 
\#fvcolorindex 43 44 45 
f 46/46 47/47 48/48 
\#fvcolorindex 46 47 48 
f 49/49 50/50 51/51 
\#fvcolorindex 49 50 51 
f 52/52 53/53 54/54 
\#fvcolorindex 52 53 54 
f 55/55 56/56 57/57 
\#fvcolorindex 55 56 57 
f 58/58 59/59 60/60 
\#fvcolorindex 58 59 60 
\#animation Animation0102
\#keyframe 0
\#keyframetranslation 0.000000 0.000000 0.000000
\#keyframerotation 0.000000 0.000000 0.000000
\#keyframescale 1.000000 1.000000 1.000000
\#part 0
\#parttranslation -3993.000244 10973.000977 -6991.000000
\#partrotation 0.175781 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 1
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.175781 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 2
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.175781 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 3
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.912109 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 4
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.736328 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 5
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.384766 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 6
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.000000 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 7
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.384766 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 8
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.087891 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 9
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.087891 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 10
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.000000 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#keyframe 1
\#keyframetranslation 0.000000 0.000000 0.000000
\#keyframerotation 0.000000 0.000000 0.000000
\#keyframescale 1.000000 1.000000 1.000000
\#part 0
\#parttranslation -3993.000244 10973.000977 -6991.000000
\#partrotation 0.175781 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 1
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.175781 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 2
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.175781 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 3
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.912109 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 4
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.736328 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 5
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.384766 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 6
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.000000 0.000000 359.912109
\#partscale 1.000000 1.000000 1.000000
\#part 7
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 359.384766 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 8
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.087891 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 9
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.087891 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000
\#part 10
\#parttranslation 0.000000 0.000000 0.000000
\#partrotation 0.000000 0.000000 0.000000
\#partscale 1.000000 1.000000 1.000000


Sample Camera:
\#camera DefaultCamera
\#keyframe 0
\#keyframefieldofview 40.823002
\#keyframecolor 1.000000 1.000000 1.000000 0.098039
\#keyframetranslation 21300.101562 789.100037 1835.000000
\#keyframerotation 270.000000 64.072258 270.000000
\#keyframe 1
\#keyframefieldofview 40.823002
\#keyframecolor 1.000000 1.000000 1.000000 0.098039
\#keyframetranslation 21284.601562 789.100037 1835.000000
\#keyframerotation 270.000000 64.248039 270.000000

Sample Relative Obj:
\#jointmode relative
\#joint 0
\#jointsrt 1.000000 1.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
\#joint 1
\#jointsrt 1.000000 1.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
\#connection 0 1
g Room01
v 318.000000 318.000000 344.000000
\#joint 1
\#vjoint 1
v -318.000000 318.000000 344.000000
\#joint 1
\#vjoint 2
v -318.000000 -318.000000 344.000000
\#joint 1
\#vjoint 3
vn -0.577350 -0.577350 -0.577350
vt 1.000000 1.000000 0.0
vt 0.000000 1.000000 0.0
vt 0.000000 0.000000 0.0
usemtl m0ClampSClampT
f 1/1/1 2/2/1 3/3/1
v 318.000000 -318.000000 344.000000
\#joint 1
\#vjoint 4
vt 1.000000 0.000000 0.0
f 4/4/1 1/1/1 3/3/1
