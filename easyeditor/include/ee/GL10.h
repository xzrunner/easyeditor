#ifndef _EASYEDITOR_GL10_H_
#define _EASYEDITOR_GL10_H_

namespace ee
{

class GL10
{
public:
	/* BeginMode */
	static const int  GL_POINTS = 0x0000;
	static const int  GL_LINES = 0x0001;
	static const int  GL_LINE_LOOP = 0x0002;
	static const int  GL_LINE_STRIP = 0x0003;
	static const int  GL_TRIANGLES = 0x0004;
	static const int  GL_TRIANGLE_STRIP = 0x0005;
	static const int  GL_TRIANGLE_FAN = 0x0006;
	static const int  GL_QUADS = 0x0007;
	static const int  GL_QUAD_STRIP = 0x0008;
	static const int  GL_POLYGON = 0x0009;

	/* BlendingFactorDest */
	static const int  GL_ZERO = 0;
	static const int  GL_ONE = 1;
	static const int  GL_SRC_COLOR = 0x0300;
	static const int  GL_ONE_MINUS_SRC_COLOR = 0x0301;
	static const int  GL_SRC_ALPHA = 0x0302;
	static const int  GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	static const int  GL_DST_ALPHA = 0x0304;
	static const int  GL_ONE_MINUS_DST_ALPHA = 0x0305;

	/* DataType */
	static const int GL_BYTE = 0x1400;
	static const int GL_UNSIGNED_BYTE = 0x1401;
	static const int GL_SHORT = 0x1402;
	static const int GL_UNSIGNED_SHORT = 0x1403;
	static const int GL_INT = 0x1404;
	static const int GL_UNSIGNED_INT = 0x1405;
	static const int GL_FLOAT = 0x1406;
	static const int GL_2_BYTES = 0x1407;
	static const int GL_3_BYTES = 0x1408;
	static const int GL_4_BYTES = 0x1409;
	static const int GL_DOUBLE = 0x140A;


	/* vertex_array */
	static const int GL_VERTEX_ARRAY = 0x8074;
	static const int GL_NORMAL_ARRAY = 0x8075;
	static const int GL_COLOR_ARRAY = 0x8076;
	static const int GL_INDEX_ARRAY = 0x8077;
	static const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	static const int GL_EDGE_FLAG_ARRAY = 0x8079;
	static const int GL_VERTEX_ARRAY_SIZE = 0x807A;
	static const int GL_VERTEX_ARRAY_TYPE = 0x807B;
	static const int GL_VERTEX_ARRAY_STRIDE = 0x807C;
	static const int GL_NORMAL_ARRAY_TYPE = 0x807E;
	static const int GL_NORMAL_ARRAY_STRIDE = 0x807F;
	static const int GL_COLOR_ARRAY_SIZE = 0x8081;
	static const int GL_COLOR_ARRAY_TYPE = 0x8082;
	static const int GL_COLOR_ARRAY_STRIDE = 0x8083;
	static const int GL_INDEX_ARRAY_TYPE = 0x8085;
	static const int GL_INDEX_ARRAY_STRIDE = 0x8086;
	static const int GL_TEXTURE_COORD_ARRAY_SIZE = 0x8088;
	static const int GL_TEXTURE_COORD_ARRAY_TYPE = 0x8089;
	static const int GL_TEXTURE_COORD_ARRAY_STRIDE = 0x808A;
	static const int GL_EDGE_FLAG_ARRAY_STRIDE = 0x808C;
	static const int GL_VERTEX_ARRAY_POINTER = 0x808E;
	static const int GL_NORMAL_ARRAY_POINTER = 0x808F;
	static const int GL_COLOR_ARRAY_POINTER = 0x8090;
	static const int GL_INDEX_ARRAY_POINTER = 0x8091;
	static const int GL_TEXTURE_COORD_ARRAY_POINTER = 0x8092;
	static const int GL_EDGE_FLAG_ARRAY_POINTER = 0x8093;
	static const int GL_V2F = 0x2A20;
	static const int GL_V3F = 0x2A21;
	static const int GL_C4UB_V2F = 0x2A22;
	static const int GL_C4UB_V3F = 0x2A23;
	static const int GL_C3F_V3F = 0x2A24;
	static const int GL_N3F_V3F = 0x2A25;
	static const int GL_C4F_N3F_V3F = 0x2A26;
	static const int GL_T2F_V3F = 0x2A27;
	static const int GL_T4F_V4F = 0x2A28;
	static const int GL_T2F_C4UB_V3F = 0x2A29;
	static const int GL_T2F_C3F_V3F = 0x2A2A;
	static const int GL_T2F_N3F_V3F = 0x2A2B;
	static const int GL_T2F_C4F_N3F_V3F = 0x2A2C;
	static const int GL_T4F_C4F_N3F_V4F = 0x2A2D;

	static const int GL_BLEND = 0x0BE2;
	static const int GL_TEXTURE_2D = 0x0DE1;
	static const int GL_TEXTURE_WIDTH = 0x1000;
	static const int GL_TEXTURE_HEIGHT = 0x1001;
	static const int GL_TEXTURE_INTERNAL_FORMAT = 0x1003;

	static const int GL_LINE_STIPPLE = 0x0B24;

	/* PixelFormat */
	static const int GL_RGB = 0x1907;
	static const int GL_RGBA = 0x1908;

	static void Begin(int mode);

	static void BindTexture(int target, unsigned int texture);

	static void BlendFunc(int sfactor, int dfactor);

	static void Color3f(float red, float green, float blue);

	static void Color4f(float red, float green, float blue, float alpha);

	static void ColorPointer(int size, int type, int stride, const void* pointer);

	static void Disable(int cap);

	static void DisableClientState(int array);

	static void DrawArrays(int mode, int first, int count);

	static void Enable(int cap);

	static void EnableClientState(int array);

	static void End();

	static void GetTexLevelParameteriv(int target, int level, int pname, int* params);

	static void LineStipple(int factor, unsigned short pattern);

	static void LineWidth(float width);

	static void PointSize(float size);

	static void PopMatrix();

	static void PushMatrix();

	static void Rotatef(float angle, float x, float y, float z);

	static void Scalef(float x, float y, float z);

	static void MultMatrixf(const float* m);

	static void TexCoord2f(float s, float t);

	static void TexCoordPointer(int size, int type, int stride, const void* pointer);

	static void Translatef(float x, float y, float z);

	static void Vertex2f(float x, float y);

	static void Vertex3f(float x, float y, float z);

	static void VertexPointer(int size, int type, int stride, const void* pointer);

	static void GetTexImage(int target, int level, int format, int type, void* img);

}; // GL10

}

#endif // _EASYEDITOR_GL10_H_