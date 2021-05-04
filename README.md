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

Writing a render system with this setup can be as easy as this:

```
// Grab the required component sets.
auto& cameras = jecs::MapSet<rpi::Camera>::Get();
auto& models = jecs::SparseSet<rpi::Model>::Get();
const auto& transforms = jecs::SparseSet<rpi::Transform>::Get();

for (const auto [camera, camIndex] : cameras)
{
	const auto eye = transforms[camIndex].position;
	const auto view = rpi::CameraSystem::GetView(camIndex);
	const auto projection = rpi::CameraSystem::GetProjection(camIndex);

	for (const auto [model, modIndex] : models)
		{
			model.shader->Use(modIndex, eye, view, projection);
			model.mesh->Draw(modIndex);
		}
}
```

Model loading, textures and lighting:
![](Vids/0.gif)

Here, two cameras are being used. One uses an inverse post effect.
![](Vids/1.gif)