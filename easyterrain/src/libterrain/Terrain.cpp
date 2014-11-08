#include "Terrain.h"

namespace libterrain
{

Terrain::Terrain()
	: m_hight_scale(0)
{
}

Terrain::~Terrain()
{
	delete m_data.pixels;
}

bool Terrain::LoadHeightMap(const char* filename, int size)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		wxLogDebug("Fail to open %s", filename);
		return false;
	}

	if (m_data.pixels) {
		delete[] m_data.pixels;
		m_data.pixels = NULL;
	}
	int sz = size * size;
	m_data.pixels = new unsigned char[sz];
	if (m_data.pixels == NULL) {
		wxLogDebug("Could not allocate memory for%s", filename);
		return false;
	}

	memset(m_data.pixels, 0, sz);
	fin.read(reinterpret_cast<char*>(m_data.pixels), sz);

	fin.close();

	m_data.size = size;

	return true;
}

bool Terrain::SaveHeightMap(const char* filename)
{
	if (m_data.size <= 0 || m_data.pixels == NULL) {
		return false;
	}

	std::locale::global(std::locale(""));
	std::ofstream fout(filename, std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		wxLogDebug("Fail to open %s", filename);
		return false;
	}

	fout.write(reinterpret_cast<const char*>(m_data.pixels), 
		sizeof(m_data.size*m_data.size));

	fout.close();

	return true;
}

//--------------------------------------------------------------
// Name:			CTERRAIN::MakeTerrainFault - public
// Description:		Create a height data set using the "Fault Formation"
//					algorithm.  Thanks a lot to Jason Shankel for this code!
// Arguments:		-iSize: Desired size of the height map
//					-iIterations: Number of detail passes to make
//					-iMinDelta, iMaxDelta: the desired min/max heights
//					-iIterationsPerFilter: Number of passes per filter
//					-fFilter: Strength of the filter
// Return Value:	A boolean value: -true: successful creation
//									 -false: unsuccessful creation
//--------------------------------------------------------------
bool Terrain::MakeTerrainFault( int iSize, int iIterations, int iMinDelta, int iMaxDelta, float fFilter )
{
	float* fTempBuffer;
	int iCurrentIteration;
	int iHeight;
	int iRandX1, iRandZ1;
	int iRandX2, iRandZ2;
	int iDirX1, iDirZ1;
	int iDirX2, iDirZ2;
	int x, z;
	int i;

	if (m_data.pixels) {
		delete[] m_data.pixels;
		m_data.pixels = NULL;
	}

	m_data.size= iSize;

	//allocate the memory for our height data
	m_data.pixels= new unsigned char [m_data.size*m_data.size];
	fTempBuffer= new float [m_data.size*m_data.size];

	//check to see if memory was successfully allocated
	if( m_data.pixels==NULL )
	{
		//something is seriously wrong here
		wxLogDebug("Could not allocate memory for height map");
		return false;
	}

	//check to see if memory was successfully allocated
	if( fTempBuffer==NULL )
	{
		//something is seriously wrong here
		wxLogDebug("Could not allocate memory for height map");
		return false;
	}

	//clear the height fTempBuffer
	for( i=0; i<m_data.size*m_data.size; i++ )
		fTempBuffer[i]= 0;

	for( iCurrentIteration=0; iCurrentIteration<iIterations; iCurrentIteration++ )
	{
		//calculate the height range (linear interpolation from iMaxDelta to
		//iMinDelta) for this fault-pass
		iHeight= iMaxDelta - ( ( iMaxDelta-iMinDelta )*iCurrentIteration )/iIterations;
		
		//pick two points at random from the entire height map
		iRandX1= rand( )%m_data.size;
		iRandZ1= rand( )%m_data.size;
		
		//check to make sure that the points are not the same
		do
		{
			iRandX2= rand( )%m_data.size;
			iRandZ2= rand( )%m_data.size;
		} while ( iRandX2==iRandX1 && iRandZ2==iRandZ1 );

		
		//iDirX1, iDirZ1 is a vector going the same direction as the line
		iDirX1= iRandX2-iRandX1;
		iDirZ1= iRandZ2-iRandZ1;
		
		for( z=0; z<m_data.size; z++ )
		{
			for( x=0; x<m_data.size; x++ )
			{
				//iDirX2, iDirZ2 is a vector from iRandX1, iRandZ1 to the current point (in the loop)
				iDirX2= x-iRandX1;
				iDirZ2= z-iRandZ1;
				
				//if the result of ( iDirX2*iDirZ1 - iDirX1*iDirZ2 ) is "up" (above 0), 
				//then raise this point by iHeight
				if( ( iDirX2*iDirZ1 - iDirX1*iDirZ2 )>0 )
					fTempBuffer[( z*m_data.size )+x]+= ( float )iHeight;
			}
		}

		//erode terrain
		FilterHeightField( fTempBuffer, fFilter );
	}

	//normalize the terrain for our purposes
	NormalizeTerrain( fTempBuffer );

	//transfer the terrain into our class's unsigned char height buffer
	for( z=0; z<m_data.size; z++ )
	{
		for( x=0; x<m_data.size; x++ )
			SetHeightAtPoint( ( unsigned char )fTempBuffer[( z*m_data.size )+x], x, z );
	}

	//delete temporary buffer
	if( fTempBuffer )
	{
		//delete the data
		delete[] fTempBuffer;
	}

	return true;
}

