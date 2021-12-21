#pragma once
#include <vector>
//向量及相关运算声明
namespace game_hit {
#if !defined(PI)
#define PI (3.14159265357)
#endif
	//全局0向量对象
	extern const CVector2D ZeroVector;
	//平面二维向量以及向量类(就是点集)
	//二维向量及其计算
	class CVector2D
	{
	public:
		float m_x, m_y;
		CVector2D();
		CVector2D(float x, float y);
		CVector2D(const CVector2D& other);
		CVector2D& operator=(const CVector2D& other);

		bool operator==(const CVector2D& other);//重载==运算符
		bool operator!=(const CVector2D& other);
		//向量运算
		void SetZero();//置0
		CVector2D operator-()const;//求反向量
		CVector2D operator+(const CVector2D& other)const;//向量求和
		CVector2D operator-(const CVector2D& other)const;//向量求差
		CVector2D operator*(float k)const;//与标量相乘
		CVector2D operator/(float k)const;//与标量相除
		CVector2D& operator+=(const CVector2D& other);//+=
		CVector2D& operator-=(const CVector2D& other);//-=
		CVector2D& operator*=(const CVector2D& other);//*=
		CVector2D& operator/=(const CVector2D& other);// /=
		float Magintude()const;//向量求模
		float AngleBetweenVector(const CVector2D& other);//求夹角
		void Normal();//向量标准化
		// 求法向量 :点积 = 0 可得 fx * vx  + fy* vy = 0 , 最简单的解为:
		CVector2D NormalizeLeft();//fx = -vy, fy = vx
		CVector2D NormalizeRight();//  fx =  vy, fy = -vx
		float operator*(const CVector2D& other)const;//向量点乘--点乘结果是一个数
	};
	//声明需要用到的图形向量形状
	class CLine {
	public:
		CLine();
		CLine(float x0, float y0, float x1, float y1);
		CVector2D m_startPos;//起点
		CVector2D m_endPos;//终点
	};
	//运动的点类
	class CMovePoint {
	public:
		CMovePoint();
		CMovePoint(float x, float y, float vx, float vy);
		CMovePoint(CVector2D pos, CVector2D speed);
	public:
		CVector2D m_Pos;//位置
		CVector2D m_Speed;//速度(注意:速度一定要和像素,帧率匹配:即为像素/帧率)
	};
	//圆结构体
	struct Circle {
		float x, y;
		float r;
	};
	//以下是类外的向量运算函数,不是类内的
	//求向量点乘
	inline float dotProduct(const CVector2D& v1, const CVector2D& v2);
	//标量的左乘
	inline CVector2D operator*(float k, const CVector2D& v);
	//距离计算公式
	inline float distance(const CVector2D& v1, const CVector2D& v2);
	// 给定初始位置，和时间T，计算新位置？
	// 给定向量 v,k,返回4个值 i'(k的标准化版本), j', i'和j'方向上的v 分量 a和b;
	// v = ai' + bj'
	inline void switchBasis(const CVector2D& v, const CVector2D& directionV, CVector2D& basis1, CVector2D &basis2, float& a, float& b);
	// 计算新坐标系中的独立分量的长度
	inline float component(CVector2D& v, CVector2D& directionV);
	// 计算新坐标系中的独立分量(结果是一个向量,也就是 vector 在 directionVector 上面的投影)
	inline CVector2D componentVector(CVector2D& v, CVector2D& directionV);

	//碰撞检测
	// 输入四个向量参数(两点确定一条直线--代表直线 ab, cd),判断是否相交，如果相交，输出交点位置,不交,输出原始位置
	// 相交返回 true, 否则返回false;
	inline bool IsCrossPoint(const CVector2D& a, const CVector2D& b, const CVector2D& c, const CVector2D& d, CVector2D* pCrossPoint);
	// 计算两条直线的相交时间
	// 返回 t 为时间 如果 t = [0,1]
	// t>1 : t 位于 B点一侧
	// t<0 : t 位于 A点一侧
	inline bool CrossPointTime(const CVector2D& p1, const CVector2D& v1, const CVector2D& p2, const CVector2D& v2, float* pt = nullptr);
	// 计算两个线段的交点
	inline bool CrossPoint(const CVector2D& a, const CVector2D& b, const CVector2D& c, const CVector2D& d, float* pt = nullptr);
	// 计算三角形外积
	inline float signed2DTriArea(CVector2D a, CVector2D b, CVector2D c);
	// 判断两条线段是否相交,如果是-返回true，否则-返回false;并且输出交点位置和相交比例(pCrossPointTime)
	inline bool IsCrossLineSegments(const CLine l1, const CLine l2, CVector2D *pCrossPoint = nullptr, float *pCrossPointTime = nullptr);
	// 判断移动的点(可以视为射线)，是否与直线产碰撞，如果碰撞，返回true,并输出碰撞的点和反射之后的速度;否则返回false
	inline bool IsHit(const CMovePoint& Ray, const CLine& line,
		CVector2D& newSpeed,
		CVector2D& crossPoint,
		float* pHitDelay = nullptr);

	//碰撞检测: 圆与旋转矩型,圆与圆是否相交
	// 计算两点之间的距离
	inline float distance(float x1, float y1, float x2, float y2);
	// 判断两个圆是否相交--用来判断是否碰撞
	inline bool IsCollision(const Circle& C1, const Circle& C2);
	// 圆型 是否与未旋转的矩型相交
	inline bool IsCollision(const Circle& C, const RectF& R);
	// 多个矩型同时判断
	inline bool IsCollisions(const Circle& C, const std::vector<RectF>& vRects);
	// 获得点在旋转之后的坐标
	inline PointF GetRotatePoint(const PointF& pt, const float fRadius, const PointF& ptCenter, const float dir);
	// 判断圆与旋转矩型相交
    // theta : 为距型的角度
	inline bool IsCollision(const Circle& C, const RectF& R, const float alpha);
	// 矩形旋转一定角度之后， 四个顶点的坐标
	inline void GetRotateRectPoints(const RectF& R, const float dir, std::vector<PointF>& vPoints);
}

