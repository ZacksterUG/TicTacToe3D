#pragma once
class IDrawable {
public:
	virtual void Render() = 0;
	virtual void RenderNoBuf() = 0;
};

