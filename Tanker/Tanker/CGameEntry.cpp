#include "pch.h"
#include "CGameEntry.h"
//可移动子类成员函数
//默认构造
CGameEntryMoveable::CGameEntryMoveable()
{

}
//拷贝构造
CGameEntryMoveable::CGameEntryMoveable(const CGameEntryMoveable& rhs)
{
	if (this != &rhs)//自己不能拷贝构造自己
	{
		m_bActive = rhs.m_bActive;//是否有效
		m_Rect = rhs.m_Rect;      //占用的区域
		m_Speed = rhs.m_Speed;    //移动速度
		m_Direction = rhs.m_Direction; //当前角度
		m_TurnDirection = rhs.m_TurnDirection;//旋转角度
	}
}
//析构
CGameEntryMoveable::~CGameEntryMoveable()
{

}
//可移动对象的共同属性实现--右转
void CGameEntryMoveable::RotateRight()
{
	m_Direction += m_TurnDirection;//当前角度+旋转角度(90°)
	if (m_Direction>=360)
		m_Direction -= 360;//旋转一周(这里保证角度在0-360之间)
}
//左转
void CGameEntryMoveable::RotateLeft()
{
	m_Direction -= m_TurnDirection;//当前角度+旋转角度(90°)
	if (m_Direction < 0)
		m_Direction=static_cast<float>(360+m_Direction) ;//旋转一周
}
//前进
void CGameEntryMoveable::Forward()
{
	//根据当前速度分解在x,y轴方向的速度
	m_Rect.X += static_cast<Gdiplus::REAL>((m_Speed)*sin(this->GetDirectionArc()));
	m_Rect.Y -= static_cast<Gdiplus::REAL>((m_Speed)*cos(this->GetDirectionArc()));
}
//后退
void CGameEntryMoveable::Backward()
{
	m_Rect.X -= static_cast<Gdiplus::REAL>((m_Speed)*sin(GetDirectionArc()));
	m_Rect.Y += static_cast<Gdiplus::REAL>((m_Speed)*cos(GetDirectionArc()));
}
//得到当前速度x方向
float CGameEntryMoveable::GetSpeedX() const
{
	return static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
}
//得到当前速度y方向速度
float CGameEntryMoveable::GetSpeedY() const
{
	return static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
}
//下一步前进的方块位置
RectF CGameEntryMoveable::ForwardNextRect() const
{
	RectF rc = m_Rect;//先取得当前位置
	//加上前进的增量
	rc.X += static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
	return rc;
}
//下一步后退的方块位置
RectF CGameEntryMoveable::BackwardNextRect() const
{
	RectF rc = m_Rect;
	//加上后退的增量
	rc.X -= static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
	return rc;
}
//获取头部位置:图片是矩形:头部位置就是角度为0时
//距离中心点为半径=对角线/2
PointF CGameEntryMoveable::GetHeadPos() const
{
	//先获得中心点位置
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	//float Radius = sqrt(pow(m_Rect.Width, 2) + pow(m_Rect.Height, 2)) / 2;
	float Radius = sqrt(pow(m_Rect.Width / 2, 2) + pow(m_Rect.Height / 2, 2));
	ptHead.X += Radius * sin(GetDirectionArc());
	ptHead.Y -= Radius * cos(GetDirectionArc());
	return ptHead;
}
//获取中心点
PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF ptCenter = PointF(m_Rect.X + m_Rect.Width / 2, m_Rect.Y + m_Rect.Height / 2);
	return ptCenter;
}
//设置中心点坐标
void CGameEntryMoveable::SetCenterPoint(const PointF& CenterPoint)
{
	PointF point = this->GetCenterPoint();
	point.X = CenterPoint.X - point.X;
	point.Y = CenterPoint.Y - point.Y;
	m_Rect.Offset(point);//应用此中心点
}
//设置方向角度--弧度制
void CGameEntryMoveable::SetDirectionArc(float dirArc)
{
	m_Direction = dirArc * 180.0f / PI;
}
//设置方向角度--角度制
void CGameEntryMoveable::SetDirection(float dir)
{
	m_Direction = dir;
}
//得到转向角度--弧度制
float CGameEntryMoveable::GetDirectionArc() const
{
	return m_Direction * PI / 180.0f;
}
//得到转向角度--角度制
float CGameEntryMoveable::GetDirecton() const
{
	return m_Direction;
}
//设置转向角度--弧度制
void CGameEntryMoveable::SetTurnDirectionArc(float turnDir)
{
	m_TurnDirection = turnDir * PI / 180.0f;
}
//设置转向角度--角度制
void CGameEntryMoveable::SetTurnDirection(float turnDir)
{
	m_TurnDirection = turnDir;
}
//得到转向角度--弧度制
float CGameEntryMoveable::GetTurnDirectionArc() const
{
	return m_TurnDirection * PI / 180.0f;
}
//得到转向角度--角度制
float CGameEntryMoveable::GetTurnDirection() const
{
	return m_TurnDirection;
}
//判别是否有效转向
bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}
//设置是否转向
void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}
//设置图形占用范围(是二维的)
void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_Rect = rect;
}
//得到占用范围
RectF CGameEntryMoveable::GetRect() const
{
	return m_Rect;
}
//设置对象速度
void CGameEntryMoveable::SetObjectSpeed(float speed)
{
	m_Speed = speed;
}
//获取速度
float CGameEntryMoveable::GetObjectSpeed() const
{
	return m_Speed;
}

//运算符重载一般放在最后定义(这里重载赋值运算符=,是为了拷贝构造而重载的--而且这里是浅拷贝,没有涉及到深拷贝)
CGameEntryMoveable& CGameEntryMoveable::operator=(const CGameEntryMoveable& rhs)
{
	if (this != &rhs)
	{
		this->m_bActive = rhs.m_bActive;//是否有效
		this->m_Rect = rhs.m_Rect;      //占用区域
		this->m_Speed = rhs.m_Speed;    //移动速度
		this->m_Direction = rhs.m_Direction;//当前角度
		this->m_TurnDirection = rhs.m_TurnDirection;//旋转角度
	}
	return *this;//返回的是对象---加*
}