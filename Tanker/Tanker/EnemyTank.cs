using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _06_坦克大战_正式
{
    class EnemyTank:Movething
    {
        public int ChangeDirSpeed { get; set; }
        private int changeDirCount = 0;
        public int AttackSpeed { get; set; }
        private int attackCount = 0;
        private Random r = new Random();
        public EnemyTank(int x, int y, int speed,Bitmap bmpDown,Bitmap bmpUp,Bitmap bmpRight,Bitmap bmpLeft)
        {
            this.X = x;
            this.Y = y;
            this.Speed = speed;
            BitmapDown = bmpDown;
            BitmapUp = bmpUp;
            BitmapRight = bmpRight;
            BitmapLeft = bmpLeft;
            this.Dir = Direction.Down;
            AttackSpeed = 60;
            ChangeDirSpeed = 70;
        }


        public override void Update()
        {
            MoveCheck();//移动检查
            Move();
            AttackCheck();
            AutoChangeDirection();

            base.Update();
        }

        private void MoveCheck()
        {

            #region 检查有没有超过窗体边界
            if (Dir == Direction.Up)
            {
                if (Y - Speed < 0)
                {
                    ChangeDirection(); return;
                }
            }
            else if (Dir == Direction.Down)
            {
                if (Y + Speed + Height > 450)
                {
                    ChangeDirection(); return;
                }
            }
            else if (Dir == Direction.Left)
            {
                if (X - Speed < 0)
                {
                    ChangeDirection(); return;
                }
            }
            else if (Dir == Direction.Right)
            {
                if (X + Speed + Width > 450)
                {
                    ChangeDirection(); return;
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
                ChangeDirection(); return;
            }
            if (GameObjectManager.IsCollidedSteel(rect) != null)
            {
                ChangeDirection(); return;
            }
            if (GameObjectManager.IsCollidedBoss(rect))
            {
                ChangeDirection(); return;
            }
        }
        private void AutoChangeDirection()
        {
            changeDirCount++;
            if (changeDirCount < ChangeDirSpeed) return;
            ChangeDirection();
            changeDirCount = 0;
        }
        private void ChangeDirection()
        {
            //4
            //Random r = new Random(); 种子
            //算法 伪随机数1000  
            while (true)
            {
                Direction dir = (Direction)r.Next(0, 4);
                if (dir == Dir)
                {
                    continue;
                }
                {
                    Dir = dir;break;
                }
            }
            // 0 1 2 3
            MoveCheck();
        }

        private void Move()
        {
            

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

        private void AttackCheck()
        {
            attackCount++;
            if (attackCount < AttackSpeed) return;

            Attack();
            attackCount = 0;
        }

        private void Attack()
        {
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


            GameObjectManager.CreateBullet(x, y, Tag.EnemyTank, Dir);
        }
    }
}
