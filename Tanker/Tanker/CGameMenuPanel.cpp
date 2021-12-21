#include "pch.h"
#include "CGameMenuPanel.h"
#include "CGame.h"

//菜单构造函数
CGameMenuPanel::CGameMenuPanel()
{
	//加载默认背景图
	m_ImgBackGround = Image::FromFile(_T("menu_background.png"));//此时该图在cpp得同级目录下,若不在则需要把路径加入进来
	//加载菜单项一:人机对战图
	m_MenuList[0].index = 0;//index表示第一项
	for(auto i=0;i<2;i++)
	{
		TCHAR path[MAX_PATH];//MAX_PATH默认库中得最大路径长度
		_stprintf_s(path, _T("menu_1player_%d.png"), i);//把第i张图格式化文件名,存放于path数组中
		auto imgPtr = Image::FromFile(path);//加载之前的图片,存放到指针中
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));//得到每张图的宽高值
		m_MenuList[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//把刚才加载的图片组合成一个元组(C++中也是键值对,python中不是),
		                                                            //并压入到菜单容器中
	 }
	//加载菜单项一:双人对战
	m_MenuList[1].index = 0;//同上
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);
		auto imgPtr = Image::FromFile(path);
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));
		m_MenuList[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));
	}
}
//菜单的析构函数
CGameMenuPanel::~CGameMenuPanel()
{

}
//设置菜单得文本--把父类的指针存放于文本中
void CGameMenuPanel::SetParent(CGame *ptr_G)
{
	m_PtrParent = ptr_G;
}
//绘制背景自己
void CGameMenuPanel::Draw(Graphics &gh) const
{
	gh.DrawImage(m_ImgBackGround, m_Rect);//画出背景图(初始化为NULL和0,0,800,600)
	//绘制子菜单
	for(auto menuList:m_MenuList)//数组也可以用:来遍历得
	{
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//获得菜单项得图片
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//获取菜单项的大小
		gh.DrawImage(img, rect);
	 }
}
//处理鼠标移动事件
void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//绘制子菜单
	for (auto &menuList:m_MenuList)
	{
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//获取菜单项的图片
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//获取菜单项图片的大小
		if (rect.Contains(pt))//判断是否包含当前鼠标位置
			menuList.index = 1;//Contains()为真--包含--那么显示第一张图片
		else menuList.index = 0;//否则显示第二张图片
	}
}
//处理鼠标左键抬起事件
void CGameMenuPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto &menuList = m_MenuList[0];//获取第一张图片(下标为0)
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//获取图片指针
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//获取图片指针
		if (rect.Contains(pt))//判断是否包含该店
		{
			//暂时不设置人家对战操作
			return;
		}
	}
	{
		auto &menuList = m_MenuList[1];//获取第二张图片(下标为1)
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//获取图片指针
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//获取图片指针
		if (rect.Contains(pt))//判断是否包含该店
		{
			//暂时不设置人家对战操作
			return;
		}
	}
}
