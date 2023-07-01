#include "enemy.h"
#include "image.h"

static enum enemyStatus enemyStatusSequence[8] = {
    enemy_normal,
    enemy_down0,
    enemy_down1,
    enemy_down2,
    enemy_down3,
    enemy_down4,
    enemy_down5,
    enemy_destroy
};

void enemyDraw(struct enemy* e)
{
    IMAGE* imgEnemy = e->imgArrEnemy[e->status];
    IMAGE* imgEnemyMask = e->imgArrEnemyMask[e->status];
    putTransparentImage(e->super.x, e->super.y, imgEnemyMask, imgEnemy);
}

void enemyUpdate(struct enemy* e)
{
    //  敌机移动
    e->super.y += e->v;

    //  如果生命为0，执行爆炸序列
    if (e->life == 0)
    {
        e->enemyDownCnt++;
        //  15帧变化一次状态
        if (e->enemyDownCnt >= 15)
        {
            e->enemyDownCnt = 0;

            //  向后变换状态，直到状态为lastStatusBeforeDestroy
            if (e->status < e->lastStatusBeforeDestroy)
                e->status = enemyStatusSequence[e->status + 1];
            else
                e->status = enemy_destroy;
        }
    }
}

void enemyHited(struct enemy* e)
{
    e->life--;
    if (e->life == 0)
        e->status = enemy_down0;
}

void enemyInit(struct enemy *e)
{
    e->super.draw = (void (*)(struct sprite*))enemyDraw;
    e->super.update = (void (*)(struct sprite*))enemyUpdate;
    e->hited = (void (*)(struct enemy*))enemyHited;

    e->enemyDownCnt = 0;
    e->status = enemy_normal;

    //  v
    int m = 1;
    int n = 3;
    e->v = rand() % (n - m + 1) + m;
}