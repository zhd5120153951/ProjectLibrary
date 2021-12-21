#pragma once
#include <vector>
//�����������������
namespace game_hit {
#if !defined(PI)
#define PI (3.14159265357)
#endif
	//ȫ��0��������
	extern const CVector2D ZeroVector;
	//ƽ���ά�����Լ�������(���ǵ㼯)
	//��ά�����������
	class CVector2D
	{
	public:
		float m_x, m_y;
		CVector2D();
		CVector2D(float x, float y);
		CVector2D(const CVector2D& other);
		CVector2D& operator=(const CVector2D& other);

		bool operator==(const CVector2D& other);//����==�����
		bool operator!=(const CVector2D& other);
		//��������
		void SetZero();//��0
		CVector2D operator-()const;//������
		CVector2D operator+(const CVector2D& other)const;//�������
		CVector2D operator-(const CVector2D& other)const;//�������
		CVector2D operator*(float k)const;//��������
		CVector2D operator/(float k)const;//��������
		CVector2D& operator+=(const CVector2D& other);//+=
		CVector2D& operator-=(const CVector2D& other);//-=
		CVector2D& operator*=(const CVector2D& other);//*=
		CVector2D& operator/=(const CVector2D& other);// /=
		float Magintude()const;//������ģ
		float AngleBetweenVector(const CVector2D& other);//��н�
		void Normal();//������׼��
		// ������ :��� = 0 �ɵ� fx * vx  + fy* vy = 0 , ��򵥵Ľ�Ϊ:
		CVector2D NormalizeLeft();//fx = -vy, fy = vx
		CVector2D NormalizeRight();//  fx =  vy, fy = -vx
		float operator*(const CVector2D& other)const;//�������--��˽����һ����
	};
	//������Ҫ�õ���ͼ��������״
	class CLine {
	public:
		CLine();
		CLine(float x0, float y0, float x1, float y1);
		CVector2D m_startPos;//���
		CVector2D m_endPos;//�յ�
	};
	//�˶��ĵ���
	class CMovePoint {
	public:
		CMovePoint();
		CMovePoint(float x, float y, float vx, float vy);
		CMovePoint(CVector2D pos, CVector2D speed);
	public:
		CVector2D m_Pos;//λ��
		CVector2D m_Speed;//�ٶ�(ע��:�ٶ�һ��Ҫ������,֡��ƥ��:��Ϊ����/֡��)
	};
	//Բ�ṹ��
	struct Circle {
		float x, y;
		float r;
	};
	//������������������㺯��,�������ڵ�
	//���������
	inline float dotProduct(const CVector2D& v1, const CVector2D& v2);
	//���������
	inline CVector2D operator*(float k, const CVector2D& v);
	//������㹫ʽ
	inline float distance(const CVector2D& v1, const CVector2D& v2);
	// ������ʼλ�ã���ʱ��T��������λ�ã�
	// �������� v,k,����4��ֵ i'(k�ı�׼���汾), j', i'��j'�����ϵ�v ���� a��b;
	// v = ai' + bj'
	inline void switchBasis(const CVector2D& v, const CVector2D& directionV, CVector2D& basis1, CVector2D &basis2, float& a, float& b);
	// ����������ϵ�еĶ��������ĳ���
	inline float component(CVector2D& v, CVector2D& directionV);
	// ����������ϵ�еĶ�������(�����һ������,Ҳ���� vector �� directionVector �����ͶӰ)
	inline CVector2D componentVector(CVector2D& v, CVector2D& directionV);

	//��ײ���
	// �����ĸ���������(����ȷ��һ��ֱ��--����ֱ�� ab, cd),�ж��Ƿ��ཻ������ཻ���������λ��,����,���ԭʼλ��
	// �ཻ���� true, ���򷵻�false;
	inline bool IsCrossPoint(const CVector2D& a, const CVector2D& b, const CVector2D& c, const CVector2D& d, CVector2D* pCrossPoint);
	// ��������ֱ�ߵ��ཻʱ��
	// ���� t Ϊʱ�� ��� t = [0,1]
	// t>1 : t λ�� B��һ��
	// t<0 : t λ�� A��һ��
	inline bool CrossPointTime(const CVector2D& p1, const CVector2D& v1, const CVector2D& p2, const CVector2D& v2, float* pt = nullptr);
	// ���������߶εĽ���
	inline bool CrossPoint(const CVector2D& a, const CVector2D& b, const CVector2D& c, const CVector2D& d, float* pt = nullptr);
	// �������������
	inline float signed2DTriArea(CVector2D a, CVector2D b, CVector2D c);
	// �ж������߶��Ƿ��ཻ,�����-����true������-����false;�����������λ�ú��ཻ����(pCrossPointTime)
	inline bool IsCrossLineSegments(const CLine l1, const CLine l2, CVector2D *pCrossPoint = nullptr, float *pCrossPointTime = nullptr);
	// �ж��ƶ��ĵ�(������Ϊ����)���Ƿ���ֱ�߲���ײ�������ײ������true,�������ײ�ĵ�ͷ���֮����ٶ�;���򷵻�false
	inline bool IsHit(const CMovePoint& Ray, const CLine& line,
		CVector2D& newSpeed,
		CVector2D& crossPoint,
		float* pHitDelay = nullptr);

	//��ײ���: Բ����ת����,Բ��Բ�Ƿ��ཻ
	// ��������֮��ľ���
	inline float distance(float x1, float y1, float x2, float y2);
	// �ж�����Բ�Ƿ��ཻ--�����ж��Ƿ���ײ
	inline bool IsCollision(const Circle& C1, const Circle& C2);
	// Բ�� �Ƿ���δ��ת�ľ����ཻ
	inline bool IsCollision(const Circle& C, const RectF& R);
	// �������ͬʱ�ж�
	inline bool IsCollisions(const Circle& C, const std::vector<RectF>& vRects);
	// ��õ�����ת֮�������
	inline PointF GetRotatePoint(const PointF& pt, const float fRadius, const PointF& ptCenter, const float dir);
	// �ж�Բ����ת�����ཻ
    // theta : Ϊ���͵ĽǶ�
	inline bool IsCollision(const Circle& C, const RectF& R, const float alpha);
	// ������תһ���Ƕ�֮�� �ĸ����������
	inline void GetRotateRectPoints(const RectF& R, const float dir, std::vector<PointF>& vPoints);
}

