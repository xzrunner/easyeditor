#pragma once

#include "interfaces.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Vector.h"
#include "common/Color.h"

#include <JSON/json.h>

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
		virtual ISprite* clone() const;

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr);

		virtual const ISymbol& getSymbol() const = 0;
		virtual void setSymbol(ISymbol* symbol) = 0;
		virtual void loadBodyFromFile() = 0;

		virtual void load(const Json::Value& val);
		virtual void store(Json::Value& val);

		virtual void setTransform(const Vector& position, float angle);
		void setScale(float xScale, float yScale);
		void setScale(float scale);
		void setShear(float xShear, float yShear);

		bool isContain(const Vector& pos) const;
		bool isIntersect(const Rect& rect) const;

		void translate(const Vector& offset);
		void rotate(float delta);

		const Vector& getPosition() const {
			return m_pos;
		}
		const Vector& getOffset() const {
			return m_offset;
		}
		void setOffset(const Vector& offset);
		float getAngle() const {
			return m_angle;
		}
		float getScaleX() const { return m_xScale; }
		float getScaleY() const { return m_yScale; }
		float getShearX() const { return m_xShear; }
		float getShearY() const { return m_yShear; }

		void setMirror(bool xMirror, bool yMirror) { m_xMirror = xMirror; m_yMirror = yMirror; }
		void getMirror(bool& xMirror, bool& yMirror) const { xMirror = m_xMirror; yMirror = m_yMirror; }
		bool getMirrorX() const { return m_xMirror; }
		bool getMirrorY() const { return m_yMirror; }

		Vector getCenter() const;

		AbstractBV* getBounding() const {
			return m_bounding;
		}

		void updateEachFrame();
		IBody* getBody() const;

		void setObserver(ISpriteObserver* observer) {
			m_observer = observer;
		}

		virtual void buildBounding() = 0;

	private:
		void afterScaleChanged();

	public:
		// info
		std::string name;		
		Colorf multiCol, addCol;
		std::string tag;
		bool clip;

		// edit
		bool visiable;
		bool editable;

	protected:
		Vector m_pos;
		float m_angle;
		Vector m_offset;
		float m_xScale, m_yScale;
		float m_xShear, m_yShear;

		bool m_xMirror, m_yMirror;

		AbstractBV* m_bounding;

		IBody* m_body;

		ISpriteObserver* m_observer;

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

