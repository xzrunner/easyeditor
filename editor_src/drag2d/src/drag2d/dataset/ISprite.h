#pragma once

#include "interfaces.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Vector.h"

namespace d2d
{
	class Vector;
	class Rect;
	class ISymbol;
	class IBody;
	class AbstractBV;

	class ISprite : public ICloneable, public UserDataImpl, public Object
	{
	public:
		ISprite();
		ISprite(const ISprite& sprite);
		virtual ~ISprite();

		//
		// ICloneable interface
		//
		virtual ISprite* clone();

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr);

		virtual const ISymbol& getSymbol() const = 0;
		virtual void setSymbol(ISymbol* symbol) = 0;
		virtual void loadBodyFromFile() = 0;

		virtual void setTransform(const Vector& position, float angle);
		void setScale(float xScale, float yScale);
		void setScale(float scale);
		void setShear(float xShear, float yShear);

		bool isContain(const Vector& pos) const;
		bool isIntersect(const Rect& rect) const;

		void translate(const Vector& offset);
		void rotate(float delta);

		const Vector& getPosition() const;
		float getAngle() const;
		float getScaleX() const { return m_xScale; }
		float getScaleY() const { return m_yScale; }
		float getShearX() const { return m_xShear; }
		float getShearY() const { return m_yShear; }

		void setMirror(bool xMirror, bool yMirror) { m_xMirror = xMirror; m_yMirror = yMirror; }
		void getMirror(bool& xMirror, bool& yMirror) const { xMirror = m_xMirror; yMirror = m_yMirror; }

		AbstractBV* getBounding() const;

		void updateEachFrame();
		IBody* getBody() const;

		virtual void buildBounding() = 0;

	private:
		void afterScaleChanged();

	public:
		std::string name;
		
		std::string multiColor, addColor;

	protected:
		Vector m_pos;
		float m_angle;
		float m_xScale, m_yScale;
		float m_xShear, m_yShear;

		bool m_xMirror, m_yMirror;

		AbstractBV* m_bounding;

		IBody* m_body;

	}; // ISprite

	class SpriteCmp
	{
	public:
		enum Type
		{
			e_file,
			e_x,
			e_y
		};

	public:
		SpriteCmp(Type type = e_file);

		bool operator() (const ISprite* s0, const ISprite* s1) const;

	private:
		Type m_type;

	}; // SpriteCmp
}

