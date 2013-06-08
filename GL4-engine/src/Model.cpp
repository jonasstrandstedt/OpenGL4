/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Model.h"

gl4::Model::Model(std::string filename, std::string texturename, glm::vec3 position, float scale, glm::vec3 rotation)
{

	_mode = GL_TRIANGLES;
	_texture = texturename;

	
	std::ifstream ifile(filename.c_str());
	if(ifile)
	{
		LOG("Found file..\n");
		ifile.close();
		loadObj(filename.c_str());
	} else {
		ERRLOG("Did not find file..\n");
	}

	_transform = glm::mat4(1.0);
	_transform = glm::rotate(_transform, rotation[0], glm::vec3(1,0,0));
	_transform = glm::rotate(_transform, rotation[1], glm::vec3(0,1,0));
	_transform = glm::rotate(_transform, rotation[2], glm::vec3(0,0,1));
	_transform = glm::translate(_transform, position);
	_transform = glm::scale(_transform, glm::vec3(scale,scale,scale));
}


void gl4::Model::loadObj(const char *filename)
{
	// temporary 
	int vertexSize = 0;
	int vertexNormalSize = 0;
	int vertexTextureSize = 0;
	int indicesSize = 0;
	
	float f1,f2,f3;
	int i1,i2,i3,i4,i5,i6,i7,i8,i9;
	char line[150];
	float maxtex = 0.0;
	
	FILE *fi;
	// START LINE COUNT
	fi = fopen(filename, "r");
	if (fi==NULL) {
		ERRLOG("Null Object\n");
	}
	while (fgets(line,150,fi)!=NULL)
	{
		if (sscanf( line, "v %f%f%f", &f1, &f2, &f3)) {
			vertexSize+=3;
		}
		if (sscanf( line, "vn %f%f%f", &f1, &f2, &f3)) {
			vertexNormalSize+=3;
		}
		if (sscanf( line, "vt %f%f%f", &f1, &f2, &f3)) {
			vertexTextureSize+=3;
		}
		if (vertexTextureSize > 0) {
			if (sscanf( line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9)) {
				indicesSize+=3;
			}
		} else {
			if (sscanf( line, "f %i//%i %i//%i %i//%i", &i1, &i2, &i3, &i4, &i5, &i6)) {
				indicesSize+=3;
			}
		}
	}
	/*	END LINE COUNT */
	
	// allocate memory for all arrays
	_isize = indicesSize;
	_vsize = indicesSize;
	
	// float arrays
	float *tempVertexArray = (float*)malloc(vertexSize*sizeof(float));
	float *tempVertexNormalArray = (float*)malloc(vertexNormalSize*sizeof(float));
	float *tempVertexTextureArray = (float*)malloc(vertexTextureSize*sizeof(float));
	_varray = (Vertex*)malloc(_vsize*sizeof(Vertex));
	
	// int arrays
	_iarray = (int*)malloc(_isize*sizeof(int));
	int *tempNormalIndicesArray = (int*)malloc(_isize*sizeof(int));
	int *tempTextureIndicesArray = (int*)malloc(_isize*sizeof(int));
	
	// keeping track of the array indexes
	unsigned int i = 0;
	unsigned int n = 0;
	unsigned int m = 0;
	unsigned int w = 0;
	
	// Go back to beginning of file
	fseek(fi, 0, SEEK_SET);
	while (fgets(line,150,fi)!=NULL)
	{
		if (sscanf( line, "v %f%f%f", &f1, &f2, &f3)) 
		{
			(tempVertexArray)[i] = f1;
			i++;
			(tempVertexArray)[i] = f2;
			i++;
			(tempVertexArray)[i] = f3;
			i++;
		}
		if (sscanf( line, "vn %f%f%f", &f1, &f2, &f3))
		{
			(tempVertexNormalArray)[n] = f1;
			n++;
			(tempVertexNormalArray)[n] = f2;
			n++;
			(tempVertexNormalArray)[n] = f3;
			n++;
		}
		
		if (sscanf( line, "vt %f%f%f", &f1, &f2, &f3)) 
		{
			(tempVertexTextureArray)[w] = f1;
			maxtex = ((tempVertexTextureArray)[w] > maxtex) ? (tempVertexTextureArray)[w] : maxtex;
			w++;
			(tempVertexTextureArray)[w] = f2;
			maxtex = ((tempVertexTextureArray)[w] > maxtex) ? (tempVertexTextureArray)[w] : maxtex;
			w++;
			(tempVertexTextureArray)[w] = f3;
			maxtex = ((tempVertexTextureArray)[w] > maxtex) ? (tempVertexTextureArray)[w] : maxtex;
			w++;
		}
		
		if (vertexTextureSize > 0)
		{
			if (sscanf( line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9)) 
			{
				(_iarray)[m] = i1-1;
				(tempTextureIndicesArray)[m] = i2-1;
				(tempNormalIndicesArray)[m] = i3-1;
				m++;
				(_iarray)[m] = i4-1;
				(tempTextureIndicesArray)[m] = i5-1;
				(tempNormalIndicesArray)[m] = i6-1;
				m++;
				(_iarray)[m] = i7-1;
				(tempTextureIndicesArray)[m] = i8-1;
				(tempNormalIndicesArray)[m] = i9-1;
				m++;
			}
		} else 
		{
			if (sscanf( line, "f %i//%i %i//%i %i//%i", &i1, &i2, &i3, &i4, &i5, &i6)) 
			{
				(_iarray)[m] = i1-1;
				(tempNormalIndicesArray)[m] = i2-1;
				m++;
				(_iarray)[m] = i3-1;
				(tempNormalIndicesArray)[m] = i4-1;
				m++;
				(_iarray)[m] = i5-1;
				(tempNormalIndicesArray)[m] = i6-1;
				m++;
			}	
		}
	}
	fclose(fi);
	// end of file read
	
	// creating the vertex array
	i=0;n=0;m=0;
	int normalIndex = 0;
	int textureIndex = 0;
	int vertexIndex = 0;
	while (m<_vsize) 
	{

		normalIndex = tempNormalIndicesArray[m]*3;
		textureIndex = tempTextureIndicesArray[m]*3;
		vertexIndex = _iarray[m]*3;
		_iarray[m] = m;
		
		int q = 0;
		while (q < 3) 
		{
			_varray[m].location[q] = tempVertexArray[vertexIndex+q];
			_varray[m].normal[q] = tempVertexNormalArray[normalIndex+q];
			q++;
		}	
		_varray[m].color[0] = 1;
		_varray[m].color[1] = 1;
		_varray[m].color[2] = 1;
		_varray[m].color[3] = 1.0;

		_varray[m].attribute[0] = 0;
		_varray[m].attribute[1] = 0;
		_varray[m].attribute[2] = 0;
		_varray[m].float_attribute = 0;

		if (vertexTextureSize > 0)
		{
			_varray[m].tex[0] = tempVertexTextureArray[textureIndex];
			_varray[m].tex[1] = tempVertexTextureArray[textureIndex+1];
		} else
		{
			_varray[m].tex[0] = 1.0;
			_varray[m].tex[1] = 1.0;
		}
		m++;
	}
	
	// free up memory
	free(tempVertexArray);
	free(tempVertexNormalArray);
	free(tempNormalIndicesArray);
	free(tempVertexTextureArray);
	free(tempTextureIndicesArray);
}
