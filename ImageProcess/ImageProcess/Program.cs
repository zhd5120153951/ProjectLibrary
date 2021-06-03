using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace ImageProcess
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            //全局应用程序对象Application,为应用程序启用可视样式
            Application.EnableVisualStyles();
            //设置控件显示文本的方式为GDI方式
            Application.SetCompatibleTextRenderingDefault(false);
            //显示指定主窗体控件
            Application.Run(new PictureProcessing());
        }
    }
}
