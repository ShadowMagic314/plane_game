#include <easyx.h>
#include "gameloop.h"
#include "scene.h"

void gameLoop(struct scene *s, int fps)
{
    timeBeginPeriod(1);
    //  ��ʼʱ�䡢����ʱ�䡢Ƶ��F
    LARGE_INTEGER startCount, endCount, F;
    //  ��ȡƵ��F
    QueryPerformanceFrequency(&F);

    //  ѭ������
    bool isLoopOver = false;

    BeginBatchDraw();
    while (1)
    {
        //  ��ȡ��ʼ����
        QueryPerformanceCounter(&startCount);

        //  ��մ���
        cleardevice();
        
        //  1. ���ƻ���
        s->draw(s);

        //  2. ��������
        s->update(s);

        //  3.  �Ƿ����
        if (s->isQuit(s))
            break;

        //  ��ȡ��������
        QueryPerformanceCounter(&endCount);

        //  ����ʱ��
        long long elapse = (endCount.QuadPart - startCount.QuadPart)
            / F.QuadPart * 1000000;

        //  ע������ĵ�λ��΢��
        while (elapse < 1000000 / fps)
        {
            Sleep(1);
            
            //  4. ������Ϣ
            ExMessage msg;
            bool isOk = peekmessage(&msg, EX_MOUSE);
            if (isOk == true)
            {
                s->control(s, &msg);
            }

            //  ���»�ȡ����ʱ��
            QueryPerformanceCounter(&endCount);
            //  ����ʱ��
            elapse = (endCount.QuadPart - startCount.QuadPart)
                * 1000000 / F.QuadPart;
        }

        //  ���ƻ���
        FlushBatchDraw();

        //  ����ѭ��
        if (isLoopOver == true)
            break;
    }
    EndBatchDraw();
    timeEndPeriod(1);
}