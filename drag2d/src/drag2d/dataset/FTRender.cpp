#include "FTRender.h"
#include "glyph_util.h"
#include "common/Exception.h"

#include <wx/filename.h>

#include <freetype/ftstroke.h>

namespace d2d
{

#if (('1234' >> 24) == '1')
#elif (('4321' >> 24) == '1')
  #define BIG_ENDIAN
#else
  #error "Couldn't determine the endianness!"
#endif

struct Vec2
{
  Vec2() { }
  Vec2(float a, float b)
  : x(a), y(b) { }

  float x, y;
};

struct Rect
{
  Rect() { }
  Rect(float left, float top, float right, float bottom)
  : xmin(left), xmax(right), ymin(top), ymax(bottom) { }

  void Include(const Vec2 &r)
  {
	xmin = std::min(xmin, r.x);
    ymin = std::min(ymin, r.y);
    xmax = std::max(xmax, r.x);
    ymax = std::max(ymax, r.y);
  }

  float Width() const { return xmax - xmin + 1; }
  float Height() const { return ymax - ymin + 1; }

  float xmin, xmax, ymin, ymax;
};

struct Span
{
	Span() { }
	Span(int _x, int _y, int _width, int _coverage)
		: x(_x), y(_y), width(_width), coverage(_coverage) { }

	int x, y, width, coverage;
};

typedef std::vector<Span> Spans;

void
RasterCallback(const int y,
			   const int count,
			   const FT_Span * const spans,
			   void * const user) 
{
	Spans *sptr = (Spans *)user;
	for (int i = 0; i < count; ++i) 
		sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
}


// Set up the raster parameters and render the outline.

void
RenderSpans(FT_Library &m_library,
			FT_Outline * const outline,
			Spans *spans) 
{
	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = RasterCallback;
	params.user = spans;

	FT_Outline_Render(m_library, outline, &params);
}

FTRender::FTRender()
{
	FT_Init_FreeType(&m_library);
}

FTRender::~FTRender()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
}

void FTRender::LoadFont(const char* filename)
{
	if (!wxFileName::FileExists(filename)) {
		throw Exception("File: %s don't exist!", filename);
	}

	if (FT_New_Face(m_library, filename, 0, &m_face)) {
		throw Exception("FT_New_Face failed (there is probably a problem with your font file)");
	}
}

GlyphSizer FTRender::GetGlyphSizer(int unicode, int size)
{
	GlyphSizer sizer;

	if (!m_face) {
		return sizer;
	}

	FT_Set_Pixel_Sizes(m_face, size, size);

	FT_Size_Metrics s = m_face->size->metrics;
	sizer.height = (s.height >> 6) + 1;    

	FT_UInt gindex = FT_Get_Char_Index(m_face, unicode);
	if (FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP) == 0) {
		sizer.height = (m_face->glyph->metrics.horiAdvance >> 6) - 1;  
	} else {
		sizer.width = 0;
	}
	return sizer;
}

uint32_t* FTRender::WriteGlyphWithStroker(int unicode, int size, union Pixel32 fontCol,
	union Pixel32 outlineCol, float outlineWidth, GlyphLayout& layout)
{
	if (!m_face) {
		return NULL;
	}