//--------------------------------------------------------------
// Name:			CTERRAIN::MakeTerrainPlasma - public
// Description:		Create a height data set using the "Midpoint
//					Displacement" algorithm.  Thanks a lot to 
//					Jason Shankel for this code!
//					Note: this algorithm has limited use, since
//					CLOD algorithms usually require a height map 
//					size of (n^2)+1 x (n^2)+1, and this algorithm
//					can only generate (n^2) x (n^2) maps
// Arguments:		-iSize: Desired size of the height map
//					-fRoughness: Desired roughness of the created map
// Return Value:	A boolean value: -true: successful creation
//									 -false: unsuccessful creation
//--------------------------------------------------------------
bool Terrain::MakeTerrainPlasma( int iSize, float fRoughness )
{
	float* fTempBuffer;
	float fHeight, fHeightReducer;
	int iRectSize= iSize;
	int ni, nj;
	int mi, mj;
	int pmi, pmj;
	int i, j;
	int x, z;

	if (m_data.pixels) {
		delete[] m_data.pixels;
		m_data.pixels = NULL;
	}

	if( fRoughness<0 )
		fRoughness*= -1;

	fHeight		  = ( float )iRectSize/2;
	fHeightReducer= ( float )pow(2, -1*fRoughness);

	m_data.size= iSize;

	//allocate the memory for our height data
	m_data.pixels= new unsigned char [m_data.size*m_data.size];
	fTempBuffer= new float [m_data.size*m_data.size];

	//check to see if memory was successfully allocated
	if( m_data.pixels==NULL )
	{
		//something is seriously wrong here
		wxLogDebug("Could not allocate memory for height map");
		return false;
	}

	//check to see if memory was successfully allocated
	if( fTempBuffer==NULL )
	{
		//something is seriously wrong here
		wxLogDebug("Could not allocate memory for height map");
		return false;
	}

	//set the first value in the height field
	fTempBuffer[0]= 0.0f;

	//being the displacement process
	while( iRectSize>0 )
	{
		/*Diamond step -

		Find the values at the center of the retangles by averaging the values at 
		the corners and adding a random offset:


		a.....b
		.     .  
		.  e  .
		.     .
		c.....d   

		e  = (a+b+c+d)/4 + random

		In the code below:
		a = (i,j)
		b = (ni,j)
		c = (i,nj)
		d = (ni,nj)
		e = (mi,mj)   */
		for( i=0; i<m_data.size; i+=iRectSize )
		{
			for( j=0; j<m_data.size; j+=iRectSize )
			{
				ni= ( i+iRectSize )%m_data.size;
				nj= ( j+iRectSize )%m_data.size;

				mi= ( i+iRectSize/2 );
				mj= ( j+iRectSize/2 );

				fTempBuffer[mi+mj*m_data.size]= ( float )( ( fTempBuffer[i+j*m_data.size] + fTempBuffer[ni+j*m_data.size] + fTempBuffer[i+nj*m_data.size] + fTempBuffer[ni+nj*m_data.size] )/4 + d2d::Random::getNum( -fHeight/2, fHeight/2 ) );
			}
		}

		/*Square step -

		Find the values on the left and top sides of each rectangle
		The right and bottom sides are the left and top sides of the neighboring rectangles,
		  so we don't need to calculate them

		The height m_data.pixels wraps, so we're never left hanging.  The right side of the last
			rectangle in a row is the left side of the first rectangle in the row.  The bottom
			side of the last rectangle in a column is the top side of the first rectangle in
			the column

              .......
		      .     .
		      .     .
		      .  d  .
		      .     .
		      .     .
		......a..g..b
		.     .     .
		.     .     .
		.  e  h  f  .
		.     .     .
		.     .     .
		......c......

		g = (d+f+a+b)/4 + random
		h = (a+c+e+f)/4 + random
		
		In the code below:
			a = (i,j) 
			b = (ni,j) 
			c = (i,nj) 
			d = (mi,pmj) 
			e = (pmi,mj) 
			f = (mi,mj) 
			g = (mi,j)
			h = (i,mj)*/
		for( i=0; i<m_data.size; i+=iRectSize )
		{
			for( j=0; j<m_data.size; j+=iRectSize )
			{

				ni= (i+iRectSize)%m_data.size;
				nj= (j+iRectSize)%m_data.size;

				mi= (i+iRectSize/2);
				mj= (j+iRectSize/2);

				pmi= (i-iRectSize/2+m_data.size)%m_data.size;
				pmj= (j-iRectSize/2+m_data.size)%m_data.size;
				
				//Calculate the square value for the top side of the rectangle
				fTempBuffer[mi+j*m_data.size]= ( float )( ( fTempBuffer[i+j*m_data.size]	  +
														fTempBuffer[ni+j*m_data.size]	  +
														fTempBuffer[mi+pmj*m_data.size]	  +
														fTempBuffer[mi+mj*m_data.size] )/4+
														d2d::Random::getNum( -fHeight/2, fHeight/2 ) );

				//Calculate the square value for the left side of the rectangle
				fTempBuffer[i+mj*m_data.size]= ( float )( ( fTempBuffer[i+j*m_data.size]	  +
														fTempBuffer[i+nj*m_data.size]	  +
														fTempBuffer[pmi+mj*m_data.size]	  +
														fTempBuffer[mi+mj*m_data.size] )/4+ 
														d2d::Random::getNum( -fHeight/2, fHeight/2 ) );
			}
		}

		//reduce the rectangle size by two to prepare for the next
		//displacement stage
		iRectSize/= 2;

		//reduce the height by the height reducer
		fHeight*= fHeightReducer;
	}

	//normalize the terrain for our purposes
	NormalizeTerrain( fTempBuffer );

	//transfer the terrain into our class's unsigned char height buffer
	for( z=0; z<m_data.size; z++ )
	{
		for( x=0; x<m_data.size; x++ )
			SetHeightAtPoint( ( unsigned char )fTempBuffer[( z*m_data.size )+x], x, z );
	}

	//delete temporary buffer
	if( fTempBuffer )
	{
		//delete the data
		delete[] fTempBuffer;
	}

	return true;
}

