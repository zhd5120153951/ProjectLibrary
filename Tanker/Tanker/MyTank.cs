using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using _06_坦克大战_正式.Properties;

namespace _06_坦克大战_正式
{
    class MyTank:Movething
    {

        public bool IsMoving { get; set; }
        public int HP { get; set; }
        private int originalX;
        private int originalY;
        //
        public MyTank(int x,int y,int speed)
        {
            IsMoving = false;
            this.X = x;
            this.Y = y;
            originalX = x;
            originalY = y;
            this.Speed = speed;
            BitmapDown = Resources.MyTankDown;
            BitmapUp = Resources.MyTankUp;
            BitmapRight = Resources.MyTankRight;
            BitmapLeft = Resources.MyTankLeft;
            this.Dir = Direction.Up;
            HP = 4;
        }

        public override void Update()
        {
            MoveCheck();//移动检查
            Move();

            base.Update();
        }

        private void MoveCheck()
        {

            #region 检查有没有超过窗体边界
            if (Dir == Direction.Up)
            {
                if (Y - Speed < 0)
                {
                    IsMoving = false; return;
                }
            }
            else if (Dir == Direction.Down)
            {
                if (Y + Speed + Height > 450)
                {
                    IsMoving = false; return;
                }
            }
            else if (Dir == Direction.Left)
            {
                if (X - Speed < 0)
                {
                    IsMoving = false; return;
                }
            }
            else if (Dir == Direction.Right)
            {
                if (X + Speed + Width > 450)
                {
                    IsMoving = false; return;
                }
            }
            #endregion


            //检查有没有和其他元素发生碰撞

            Rectangle rect = GetRectangle();

            switch (Dir)
            {
                case Direction.Up:
                    rect.Y -= Speed;
                    break;
                case Direction.Down:
                    rect.Y += Speed;
                    break;
                case Direction.Left:
                    rect.X -= Speed;
                    break;
                case Direction.Right:
                    rect.X += Speed;
                    break;
            }

            if (GameObjectManager.IsCollidedWall(rect) != null)
            {
                IsMoving = false; return;
            }
            if (GameObjectManager.IsCollidedSteel(rect) != null)
            {
                IsMoving = false; return;
            }
            if (GameObjectManager.IsCollidedBoss(rect))
            {
                IsMoving = false; return;
            }
        }

        private void Move()
        {
            if (IsMoving == false) return;

            switch (Dir)
            {
                case Direction.Up:
                    Y -= Speed;
                    break;
                case Direction.Down:
                    Y += Speed;
                    break;
                case Direction.Left:
                    X -= Speed;
                    break;
                case Direction.Right:
                    X += Speed;
                    break;
            }
        }
        // GameMainThread  KeyDown
        // 1 2 
        public void KeyDown(KeyEventArgs args)
        {
            switch (args.KeyCode)
            {
                case Keys.W:
                    Dir = Direction.Up;
                    IsMoving = true;
                    break;
                case Keys.S:
                    Dir = Direction.Down;
                    IsMoving = true;
                    break;
                case Keys.A:
                    Dir = Direction.Left;
                    IsMoving = true;
                    break;
                case Keys.D:
                    Dir = Direction.Right;
                    IsMoving = true;
                    break;
                case Keys.Space:
                    Attack();
                    break;
            }
        }
        private void Attack()
        {
            SoundMananger.PlayFire();
            //发射子弹
            int x = this.X;
            int y = this.Y;

            switch (Dir)
            {
                case Direction.Up:
                    x = x + Width / 2;
                    break;
                case Direction.Down:
                    x = x + Width / 2;
                    y += Height;
                    break;
                case Direction.Left:
                    y = y + Height / 2;
                    break;
                case Direction.Right:
                    x += Width;
                    y = y + Height / 2;
                    break;
            }


            GameObjectManager.CreateBullet(x,y,Tag.MyTank,Dir);
        }
        public void KeyUp(KeyEventArgs args)
        {
            switch (args.KeyCode)
            {
                case Keys.W:
                    IsMoving = false;
                    break;
                case Keys.S:
                    IsMoving = false;
                    break;
                case Keys.A:
                    IsMoving = false;
                    break;
                case Keys.D:
                    IsMoving = false;
                    break;
            }
        }
        public void TakeDamage()
        {
            HP--;

            if (HP <= 0)
            {
                X = originalX;
                Y = originalY;
                HP = 4;
            }
        }
    }
}
