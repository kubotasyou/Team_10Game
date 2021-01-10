#include "Collision.h"

bool Collision::SphereToSphere(const Sphere & sphere1, const Sphere & sphere2)
{
	float x = (sphere2.center.m128_f32[0] - sphere1.center.m128_f32[0]) * (sphere2.center.m128_f32[0] - sphere1.center.m128_f32[0]);
	float y = (sphere2.center.m128_f32[1] - sphere1.center.m128_f32[1]) * (sphere2.center.m128_f32[1] - sphere1.center.m128_f32[1]);
	float z = (sphere2.center.m128_f32[2] - sphere1.center.m128_f32[2]) * (sphere2.center.m128_f32[2] - sphere1.center.m128_f32[2]);

	float rad2 = (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);

	if (x + y + z >= rad2)
	{
		return false;
	}

	return true;
}

bool Collision::CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
	XMFLOAT3 normal, float distance)
{
	XMVECTOR sphere = XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1.0f);
	XMVECTOR panel = XMVectorSet(normal.x, normal.y, normal.z, 0.0f);

	XMVECTOR distV = XMVector3Dot(sphere, panel);

	//法線ベクトルを決めることによって、平面が定義される。
	//distanceは法線ベクトル方向に動かした大きさ。
	float dist = distV.m128_f32[0] - distance;

	////fabsf(絶対値の結果をfloatで返す)
	//float absDist = fabsf(dist);
	//if (absDist > radius)
	//{
	//	return false;
	//}

	//距離の絶対値が、半径よりも大きければ当たっていない。
	if (fabsf(dist) > radius)
	{
		return false;
	}

	return true;
}

void Collision::ClosestPtPointToTriangle(XMFLOAT3 point, XMFLOAT3 triangleP0, XMFLOAT3 triangleP1, XMFLOAT3 triangleP2, XMFLOAT3 closester)
{
	using namespace DirectX;

	XMVECTOR pt = XMVectorSet(point.x, point.y, point.z, 1);
	XMVECTOR p0 = XMVectorSet(triangleP0.x, triangleP0.y, triangleP0.z, 1);
	XMVECTOR p1 = XMVectorSet(triangleP1.x, triangleP1.y, triangleP1.z, 1);
	XMVECTOR p2 = XMVectorSet(triangleP2.x, triangleP2.y, triangleP2.z, 1);
	XMVECTOR closest = XMVectorSet(closester.x, closester.y, closester.z, 0);

	XMVECTOR p0_p1 = p1 - p0; //p1→p0の辺のベクトル　辺①
	XMVECTOR p0_p2 = p2 - p0; //p2→p0の辺のベクトル　辺②
	XMVECTOR p1_p2 = p2 - p1; //p2→p1の辺のベクトル　辺③
	XMVECTOR p0_pt = pt - p0;       //点→p0の長さのベクトル
	XMVECTOR p1_pt = pt - p1;       //点→p1の長さのベクトル
	XMVECTOR p2_pt = pt - p2;       //点→p2の長さのベクトル

	//内積は、VECTOR型で返してはいるけど、xyzどれにもおんなじ値が入っているので実質float型が返ってる。
	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);//辺①と、点との距離の内積
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);//辺②と、点との距離の内積

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);//辺①と、点との距離の内積
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);//辺②と、点との距離の内積
	//XMVECTOR d4 = XMVector3Dot(p1_p2, p1_pt);//辺③と、点との距離の内積

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);//辺①と、点との距離の内積
	//XMVECTOR d5 = XMVector3Dot(p1_p2, p2_pt);//辺①と、点との距離の内積
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);//辺②と、点との距離の内積

	//辺①と、点との距離の内積 がマイナス && 辺②と、点との距離の内積 がマイナスの時
	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		//p0が近接点となる。
		closest = p0;
		return;
	}

	//-----------------------------------------------------------------

	//pointが"p1"の外側の頂点領域の中にあるのかどうかチェック

	//
	if (d3.m128_f32[0] >= 0.0f && /*d4.m128_f32[0] <= 0.0f*/ d4.m128_f32[0] <= d3.m128_f32[0])
	{
		//p1が接近点となる
		closest = p1;
		return;
	}

	//-----------------------------------------------------------------

	//pointが"p2"の外側の頂点領域の中にあるのかどうかチェック

	//
	if (d5.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0] /*d6.m128_f32[0] >= 0.0f*/)
	{
		//p2が接近点となる
		closest = p2;
		return;
	}

	//-----------------------------------------------------------------

	//pointが辺①の辺領域の中にあるかどうかをチェックし、
	//あればpointの辺①上に対する射影を返す。

	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];

	//XMVECTOR vc = XMVector3Dot(XMVector3Cross(p0_p1, p0_p2), XMVector3Cross(p0_pt, p1_pt));

	if (vc/*.m128_f32[0]*/ <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		//辺①が接近点となる
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		closest = p0 + v * p0_p1;
		return;
	}

	//---------------------------------------------------------------------------

	//pointが辺②の辺領域の中にあるかどうかをチェックし、
	//あればpointの辺②上に対する射影を返す。

	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];

	//XMVECTOR vb = XMVector3Dot(XMVector3Cross(p1_p2, p0_p1), XMVector3Cross(p1_pt, p2_pt));

	if (vb/*.m128_f32[0]*/ <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		//辺②が接近点となる
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		closest = p0 + w * p0_p2;
		return;
	}

	//---------------------------------------------------------------------------

	//pointが辺③の辺領域の中にあるかどうかをチェックし、
	//あればpointのp1_p2上に対する射影を返す。

	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];

	//XMVECTOR va = XMVector3Dot(XMVector3Cross(p0_p2, p1_p2), XMVector3Cross(p2_pt, p0_pt));

	if (va/*.m128_f32[0] */<= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) <= 0.0f)
	{
		//p1_p2の辺領域が接近点となる
		float x = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		closest = p1 + x * p1_p2;
		return;
	}

	float denom = 1.0f / (va/*.m128_f32[0]*/ + vb/*.m128_f32[0] */+ vc/*.m128_f32[0]*/);
	float v = vb/*.m128_f32[0]*/ * denom;
	float w = vc/*.m128_f32[0]*/ * denom;
	closest = p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphereToTriangle(XMFLOAT3 sphereCenter, float radius, XMFLOAT3 triangleP0, XMFLOAT3 triangleP1, XMFLOAT3 triangleP2, XMVECTOR inter)
{
	//当たったとされる場所との最近接点
	XMFLOAT3 point = { 0,0,0 };
	ClosestPtPointToTriangle(sphereCenter, triangleP0, triangleP1, triangleP2, point);

	XMVECTOR pt = XMVectorSet(point.x, point.y, point.z, 1);
	XMVECTOR center = XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1);

	//点pと球の中心点の差分
	XMVECTOR v = pt - center;

	//距離の二乗を求める
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > radius * radius)
	{
		return false;
	}

	return true;
}
