# dnd-win-bug-minimal-example
Minimal example of drag and drop bug on Windows when using Qt with gstreamer

To trigger bug: 
- select a media file to play
- move QListWidget items around to show that you can move them as expected
- play and stop the file 4 times
- drag and drop will stop working in the QListWidget

To build, modify the CMakeLists.txt file and modify the gstreamer paths to match your gstreamer install path.  If you have gstreamer installed in the default location (C:\gstreamer\) and you're using 1.18.3, you won't need to modify the paths.

To have cmake automatically run windeployqt and copy the necessary gstreamer dlls into your build output directory so you can run it from there after build, add -DDEPLOY_DEPS to your cmake command line

Here's the exception it's throwing:
```
Exception thrown at 0x74FD46D2 (KernelBase.dll) in DnDWinBugTestCase.exe: 0x80010012: The callee (server [not server application]) is not available and disappeared; all connections are invalid. The call did not execute.
```
The call stack shows that it's triggering on the Qt side after a call to:
const HRESULT r = DoDragDrop(dropDataObject, windowDropSource, allowedEffects, &resultEffect);
from ole32.dll in qwindowsdrag.cpp on line 700 in the QWindowsDrag.cpp file.
