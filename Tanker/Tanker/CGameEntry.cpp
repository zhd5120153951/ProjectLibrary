#include "pch.h"
#include "CGameEntry.h"
//���ƶ������Ա����
//Ĭ�Ϲ���
CGameEntryMoveable::CGameEntryMoveable()
{

}
//��������
CGameEntryMoveable::CGameEntryMoveable(const CGameEntryMoveable& rhs)
{
	if (this != &rhs)//�Լ����ܿ��������Լ�
	{
		m_bActive = rhs.m_bActive;//�Ƿ���Ч
		m_Rect = rhs.m_Rect;      //ռ�õ�����
		m_Speed = rhs.m_Speed;    //�ƶ��ٶ�
		m_Direction = rhs.m_Direction; //��ǰ�Ƕ�
		m_TurnDirection = rhs.m_TurnDirection;//��ת�Ƕ�
	}
}
//����
CGameEntryMoveable::~CGameEntryMoveable()
{

}
//���ƶ�����Ĺ�ͬ����ʵ��--��ת
void CGameEntryMoveable::RotateRight()
{
	m_Direction += m_TurnDirection;//��ǰ�Ƕ�+��ת�Ƕ�(90��)
	if (m_Direction>=360)
		m_Direction -= 360;//��תһ��(���ﱣ֤�Ƕ���0-360֮��)
}
//��ת
void CGameEntryMoveable::RotateLeft()
{
	m_Direction -= m_TurnDirection;//��ǰ�Ƕ�+��ת�Ƕ�(90��)
	if (m_Direction < 0)
		m_Direction=static_cast<float>(360+m_Direction) ;//��תһ��
}
//ǰ��
void CGameEntryMoveable::Forward()
{
	//���ݵ�ǰ�ٶȷֽ���x,y�᷽����ٶ�
	m_Rect.X += static_cast<Gdiplus::REAL>((m_Speed)*sin(this->GetDirectionArc()));
	m_Rect.Y -= static_cast<Gdiplus::REAL>((m_Speed)*cos(this->GetDirectionArc()));
}
//����
void CGameEntryMoveable::Backward()
{
	m_Rect.X -= static_cast<Gdiplus::REAL>((m_Speed)*sin(GetDirectionArc()));
	m_Rect.Y += static_cast<Gdiplus::REAL>((m_Speed)*cos(GetDirectionArc()));
}
//�õ���ǰ�ٶ�x����
float CGameEntryMoveable::GetSpeedX() const
{
	return static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
}
//�õ���ǰ�ٶ�y�����ٶ�
float CGameEntryMoveable::GetSpeedY() const
{
	return static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
}
//��һ��ǰ���ķ���λ��
RectF CGameEntryMoveable::ForwardNextRect() const
{
	RectF rc = m_Rect;//��ȡ�õ�ǰλ��
	//����ǰ��������
	rc.X += static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
	return rc;
}
//��һ�����˵ķ���λ��
RectF CGameEntryMoveable::BackwardNextRect() const
{
	RectF rc = m_Rect;
	//���Ϻ��˵�����
	rc.X -= static_cast<Gdiplus::REAL>(m_Speed*sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>(m_Speed*cos(GetDirectionArc()));
	return rc;
}
//��ȡͷ��λ��:ͼƬ�Ǿ���:ͷ��λ�þ��ǽǶ�Ϊ0ʱ
//�������ĵ�Ϊ�뾶=�Խ���/2
PointF CGameEntryMoveable::GetHeadPos() const
{
	//�Ȼ�����ĵ�λ��
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	//float Radius = sqrt(pow(m_Rect.Width, 2) + pow(m_Rect.Height, 2)) / 2;
	float Radius = sqrt(pow(m_Rect.Width / 2, 2) + pow(m_Rect.Height / 2, 2));
	ptHead.X += Radius * sin(GetDirectionArc());
	ptHead.Y -= Radius * cos(GetDirectionArc());
	return ptHead;
}
//��ȡ���ĵ�
PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF ptCenter = PointF(m_Rect.X + m_Rect.Width / 2, m_Rect.Y + m_Rect.Height / 2);
	return ptCenter;
}
//�������ĵ�����
void CGameEntryMoveable::SetCenterPoint(const PointF& CenterPoint)
{
	PointF point = this->GetCenterPoint();
	point.X = CenterPoint.X - point.X;
	point.Y = CenterPoint.Y - point.Y;
	m_Rect.Offset(point);//Ӧ�ô����ĵ�
}
//���÷���Ƕ�--������
void CGameEntryMoveable::SetDirectionArc(float dirArc)
{
	m_Direction = dirArc * 180.0f / PI;
}
//���÷���Ƕ�--�Ƕ���
void CGameEntryMoveable::SetDirection(float dir)
{
	m_Direction = dir;
}
//�õ�ת��Ƕ�--������
float CGameEntryMoveable::GetDirectionArc() const
{
	return m_Direction * PI / 180.0f;
}
//�õ�ת��Ƕ�--�Ƕ���
float CGameEntryMoveable::GetDirecton() const
{
	return m_Direction;
}
//����ת��Ƕ�--������
void CGameEntryMoveable::SetTurnDirectionArc(float turnDir)
{
	m_TurnDirection = turnDir * PI / 180.0f;
}
//����ת��Ƕ�--�Ƕ���
void CGameEntryMoveable::SetTurnDirection(float turnDir)
{
	m_TurnDirection = turnDir;
}
//�õ�ת��Ƕ�--������
float CGameEntryMoveable::GetTurnDirectionArc() const
{
	return m_TurnDirection * PI / 180.0f;
}
//�õ�ת��Ƕ�--�Ƕ���
float CGameEntryMoveable::GetTurnDirection() const
{
	return m_TurnDirection;
}
//�б��Ƿ���Чת��
bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}
//�����Ƿ�ת��
void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}
//����ͼ��ռ�÷�Χ(�Ƕ�ά��)
void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_Rect = rect;
}
//�õ�ռ�÷�Χ
RectF CGameEntryMoveable::GetRect() const
{
	return m_Rect;
}
//���ö����ٶ�
void CGameEntryMoveable::SetObjectSpeed(float speed)
{
	m_Speed = speed;
}
//��ȡ�ٶ�
float CGameEntryMoveable::GetObjectSpeed() const
{
	return m_Speed;
}

//���������һ����������(�������ظ�ֵ�����=,��Ϊ�˿�����������ص�--����������ǳ����,û���漰�����)
CGameEntryMoveable& CGameEntryMoveable::operator=(const CGameEntryMoveable& rhs)
{
	if (this != &rhs)
	{
		this->m_bActive = rhs.m_bActive;//�Ƿ���Ч
		this->m_Rect = rhs.m_Rect;      //ռ������
		this->m_Speed = rhs.m_Speed;    //�ƶ��ٶ�
		this->m_Direction = rhs.m_Direction;//��ǰ�Ƕ�
		this->m_TurnDirection = rhs.m_TurnDirection;//��ת�Ƕ�
	}
	return *this;//���ص��Ƕ���---��*
}