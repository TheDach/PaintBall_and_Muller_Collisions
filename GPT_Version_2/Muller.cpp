#include "Muller.h"

//====================================== 100% ПЕРЕСЕЧЕНИЯ | MULLER ===============================================//


/* if USE_EPSILON_TEST is true then we do a check:
		 if |dv|<EPSILON then dv=0.0;
   else no check is done (which is less robust)
*/
#define USE_EPSILON_TEST TRUE  
#define EPSILON 0.000001


/* some macros */
#define CROSS(dest,v1,v2)                      \
              dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
              dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
              dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2)          \
            dest[0]=v1[0]-v2[0]; \
            dest[1]=v1[1]-v2[1]; \
            dest[2]=v1[2]-v2[2]; 

/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               float c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }

#define ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1) \
              isect0=VV0+(VV1-VV0)*D0/(D0-D1);    \
              isect1=VV0+(VV2-VV0)*D0/(D0-D2);


#define COMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,isect0,isect1) \
  if(D0D1>0.0f)                                         \
  {                                                     \
    /* here we know that D0D2<=0.0 */                   \
    /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else if(D0D2>0.0f)                                    \
  {                                                     \
    /* here we know that d0d1<=0.0 */                   \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D1*D2>0.0f || D0!=0.0f)                       \
  {                                                     \
    /* here we know that d0d1<=0.0 or that D0!=0.0 */   \
    ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1);          \
  }                                                     \
  else if(D1!=0.0f)                                     \
  {                                                     \
    ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
  }                                                     \
  else if(D2!=0.0f)                                     \
  {                                                     \
    ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
  }                                                     \
  else                                                  \
  {                                                     \
    /* triangles are coplanar */                        \
    return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2);      \
  }



/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }                                

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  float a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3])
{
	float A[3];
	short i0, i1;
	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A[0] = fabs(N[0]);
	A[1] = fabs(N[1]);
	A[2] = fabs(N[2]);
	if (A[0] > A[1])
	{
		if (A[0] > A[2])
		{
			i0 = 1;      /* A[0] is greatest */
			i1 = 2;
		}
		else
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if (A[2] > A[1])
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
		else
		{
			i0 = 0;      /* A[1] is greatest */
			i1 = 2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V1, V2, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V2, V0, U0, U1, U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0, U0, U1, U2);
	POINT_IN_TRI(U0, V0, V1, V2);

	return 0;
}


int tri_tri_intersect(float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3])
{
	float E1[3], E2[3];
	float N1[3], N2[3], d1, d2;
	float du0, du1, du2, dv0, dv1, dv2;
	float D[3];
	float isect1[2], isect2[2];
	float du0du1, du0du2, dv0dv1, dv0dv2;
	short index;
	float vp0, vp1, vp2;
	float up0, up1, up2;
	float b, c, max;

	/* compute plane equation of triangle(V0,V1,V2) */
	SUB(E1, V1, V0);
	SUB(E2, V2, V0);
	CROSS(N1, E1, E2);
	d1 = -DOT(N1, V0);
	/* plane equation 1: N1.X+d1=0 */

	/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
	du0 = DOT(N1, U0) + d1;
	du1 = DOT(N1, U1) + d1;
	du2 = DOT(N1, U2) + d1;

	/* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
	if (fabs(du0) < EPSILON) du0 = 0.0;
	if (fabs(du1) < EPSILON) du1 = 0.0;
	if (fabs(du2) < EPSILON) du2 = 0.0;
#endif
	du0du1 = du0 * du1;
	du0du2 = du0 * du2;

	if (du0du1 > 0.0f && du0du2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return 0;                    /* no intersection occurs */

	/* compute plane of triangle (U0,U1,U2) */
	SUB(E1, U1, U0);
	SUB(E2, U2, U0);
	CROSS(N2, E1, E2);
	d2 = -DOT(N2, U0);
	/* plane equation 2: N2.X+d2=0 */

	/* put V0,V1,V2 into plane equation 2 */
	dv0 = DOT(N2, V0) + d2;
	dv1 = DOT(N2, V1) + d2;
	dv2 = DOT(N2, V2) + d2;

#if USE_EPSILON_TEST==TRUE
	if (fabs(dv0) < EPSILON) dv0 = 0.0;
	if (fabs(dv1) < EPSILON) dv1 = 0.0;
	if (fabs(dv2) < EPSILON) dv2 = 0.0;
#endif

	dv0dv1 = dv0 * dv1;
	dv0dv2 = dv0 * dv2;

	if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return 0;                    /* no intersection occurs */

	/* compute direction of intersection line */
	CROSS(D, N1, N2);

	/* compute and index to the largest component of D */
	max = fabs(D[0]);
	index = 0;
	b = fabs(D[1]);
	c = fabs(D[2]);
	if (b > max) max = b, index = 1;
	if (c > max) max = c, index = 2;

	/* this is the simplified projection onto L*/
	vp0 = V0[index];
	vp1 = V1[index];
	vp2 = V2[index];

	up0 = U0[index];
	up1 = U1[index];
	up2 = U2[index];

	/* compute interval for triangle 1 */
	COMPUTE_INTERVALS(vp0, vp1, vp2, dv0, dv1, dv2, dv0dv1, dv0dv2, isect1[0], isect1[1]);

	/* compute interval for triangle 2 */
	COMPUTE_INTERVALS(up0, up1, up2, du0, du1, du2, du0du1, du0du2, isect2[0], isect2[1]);

	SORT(isect1[0], isect1[1]);
	SORT(isect2[0], isect2[1]);

	if (isect1[1] < isect2[0] || isect2[1] < isect1[0]) return 0;
	return 1;
}

