using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _06_坦克大战_正式
{
    enum GameState
    {
        Running,
        GameOver
    }

    class GameFramework
    {
        public static Graphics g;
        private static GameState gameState = GameState.Running;

        public static void Start()
        {
            SoundMananger.InitSound();
            GameObjectManager.Start();
            GameObjectManager.CreateMap();
            GameObjectManager.CreateMyTank();
            SoundMananger.PlayStart();
        }

        public static void Update()
        {
            //FPS
            //GameObjectManager.DrawMap();
            //GameObjectManager.DrawMyTank();
            

            if (gameState == GameState.Running)
            {
                GameObjectManager.Update();
            }else if(gameState == GameState.GameOver)
            {
                GameOverUpdate();
            }
        }

        private static void GameOverUpdate()
        {
            Bitmap bmp = Properties.Resources.GameOver;
            bmp.MakeTransparent(Color.Black);
            int x = 450 / 2 - Properties.Resources.GameOver.Width / 2;
            int y = 450/2- Properties.Resources.GameOver.Height / 2;
            g.DrawImage(bmp, x, y);
        }

        public static void ChangeToGameOver()
        {
            gameState = GameState.GameOver;
        }

    }
}
