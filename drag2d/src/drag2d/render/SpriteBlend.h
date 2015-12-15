//#ifndef _DRAG2D_SPRITE_BLEND_H_
//#define _DRAG2D_SPRITE_BLEND_H_
//
//namespace d2d
//{
//
//class SpriteBlend
//{
//public:
//	void Draw(const ISprite* sprite) const;
//	
//	static SpriteRenderer* Instance();
//		
//private:
//	SpriteBlend();
//	~SpriteBlend();
//
//	void DrawImplBlend(const ISprite* sprite) const;
//
//	void DrawUnderToTmp(const ISprite* sprite) const;
//	void DrawSprToTmp(const ISprite* sprite) const;
//	void DrawTmpToScreen(const ISprite* sprite) const;
//
//	void InitBlendShader() const;
//
//private:
//	mutable FBO* m_fbo;
//	mutable int m_blend_idx;
//
//	const Camera* m_cam;
//
//	Matrix m_mt;
//
//private:
//	static SpriteBlend* m_instance;
//
//}; // SpriteBlend
//
//}
//
//#endif // _DRAG2D_SPRITE_BLEND_H_