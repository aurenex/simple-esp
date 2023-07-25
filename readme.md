# Simple-ESP

A simple open-source project demonstrating the possibility of implementing ESP for Java Minecraft (from 1.0 to 1.12.2) using interceptions of only OpenGL functions.

![simple-esp-preview2](https://github.com/aurenex/simple-esp/assets/125130325/7746b6b0-e3dd-43d4-a2d0-15c487af83b1)

Special thanks to [zayats80888][zayats80888-link], who gave me a lot of useful knowledge about OpenGL, was always patient with my possibly stupid questions and made invaluable contributions to the project. And also [Jacquelin Potier][jackquelin-site-link], for developing the wonderful [WinAPIOverride][jackquelin-program-link] program, which i used for monitoring OpenGL function calls. And of course [Tsuda Kageyu][tsuda-link], whose [project][tsuda-project-link] is used to intercept function calls.

### Explanation
The Java version of the Minecraft game uses OpenGL to draw graphics. We cannot determine exactly which object is currently being drawn, nor can we get its in-game coordinates, since the game does not report this information to the library. However, we can indirectly determine that this is exactly the object that we need based on some signs.

For example:
```cpp
// This part of the code is called every time before drawing the player
protected void preRenderCallback(AbstractClientPlayer entitylivingbaseIn, float partialTickTime)
{
    float f = 0.9375F;
    GlStateManager.scale(f, f, f);
}
```

As a result, the function call will look like this:
```java
glScalef(0.9375F, 0.9375F, 0.9375F);
```

By intercepting the `glScalef` function and comparing all three parameters in it with those written above, we can assume that the game is currently going to render the player.

```cpp
void WINAPI hk_glScalef(float x, float y, float z)
{
    if (x == 0.9375f and y == 0.9375f and z == 0.9375f)
    {
        // Your code here
    }

    // Calling the original function
    fn_glScalef(x, y, z);
}
```

Now we can draw something right inside the condition or save the data of the `GL_MODELVIEW_MATRIX` and `GL_PROJECTION_MATRIX` matrixes for later use.

Function names, as well as unique parameters, can be obtained by viewing the decompiled game code or by monitoring OpenGL function calls using [specialized tools][specialized-tools-link].

### Compilation
- Clone this repository.
- Open the **simple-esp** solution file in [Visual Studio IDE][vs-download-link].
- Select the target platform.
- Press `ctrl + shift + b` to compile.

All compiled open-source releases available in our telegram [channel][tg-channel-link].

### Usage
- Open any DLL-injector **as administrator**.
- Find the **java minecraft** process.
- Inject the `simple-esp.dll` into process.

Before use, we strongly recommend that you read the [license][license-link].

[vs-download-link]: <https://visualstudio.microsoft.com/downloads/>
[tg-channel-link]: <https://t.me/aurenex>
[tsuda-link]: <https://github.com/tsudakageyu>
[tsuda-project-link]: <https://github.com/tsudakageyu/minhook>
[jackquelin-site-link]: <http://jacquelin.potier.free.fr>
[jackquelin-program-link]: <http://jacquelin.potier.free.fr/winapioverride32>
[zayats80888-link]: <https://www.cyberforum.ru/members/1417352.html>
[license-link]: <../master/license.md>
[specialized-tools-link]: <https://www.khronos.org/opengl/wiki/Debugging_Tools>
