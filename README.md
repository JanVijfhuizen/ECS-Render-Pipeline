ECS Render Pipeline

This is just a personal project for me to get better at rendering with OpenGL.
I've tried to keep things as dependency free as possible, so most classes aren't 
reliant on my ECS framework, and function just fine by themselves.

I've provided a basic renderer system which shows some of the capabilities of the
individual components.

Among other things, this project contains:
- Shader, texture and model (.obj) loaders.
- Mesh/shader wrapper classes.
- Post processing stack that supports multiple cameras.
- Window module.

