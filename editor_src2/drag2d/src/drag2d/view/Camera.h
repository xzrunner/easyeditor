#pragma once

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class ICameraObserver;

	class Camera
	{
	public:
		Camera(float scale = 1.0f);

		const Vector& getCenter() const;
		void setCenter(const Vector& center);

		float getScale() const;
		void setScale(float scale);
		void setScale(float scale, int x, int y, int width, int height);

		Vector transPosScreenToProject(int x, int y, int width, int height) const;
		Vector transPosProjectToScreen(const Vector& proj, int width, int height) const;

		// For reset projection transform (gluOrtho2D)
		void addObserver(ICameraObserver* observer);

	private:
		void notifyObservers();

	private:
		Vector m_center;
		float m_scale;

		std::vector<ICameraObserver*> m_observers;

	}; // Camera
}

