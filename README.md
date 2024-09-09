# Falling Sand

I stumbled upon a YouTube [video](https://youtu.be/L4u7Zy_b868?t=1111) about "falling sand simulation" and thought "hey, why not recreate it!", so here we are. 

---

"Simulation" happens in a [Compute Shader](https://www.khronos.org/opengl/wiki/Compute_Shader) which outputs colors into a 2D texture which is sampled when rendering a full-screen quad. 

"Simulation" on a **10k** by **10k** grid (thus **100M** cells) runs with a frame time of `~6ms` on my computer with a **Ryzen 5 3600** and **2080 Super** on release mode with `-O3` flag. The *majority* of that time is highly likely spent swapping the front and back buffers of the GLFW window, i.e. calling `glfwSwapBuffers()`. So, the actual "simulation" is likely *significantly* faster than that number. With that said, performance was never the focus as I just wanted to slap up a quick version and call it a day. It's still fun to see some numbers, even if they don't really mean much in the grand scheme of things.

![example](https://github.com/user-attachments/assets/f99f414e-3e39-4a7d-9407-247a64c2d4be)

Built with C++20 using OpenGL 4.6, GLFW, GLM.
