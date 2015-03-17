#pragma once

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class Camera
	{
	public:
		Camera(float scale = 1.0f);

		void Reset();

		void Translate(const Vector& offset);
		void Scale(float scale, int x, int y, int width, int height);

		Vector transPosScreenToProject(int x, int y, int width, int height) const;
		Vector transPosProjectToScreen(const Vector& proj, int width, int height) const;

		void UpdateModelView() const;

		float GetScale() const { return m_scale; }
		void SetScale(float s) { m_scale = s; }

		const Vector& GetPosition() const { return m_center; }

	private:
		Vector m_center;
		float m_scale;

	}; // Camera
}

