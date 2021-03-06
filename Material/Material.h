#include "../include/shader.h"
#include "../Particle/Particle.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

#define Pi 3.1416
#define eNum 180
#define rNum 800
#define fNum 9
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))


float rPointer[(rNum+1) * eNum * 4 * 3 * fNum];
float radius[rNum];

float normal(int j, string t)
{
	if (t == "CONSTANT")
		return 1.0f;
	else if (t == "QUADRATIC")
		return ((float)j / rNum) * ((float)j / rNum);
	else if (t == "SIN")
		return sin(Pi * j / rNum);
	else
		return 1.0f;
}

class Material
{
private:

	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	unsigned int materialVAO, materialVBO;
    unsigned int woodID,woodSurfaceID;
	float offsetX;
	float length;
	float initR;
	string texture;

	void renderPiece(int num){
		float y1, z1, y2, z2, y3, z3, y4, z4, tl,tr,tt,tb,leftR, rightR, normalX, normalY, normalZ,ifsurface;
		float pieceWidth = length / (2*rNum), offset;
		if (num == 0)
		{
			rightR = radius[0];
			leftR = (radius[0] + radius[1]) / 2;
		}
		else if (num == rNum - 1)
		{
			rightR = (radius[rNum - 2] + radius[rNum - 1]) / 2;
			leftR = radius[rNum - 1];
		}
		else
		{
			rightR = (radius[num] + radius[num - 1]) / 2;
			leftR = (radius[num] + radius[num + 1]) / 2;
		}
		if(rightR==1.5f&&leftR==1.5f)
			ifsurface=1.5f;
		else
			ifsurface=0.0f;
		// rNum <- 0
		offset = (rNum - 1 - 2 * num) * pieceWidth;

		tl = 0.25+float(rNum-num)/rNum*0.75;
		tr = 0.25+float(rNum-num+1)/rNum*0.75;

		for (int i = 0; i < eNum; i++)
		{
			y1 = leftR * cos(2 * Pi / eNum * i);
			z1 = leftR * sin(2 * Pi / eNum * i);

			y2 = leftR * cos(2 * Pi / eNum * (i + 1));
			z2 = leftR * sin(2 * Pi / eNum * (i + 1));

			y3 = rightR * cos(2 * Pi / eNum * i);
			z3 = rightR * sin(2 * Pi / eNum * i);

			y4 = rightR * cos(2 * Pi / eNum * (i + 1));
			z4 = rightR * sin(2 * Pi / eNum * (i + 1));

			tt = float(i)/eNum;
			tb = float(i+1)/eNum;

			normalX = ((y1 - y3) * (z2 - z1) + (y2 - y1) * (z3 - z1)) / (2 * pieceWidth);
			normalY = z2 - z1;
			normalZ = y1 - y2;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 0] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1] = y1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2] = z1;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7] = tl;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8] = tt;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 1] = y2;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 2] = z2;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 7] = tl;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 8] = tb;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 1] = y3;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 2] = z3;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 7] = tr;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 8] = tt;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 1] = y3;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 2] = z3;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 7] = tr;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 8] = tt;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 1] = y2;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 2] = z2;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 7] = tl;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 8] = tb;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 1] = y4;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 2] = z4;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 3] = normalX;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 4] = normalY;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 5] = normalZ;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 6] = ifsurface;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 7] = tr;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 8] = tb;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 1] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 2] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 3] = -1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 7] = 0.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 8] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 1] = y1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 2] = z1;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 3] = -1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 7] = 1.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 8] = 1.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum] = offset - pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 1] = y2;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 2] = z2;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 3] = -1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 7] = 1.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 8] = 1.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 1] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 2] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 3] = 1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 7] = 0.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 8] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 1] = y3;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 2] = z3;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 3] = 1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 7] = 1.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 8] = 1.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum] = offset + pieceWidth;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 1] = y4;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 2] = z4;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 3] = 1;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 4] = 0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 5] = 0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 6] = 0.0;

			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 7] = 1.0;
			rPointer[num * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 8] = 1.0;
		}
	}

	void renderBase(){
		float y1, z1, y2, z2, tl,tr,tt,tb,leftR, rightR, normalX, normalY, normalZ,ifsurface=1.5f;
		float pieceWidth = 1.0f, offset;

		offset = -length/2-pieceWidth;

		tl = 0;
		tr = 0.25;

		for (int i = 0; i < eNum; i++)
		{
			y1 = initR * cos(2 * Pi / eNum * i);
			z1 = initR * sin(2 * Pi / eNum * i);

			y2 = initR * cos(2 * Pi / eNum * (i + 1));
			z2 = initR * sin(2 * Pi / eNum * (i + 1));

			tt = float(i)/eNum;
			tb = float(i+1)/eNum;

			normalX = 0;
			normalY = z2 - z1;
			normalZ = y1 - y2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 0] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1] = y1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2] = z1;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7] = tl;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8] = tt;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 1] = y2;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 2] = z2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 7] = tl;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 1 * fNum + 8] = tb;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 1] = y1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 2] = z1;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 7] = tr;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 2 * fNum + 8] = tt;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 1] = y1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 2] = z1;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 7] = tr;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 3 * fNum + 8] = tt;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 1] = y2;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 2] = z2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 7] = tl;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 4 * fNum + 8] = tb;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 1] = y2;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 2] = z2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 3] = normalX;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 4] = normalY;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 5] = normalZ;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 6] = ifsurface;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 7] = tr;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 5 * fNum + 8] = tb;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 1] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 2] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 3] = -1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 7] = 0.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 6 * fNum + 8] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 1] = y1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 2] = z1;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 3] = -1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 7] = 1.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 7 * fNum + 8] = 1.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum] = offset - pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 1] = y2;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 2] = z2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 3] = -1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 7] = 1.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 8 * fNum + 8] = 1.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 1] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 2] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 3] = 1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 7] = 0.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 9 * fNum + 8] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 1] = y1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 2] = z1;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 3] = 1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 7] = 1.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 10 * fNum + 8] = 1.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum] = offset + pieceWidth;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 1] = y2;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 2] = z2;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 3] = 1;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 4] = 0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 5] = 0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 6] = 0.0;

			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 7] = 1.0;
			rPointer[rNum * eNum * 4 * 3 * fNum + i * 4 * 3 * fNum + 11 * fNum + 8] = 1.0;
		}
	}
	void renderInit()
	{
		for (int j = 0; j < rNum; j++)
		{
			radius[j] = initR * normal(j, "CONSTANT");
		}
		for (int j = 0; j < rNum; j++)
		{
			renderPiece(j);
		}
	}

