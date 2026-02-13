#include "Triangle.h"

Triangle::Triangle(Vec3d v1, Vec3d v2, Vec3d v3, Vec2d t1, Vec2d t2, Vec2d t3, olc::Pixel col)
	: vertex{ v1, v2, v3 }, 
	 texture{ t1, t2, t3 }, 
	 color(col) {}




int Triangle::ClipAgainstPlane(Vec3d planePoint, Vec3d planeNormal, Triangle& inTriangle, Triangle& outTriangle1, Triangle& outTriangle2)
{
	planeNormal.Normalise();

	auto dist = [&](const Vec3d& p) 
	{
		Vec3d n = p;
		n.Normalise();
		return (planeNormal.x * p.x + planeNormal.y * p.y + planeNormal.z * p.z - Vec3d::DotProduct(planeNormal, planePoint));
	};

	Vec3d* insidePoints[3];		int nInsidePointCount = 0;
	Vec3d* outsidePoints[3];	int nOutsidePointCount = 0;

	Vec2d* insideTexture[3];	int nInsideTextureCount = 0;
	Vec2d* outsideTexture[3];	int nOutsideTextureCount = 0;


	float d0 = dist(inTriangle.vertex[0]);
	float d1 = dist(inTriangle.vertex[1]);
	float d2 = dist(inTriangle.vertex[2]);
	


	if (d0 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTriangle.vertex[0];
		insideTexture[nInsideTextureCount++] = &inTriangle.texture[0];
	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTriangle.vertex[0];
		outsideTexture[nOutsideTextureCount++] = &inTriangle.texture[0];
	}

	if (d1 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTriangle.vertex[1];
		insideTexture[nInsideTextureCount++] = &inTriangle.texture[1];

	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTriangle.vertex[1];
		outsideTexture[nOutsideTextureCount++] = &inTriangle.texture[1];
	}

	if (d2 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTriangle.vertex[2];
		insideTexture[nInsideTextureCount++] = &inTriangle.texture[2];
	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTriangle.vertex[2];
		outsideTexture[nOutsideTextureCount++] = &inTriangle.texture[2];
	}

	
	
	if (nInsidePointCount == 0) 
	{
		// All points lie on the outside of plane,
		// so clip whole triangle.It ceases to exist

		return 0;
	}

	if (nInsidePointCount == 3)	
	{
		// All points lie on the inside of plane, so do 
		// nothing and allow the triangle to simply pass through

		outTriangle1 = inTriangle;
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2) 
	{
		// Triangle should be clipped. As two points lie outside the plane, 
		// the triangle simply becomes a smaller triangle

		outTriangle1.color = inTriangle.color;

		outTriangle1.vertex[0] = *insidePoints[0];
		outTriangle1.texture[0] = *insideTexture[0];

		float t;
		outTriangle1.vertex[1] = Vec3d::IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[0], t);
		outTriangle1.texture[1].u = t * (outsideTexture[0]->u - insideTexture[0]->u) + insideTexture[0]->u;
		outTriangle1.texture[1].v = t * (outsideTexture[0]->v - insideTexture[0]->v) + insideTexture[0]->v;
		outTriangle1.texture[1].w = t * (outsideTexture[0]->w - insideTexture[0]->w) + insideTexture[0]->w;

		outTriangle1.vertex[2] = Vec3d::IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[1], t);
		outTriangle1.texture[2].u = t * (outsideTexture[1]->u - insideTexture[0]->u) + insideTexture[0]->u;
		outTriangle1.texture[2].v = t * (outsideTexture[1]->v - insideTexture[0]->v) + insideTexture[0]->v;
		outTriangle1.texture[2].w = t * (outsideTexture[1]->w - insideTexture[0]->w) + insideTexture[0]->w;

			
		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		outTriangle1.color = inTriangle.color; outTriangle2.color = inTriangle.color;

		outTriangle1.vertex[0] = *insidePoints[0];
		outTriangle1.vertex[1] = *insidePoints[1];
		outTriangle1.texture[0] = *insideTexture[0];
		outTriangle1.texture[1] = *insideTexture[1];

		float t;
		outTriangle1.vertex[2] = Vec3d::IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[0], t);
		outTriangle1.texture[1].u = t * (outsideTexture[0]->u - insideTexture[0]->u) + insideTexture[0]->u;
		outTriangle1.texture[1].v = t * (outsideTexture[0]->v - insideTexture[0]->v) + insideTexture[0]->v;
		outTriangle1.texture[1].w = t * (outsideTexture[0]->w - insideTexture[0]->w) + insideTexture[0]->w;

		outTriangle2.vertex[0] = *insidePoints[1];
		outTriangle2.texture[0] = *insideTexture[1];
		outTriangle2.vertex[1] = outTriangle1.vertex[2];
		outTriangle2.texture[1] = outTriangle1.texture[2];

		outTriangle2.vertex[2] = Vec3d::IntersectPlane(planePoint, planeNormal, *insidePoints[1], *outsidePoints[0], t);
		outTriangle2.texture[2].u = t * (outsideTexture[0]->u - insideTexture[1]->u) + insideTexture[1]->u;
		outTriangle2.texture[2].v = t * (outsideTexture[0]->v - insideTexture[1]->v) + insideTexture[1]->v;
		outTriangle2.texture[2].w = t * (outsideTexture[0]->w - insideTexture[1]->w) + insideTexture[1]->w;


		return 2;
	}

	return 0;
}