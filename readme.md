# Simple ESP
A simple open-source project demonstrating the possibility of implementing ESP for Java Minecraft (from 1.0 to 1.12.2) using interceptions of only OpenGL functions.

![image](https://github.com/aurenex/simple-esp/assets/125130325/fd885e89-5259-47cc-b619-c8a2d8a5ae80)

### Explanation
Java Minecraft uses the OpenGL library to render graphics. The game does not tell the library what specific game object is currently being drawn, but we can determine this by some signs. For example, the `glTranslatef(0.5f, 0.4375f, 0.9375f)` function is always called before the chest is drawn. By intercepting the function and [comparing] its parameters, we can indirectly determine that at the moment the game is going to draw the chest. You cannot get the game coordinates of the object, but you can [save] the current (at the time of the function call) matrixes settings and [use] them later to draw something of your own in the same place.

### Compilation
- Clone this repository in any way you like.
- Open the **simple-esp** solution file in Visual Studio IDE.
  - Select your target platform.
  - Press `Ctrl + Shift + B` to compile.

All compiled open-source releases available in our telegram [channel].

### Usage
- Open any DLL-injector **as administrator**.
- Find the **java minecraft** process.
- Inject the `simple-esp.dll` into process.

### Attention
The provided source code is offered "as is," without any warranties or guarantees of its fitness for a particular purpose or functionality. The user assumes all risks associated with its use, modification, or distribution. The author shall not be liable for any damages, losses, or issues arising from the use of the provided code.

[channel]: <https://t.me/aurenex>
[comparing]: <../master/simple-esp/src/dllmain.cpp#L169>
[save]: <../master/simple-esp/src/dllmain.cpp#L30>
[use]: <../master/simple-esp/src/dllmain.cpp#L144>
[LICENSE]: <../master/license.md>