public:
	Shader ironShader,woodShader;

	Material(glm::vec3 lightPos, glm::vec3 viewPos,float offsetX,float initR,float length)
	:ironShader(Shader("../Material/Iron.vs", "../Material/Iron.fs")),woodShader(Shader("../Material/Wood.vs","../Material/Wood.fs"))
	{
		this->offsetX=offsetX;
		this->initR=initR;
		this->length=length;
		this->texture="iron";

		this->initialize();

		glGenVertexArrays(1, &materialVAO);
		glGenBuffers(1, &materialVBO);

		glGenTextures(1, &this->woodID);
		glBindTexture(GL_TEXTURE_2D, this->woodID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int widthTex, heightTex, nrChannels;
        unsigned char *data = stbi_load("../Material/Wood.jpg", &widthTex, &heightTex, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

		glGenTextures(1, &this->woodSurfaceID);
		glBindTexture(GL_TEXTURE_2D, this->woodSurfaceID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("../Material/Surface.jpg", &widthTex, &heightTex, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

		woodShader.use();
        woodShader.setInt("woodTexture",0);
        woodShader.setInt("surfaceTexture",1);

		woodShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		woodShader.setVec3("lightPos", lightPos);
		woodShader.setVec3("viewPos", viewPos);

        woodShader.setVec3("wood.ambient", 0.450000, 0.450000, 0.450000);
        woodShader.setVec3("wood.diffuse", 0.45, 0.45, 0.45);
        woodShader.setVec3("wood.specular", 0.444597, 0.444597, 0.444597);
        woodShader.setFloat("wood.shininess",  14.800003);

		woodShader.setVec3("surface.ambient", 0.505882, 0.505882, 0.505882);
        woodShader.setVec3("surface.diffuse", 0.227451, 0.227451, 0.227451);
        woodShader.setVec3("surface.specular", 0.333333, 0.333333, 0.521569);
        woodShader.setFloat("surface.shininess",  9.846150);

		ironShader.use();
		ironShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		ironShader.setVec3("lightPos", lightPos);
		ironShader.setVec3("viewPos", viewPos);

        ironShader.setVec3("steel.ambient", 0.250000, 0.250000, 0.250000);
        ironShader.setVec3("steel.diffuse", 0.65, 0.65, 0.65);
        ironShader.setVec3("steel.specular", 0.774597, 0.774597, 0.774597);
        ironShader.setFloat("steel.shininess",  76.800003);

		ironShader.setVec3("surface.ambient", 0.305882, 0.305882, 0.305882);
        ironShader.setVec3("surface.diffuse", 0.427451, 0.427451, 0.427451);
        ironShader.setVec3("surface.specular", 0.333333, 0.333333, 0.521569);
        ironShader.setFloat("surface.shininess",  9.846150);
	}

	void initialize(){
		this->renderInit();
		this->renderBase();

		glBindVertexArray(materialVAO);
		glBindBuffer(GL_ARRAY_BUFFER, materialVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rPointer), &rPointer, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(7 * sizeof(float)));
		glEnableVertexAttribArray(3);
	}

	void drawMaterial(glm::mat4 view, glm::mat4 projection, glm::mat4 model,glm::mat4 rotate,bool ifdisplay)
	{
		model=glm::translate(model,glm::vec3(offsetX,0.0f,0.0f));
		if(texture=="wood"){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->woodID);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->woodSurfaceID);

			woodShader.use();
			woodShader.setMat4("view", view);
			woodShader.setMat4("projection", projection);
			woodShader.setMat4("model", model);
			woodShader.setMat4("rotate",rotate);
		} else {
			ironShader.use();
			ironShader.setMat4("view", view);
			ironShader.setMat4("projection", projection);
			ironShader.setMat4("model", model);
			ironShader.setMat4("rotate",rotate);
		}

		glBindVertexArray(materialVAO);
		if(ifdisplay)
			glDrawArrays(GL_TRIANGLES, 0, rNum * eNum * 4 * 3);
		else
			glDrawArrays(GL_TRIANGLES, 0, (rNum+1) * eNum * 4 * 3);
		glBindVertexArray(0);
	}

	void changeTexure(ParticleSystem *ps){
		texture=(texture=="wood")?"iron":"wood";
		ps->changeColor(texture);
	}

	void updateRadius(glm::vec3 knifePos,glm::vec3 lastPos,ParticleSystem *ps,float dt){
		float pieceWidth = length / (2*rNum),leftLimit=offsetX-length/2,rightLimit=offsetX+length/2;
		glm::vec3 leftPos,rightPos;
		bool toLeft;

		if(knifePos.x < lastPos.x){
			leftPos=knifePos;
			rightPos=lastPos;
			toLeft = true;
		}
		else {
			leftPos=lastPos;
			rightPos=knifePos;
			toLeft = false;
		}

		if(leftPos.x > rightLimit || rightPos.x < leftLimit){
			return;
		}

		if(leftPos.x==rightPos.x){
			return;
		}
		else {
			float k=(rightPos.y-leftPos.y)/(rightPos.x-leftPos.x),
				b=(rightPos.x*leftPos.y-leftPos.x*rightPos.y)/(rightPos.x-leftPos.x);
			
			if(leftPos.x < leftLimit) leftPos=glm::vec3(leftLimit,k*leftLimit+b,0);
			if(rightPos.x > rightLimit) rightPos=glm::vec3(rightLimit,k*rightLimit+b,0);

			int leftNum=((offsetX-leftPos.x)/length+0.5)*rNum,rightNum=((offsetX-rightPos.x)/length+0.5)*rNum;
			float knifeX,knifeY;
			for(int i=rightNum;i<=leftNum;i++){
				knifeX=(rNum -1-2*i)*pieceWidth+offsetX;
				knifeY=k*knifeX+b;
				if(knifeY<=0&&(-knifeY)<radius[i]){
					int pNum=MAX((-knifeY-radius[i]) * 8 /1.5f,1);
					for(int j=0;j<pNum;j++){
						ps->insertParticle(Particle(glm::vec3(knifeX,knifeY,0),glm::vec3((toLeft?1:-1)*rand()%10,-rand()%10,-5+rand()%10)));
					}

					radius[i]=-knifeY;
				}
					
			}
			for(int i=MAX(0,rightNum-1);i<=MIN(rNum-1,leftNum+1);i++){
				renderPiece(i);
			}
		}

		glBindVertexArray(materialVAO);
		glBindBuffer(GL_ARRAY_BUFFER, materialVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rPointer), &rPointer, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, fNum * sizeof(float), (void *)(7 * sizeof(float)));
		glEnableVertexAttribArray(3);
		
		return;
	}
};