	// Set the size to use.
	//  if (FT_Set_Char_Size(m_face, size >> 6, size >> 6, 90, 90) == 0)
	FT_Set_Pixel_Sizes(m_face, size, size);
	FT_Size_Metrics s = m_face->size->metrics;
	layout.metrics_height = s.height >> 6;    
	{
		// Load the glyph we are looking for.
		FT_UInt gindex = FT_Get_Char_Index(m_face, unicode);
		if(gindex ==0 )
			return NULL;

		if (FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP) == 0)
		{
			// Need an outline for this to work.
			if (m_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
			{
				// Render the basic glyph to a span list.
				Spans spans;
				RenderSpans(m_library, &m_face->glyph->outline, &spans);

				// Next we need the spans for the outline.
				Spans outlineSpans;

				// Set up a stroker.
				FT_Stroker stroker;
				FT_Stroker_New(m_library, &stroker);
				FT_Stroker_Set(stroker,
					(int)(outlineWidth * 64),
					FT_STROKER_LINECAP_ROUND,
					FT_STROKER_LINEJOIN_ROUND,
					0);

				FT_Glyph glyph;
				if (FT_Get_Glyph(m_face->glyph, &glyph) == 0)
				{


					FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
					// Again, this needs to be an outline to work.
					if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
					{
						// Render the outline spans to the span list
						FT_Outline *o =
							&reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
						RenderSpans(m_library, o, &outlineSpans);
					}

					// Clean up afterwards.
					FT_Stroker_Done(stroker);
					FT_Done_Glyph(glyph);

					// Now we need to put it all together.
					if (!spans.empty())
					{
						// Figure out what the bounding rect is for both the span lists.
						Rect rect(spans.front().x,
							spans.front().y,
							spans.front().x,
							spans.front().y);
						for (Spans::iterator s = spans.begin();
							s != spans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}
						for (Spans::iterator s = outlineSpans.begin();
							s != outlineSpans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}

						layout.bearingX = m_face->glyph->metrics.horiBearingX >> 6;
						layout.bearingY = m_face->glyph->metrics.horiBearingY >> 6;
						layout.advance = m_face->glyph->metrics.horiAdvance >> 6;

						// Get some metrics of our image.
						int imgWidth = rect.Width(),
							imgHeight = rect.Height(),
							imgSize = imgWidth * imgHeight;

						// Allocate data for our image and clear it out to transparent.
						//            Pixel32 *pxl = new Pixel32[imgSize];
						Pixel32 *pxl = new Pixel32[imgSize];
						memset(pxl, 0, sizeof(Pixel32) * imgSize);

						// Loop over the outline spans and just draw them into the
						// image.
						for (Spans::iterator s = outlineSpans.begin(); s != outlineSpans.end(); ++s)
							for (int w = 0; w < s->width; ++w) {
								// todo rotate
//								int index = (int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth + s->x - rect.xmin + w);								
								int index = (int)((s->y - rect.ymin) * imgWidth + s->x - rect.xmin + w);								
//								set_color(&pxl[index], outlineCol.b, outlineCol.g, outlineCol.r, s->coverage);
								pxl[index].Set(outlineCol.r, outlineCol.g, outlineCol.b, s->coverage);
							}

							// Then loop over the regular glyph spans and blend them into
							// the image.
							for (Spans::iterator s = spans.begin(); s != spans.end(); ++s)
								for (int w = 0; w < s->width; ++w)
								{
									// todo rotate
// 									Pixel32 &dst =
// 										pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth + s->x - rect.xmin + w)];
									Pixel32 &dst =
										pxl[(int)((s->y - rect.ymin) * imgWidth + s->x - rect.xmin + w)];

									Pixel32 src;
//									set_color(&src, fontCol.b, fontCol.g, fontCol.r, s->coverage);
									src.Set(fontCol.r, fontCol.g, fontCol.b, s->coverage);
									dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
									dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
									dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
									dst.a = std::min(255, dst.a + src.a);
								}

								layout.sizer.width = imgWidth;
								layout.sizer.height = imgHeight;
								return (uint32_t*)pxl;
					}
				}
			}
		}
	}
	return NULL;
}

uint32_t* FTRender::WriteGlyphNoStroker(int unicode, int size, union Pixel32 col, GlyphLayout& layout)
{
	if (!m_face) {
		return NULL;
	}

	FT_Set_Pixel_Sizes(m_face, size, size);
	FT_Size_Metrics s = m_face->size->metrics;
	layout.metrics_height = s.height >> 6;

	FT_UInt gindex = FT_Get_Char_Index(m_face, unicode);
	if(gindex ==0 )
		return NULL;

	if (FT_Load_Glyph(m_face, gindex, FT_LOAD_DEFAULT)) {
		return NULL;
	}

	FT_Glyph glyph;
	if (FT_Get_Glyph(m_face->glyph, &glyph)) {
		return NULL;
	}

	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmap_glyph->bitmap; //just to make things easier

	// Get metrics
	layout.bearingX = m_face->glyph->metrics.horiBearingX >> 6;
	layout.bearingY = m_face->glyph->metrics.horiBearingY >> 6;
	layout.sizer.height = bitmap.rows;
	layout.sizer.width = bitmap.width;
	layout.advance = m_face->glyph->metrics.horiAdvance >> 6;

	int sz = bitmap.rows*bitmap.width;
	uint32_t* dst = new uint32_t[sz];
	memset(dst, 0, sz);

	// todo rotate
// 	for (int i = 0; i<sz; i++)
// 	{
// 		unsigned char r = ((col.r * bitmap.buffer[i]) >> 8) + 1,
// 			g = ((col.g * bitmap.buffer[i]) >> 8) + 1,
// 			b = ((col.b * bitmap.buffer[i]) >> 8) + 1,
// 			a = bitmap.buffer[i];
// 		dst[i] = a << 24 | b << 16 | g << 8 | r;                    
// 	}
	int ptr = 0;
	for (int i = 0; i < bitmap.rows; ++i) {
		for (int j = 0; j < bitmap.width; ++j) {
			unsigned char r = ((col.r * bitmap.buffer[ptr]) >> 8) + 1,
				g = ((col.g * bitmap.buffer[ptr]) >> 8) + 1,
				b = ((col.b * bitmap.buffer[ptr]) >> 8) + 1,
				a = bitmap.buffer[ptr];
			int dst_ptr = (bitmap.rows - 1 - i) * bitmap.width + j;
			dst[dst_ptr] = a << 24 | b << 16 | g << 8 | r;
			++ptr;
		}
	}

	FT_Done_Glyph(glyph);

	return dst;
}

}