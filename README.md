ECS Render Pipeline

This is just a personal project for me to get better at rendering with OpenGL.
I've tried to keep things as dependency free as possible, so most classes aren't 
reliant on my ECS framework, and function just fine by themselves.

I've provided a basic renderer system which shows some of the capabilities of the
individual components.

Among other things, this project contains:
- Shader/Texture loaders.
- Mesh/Shader wrapper classes.
- a Post Processing stack.
- a Window module.

Instancing/batching will be implemented later on.
Later on I'll also provide a default shader with lightmaps, shadows and proper shading.

