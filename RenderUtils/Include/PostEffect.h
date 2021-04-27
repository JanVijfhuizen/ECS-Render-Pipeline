#pragma once

namespace rut
{
	// Effect used to process and modify the final image of a camera.
	class PostEffect
	{
	public:
		// The next image will be drawn with this effect.
		virtual void Use() = 0;
	};
}