void PullTriangle(const float(&tetr)[12], float(&out)[9], int index)
{
	switch (index)
	{
	case 1:
		out[0] = tetr[3];//p1
		out[1] = tetr[4];//
		out[2] = tetr[5];//
		out[3] = tetr[6];//p2
		out[4] = tetr[7];//
		out[5] = tetr[8];//
		out[6] = tetr[9];//p3
		out[7] = tetr[10];
		out[8] = tetr[11];
		break;

	case 2:
		out[0] = tetr[0];//p0
		out[1] = tetr[1];//
		out[2] = tetr[2];//
		out[3] = tetr[6];//p2
		out[4] = tetr[7];//
		out[5] = tetr[8];//
		out[6] = tetr[9];//p3
		out[7] = tetr[10];//
		out[8] = tetr[11];//
		break;

	case 3:
		out[0] = tetr[0];//p0
		out[1] = tetr[1];//
		out[2] = tetr[2];//
		out[3] = tetr[6];//p2
		out[4] = tetr[7];//
		out[5] = tetr[8];//
		out[6] = tetr[3];//p1
		out[7] = tetr[4];//
		out[8] = tetr[5];//
		break;

	case 4:
		out[0] = tetr[0];//p0
		out[1] = tetr[1];//
		out[2] = tetr[2];//
		out[3] = tetr[3];//p1
		out[4] = tetr[4];//
		out[5] = tetr[5];//
		out[6] = tetr[9];//p3
		out[7] = tetr[10];//
		out[8] = tetr[11];//
		break;
	default:
		std::cerr << "\nError (index<0 || index>4)\n";
		break;
	}
}
bool tetras_intersect(const float(&tetr0)[12], const float(&tetr1)[12]) {
	// Массивы для хранения треугольников из тетраэдров
	float tri0[9], tri1[9];

	// Перебор всех треугольников первого тетраэдра
	for (int i = 1; i <= 4; ++i) {
		PullTriangle(tetr0, tri0, i); // Достаём треугольник из первого тетраэдра

		// Перебор всех треугольников второго тетраэдра
		for (int j = 1; j <= 4; ++j) {
			PullTriangle(tetr1, tri1, j); // Достаём треугольник из второго тетраэдра

			// Проверка на пересечение текущих треугольников
			float V0[3] = { tri0[0], tri0[1], tri0[2] };
			float V1[3] = { tri0[3], tri0[4], tri0[5] };
			float V2[3] = { tri0[6], tri0[7], tri0[8] };

			float U0[3] = { tri1[0], tri1[1], tri1[2] };
			float U1[3] = { tri1[3], tri1[4], tri1[5] };
			float U2[3] = { tri1[6], tri1[7], tri1[8] };


			if (tri_tri_intersect(V0, V1, V2, U0, U1, U2)) {
				// Если хотя бы один треугольник пересекается, возвращаем true
				return true;
			}
		}
	}

	// Если не было найдено пересечений, возвращаем false
	return false;
}
int Muller(std::vector<std::vector<int>>& Out, int size, float** resultTetrUpPoints, float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2)
{
	int count = 0;
	int index1 = 0, index2 = 0;
	float arr0[12] = { 0 }, arr1[12] = { 0 };

	// Распараллеливаем цикл с использованием OpenMP
#pragma omp parallel for private(index1, index2, arr0, arr1) reduction(+:count)
	for (int i = 0; i < size; i++)
	{
		index1 = Out[i][0];
		index2 = Out[i][1];

		// Копирование данных в массивы arr0 и arr1
		for (int j = 0; j < 12; j++)
		{
			arr0[j] = resultTetrUpPoints[index1][j];
			arr1[j] = resultTetrUpPoints_2[index2][j];
		}

		// Проверка пересечения
		if (tetras_intersect(arr0, arr1))
		{
			// Синхронизация при добавлении в Out2
#pragma omp critical
			{
				Out2.push_back({ index1, index2 });
			}
			count++;
		}
	}

	return count;
}


//====================================================================================================//