bool Terrain::LoadTexture(char* filename)
{
	return m_texture.Load(filename, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
}

//--------------------------------------------------------------
// Name:			CTERRAIN::NormalizeTerrain - private
// Description:		Scale the terrain height values to a range of
//					0-255
// Arguments:		-fpHeightData: the height data buffer
// Return Value:	None
//--------------------------------------------------------------
void Terrain::NormalizeTerrain( float* fpHeightData )
{
	float fMin, fMax;
	float fHeight;
	int i;

	fMin= fpHeightData[0];
	fMax= fpHeightData[0];

	//find the min/max values of the height fTempBuffer
	for( i=1; i<m_data.size*m_data.size; i++ )
	{
		if( fpHeightData[i]>fMax ) 
			fMax= fpHeightData[i];

		else if( fpHeightData[i]<fMin ) 
			fMin= fpHeightData[i];
	}

	//find the range of the altitude
	if( fMax<=fMin )
		return;

	fHeight= fMax-fMin;

	//scale the values to a range of 0-255 (because I like things that way)
	for( i=0; i<m_data.size*m_data.size; i++ )
		fpHeightData[i]= ( ( fpHeightData[i]-fMin )/fHeight )*255.0f;
}

//--------------------------------------------------------------
// Name:			CTERRAIN::FilterHeightBand - private
// Description:		Apply the erosion filter to an individual 
//					band of height values
// Arguments:		-fpBand: the band to be filtered
//					-iStride: how far to advance per pass
//					-iCount: Number of passes to make
//					-fFilter: the filter strength
// Return Value:	None
//--------------------------------------------------------------
void Terrain::FilterHeightBand( float* fpBand, int iStride, int iCount, float fFilter )
{
	float v= fpBand[0];
	int j  = iStride;
	int i;

	//go through the height band and apply the erosion filter
	for( i=0; i<iCount-1; i++ )
	{
		fpBand[j]= fFilter*v + ( 1-fFilter )*fpBand[j];

		v = fpBand[j];
		j+= iStride;
	}
}

//--------------------------------------------------------------
// Name:			CTERRAIN::FilterHeightfTempBuffer - private
// Description:		Apply the erosion filter to an entire buffer
//					of height values
// Arguments:		-fpHeightData: the height values to be filtered
//					-fFilter: the filter strength
// Return Value:	None
//--------------------------------------------------------------
void Terrain::FilterHeightField( float* fpHeightData, float fFilter )
{
	int i;

	//erode left to right
	for( i=0; i<m_data.size; i++ )
		FilterHeightBand( &fpHeightData[m_data.size*i], 1, m_data.size, fFilter );

	//erode right to left
	for( i=0; i<m_data.size; i++ )
		FilterHeightBand( &fpHeightData[m_data.size*i+m_data.size-1], -1, m_data.size, fFilter );

	//erode top to bottom
	for( i=0; i<m_data.size; i++ )
		FilterHeightBand( &fpHeightData[i], m_data.size, m_data.size, fFilter);

	//erode from bottom to top
	for( i=0; i<m_data.size; i++ )
		FilterHeightBand( &fpHeightData[m_data.size*(m_data.size-1)+i], -m_data.size, m_data.size, fFilter );
}

}