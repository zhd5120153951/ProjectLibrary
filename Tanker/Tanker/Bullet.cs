using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _06_坦克大战_正式.Properties;

namespace _06_坦克大战_正式
{
    enum Tag
    {
        MyTank,
        EnemyTank
    }
    class Bullet:Movething
    {
        public Tag Tag { get; set; }

        public bool IsDestroy { get; set; }

        public Bullet(int x, int y, int speed,Direction dir,Tag tag)
        {
            IsDestroy = false;
            this.X = x;
            this.Y = y;
            this.Speed = speed;
            BitmapDown = Resources.BulletDown;
            BitmapUp = Resources.BulletUp;
            BitmapRight = Resources.BulletRight;
            BitmapLeft = Resources.BulletLeft;
            this.Dir = dir;
            this.Tag = tag;

            this.X -= Width / 2;
            this.Y -= Height / 2;
        }

        public override void DrawSelf()
        {
            base.DrawSelf();
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
                if (Y +Height/2+3 < 0)
                {
                    IsDestroy = true ; return;
                }
            }
            else if (Dir == Direction.Down)
            {
                if (Y + Height / 2 -3 > 450)
                {
                    IsDestroy=true; return;
                }
            }
            else if (Dir == Direction.Left)
            {
                if (X +Width/2-3 < 0)
                {
                    IsDestroy = true; return;
                }
            }
            else if (Dir == Direction.Right)
            {
                if (X+Width/2+3 > 450)
                {
                    IsDestroy = true; return;
                }
            }
            #endregion


            //检查有没有和其他元素发生碰撞

            Rectangle rect = GetRectangle();

            rect.X = X + Width / 2 - 3;
            rect.Y = Y + Height / 2 - 3;
            rect.Height = 3;
            rect.Width = 3;

            //1、墙 2、钢墙 3、坦克
            int xExplosion = this.X + Width / 2;
            int yExplosion = this.Y + Height / 2;

            NotMovething wall = null;
            if ( (wall=GameObjectManager.IsCollidedWall(rect)) != null)
            {
                IsDestroy=true;
                GameObjectManager.DestroyWall(wall);
                GameObjectManager.CreateExplosion(xExplosion, yExplosion);
                SoundMananger.PlayBlast();
                return;
            }
            if (GameObjectManager.IsCollidedSteel(rect) != null)
            {
                GameObjectManager.CreateExplosion(xExplosion, yExplosion);

                IsDestroy = true; return;
            }
            if (GameObjectManager.IsCollidedBoss(rect))
            {
                SoundMananger.PlayBlast();
                GameFramework.ChangeToGameOver(); return;
            }

            if (Tag == Tag.MyTank)
            {
                EnemyTank tank = null;
                if ( (tank = GameObjectManager.IsCollidedEnemyTank(rect)) != null)
                {
                    IsDestroy = true;
                    GameObjectManager.DestroyTank(tank);
                    GameObjectManager.CreateExplosion(xExplosion, yExplosion);
                    SoundMananger.PlayHit();
                    return;
                }
            }else if(Tag== Tag.EnemyTank)
            {
                MyTank mytank = null;
                if( (mytank = GameObjectManager.IsCollidedMyTank(rect)) != null)
                {
                    IsDestroy = true;
                    GameObjectManager.CreateExplosion(xExplosion, yExplosion);
                    SoundMananger.PlayBlast();
                    mytank.TakeDamage();

                    return;
                }
            } 
        }

        private void ChangeDirection() { }

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
    }
}
