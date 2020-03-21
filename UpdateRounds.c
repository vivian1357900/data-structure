#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start; //跳躍原點
    int length; //跳躍長度
    int end; //跳躍終點
}table;

int compare(const void *a, const void *b)
{
    table c = *(table *)a;
    table d = *(table *)b;
    if (c.length == d.length)
        return c.end - d.end;
    return d.length - c.length;
}

int main()
{
    int node = 0, i;
    scanf ("%d", &node);
    int old_route[node], final_route[node];
    int *white_real = (int*)malloc(sizeof(int)*node);
    int output[node][node];
    int round = 0;
    //讀入路徑
    for (i = 0; i < node; i++)
    {
        scanf ("%d", &old_route[i]);
        output[round][i] = old_route[i];
    }
    round++;
    for (i = 0; i < node; i++)
        scanf ("%d", &final_route[i]);

//step 1:更新紅點

    //紀錄白點實線
    white_real[0] = 0; //走白點實線的路徑,s = 0
    int white = 1; //白點實線個數,s必為白點
    //走舊路徑
    for (i = 0; i < node-1;)
    {
        i = old_route[i];
        //判斷該點在新路徑有沒有走過,有則紀錄
        if (final_route[i] != -1 || i == node-1)
        {
            white_real[white] = i;
            white++;
        }
    }

    //記錄白點虛線
    int *white_dash = (int*)malloc(sizeof(int)*node);
    white_dash[0] = 0; //走白點虛線的路徑,s = 0
    white = 1; //白點虛線個數,s必為白點
    int is_red_update = 0; //判斷有沒有紅點被更新
    //走新路徑
    for (i = 0; i < node-1;)
    {
        i = final_route[i];
//        printf ("")
        //判斷該點在舊路徑有沒有走過,無:更新舊路徑,有:紀錄
        if (old_route[i] == -1 && i != node-1)
        {
            old_route[i] = final_route[i];
            is_red_update = 1; //有紅點被更新
        }
        else
        {
            white_dash[white] = i;
            white++;
        }
    }
    if (is_red_update == 1)
    {
        for (i = 0; i < node; i++)
            output[round][i] = old_route[i];
        round++;
    }

//step 2:跳最遠的點合併

    int white2 = white;
    int j = 0, k = 0;
    while (1)
    {
        table test_update[white2-1]; //在路徑上選可能更新的點
        table to_update[white2-1]; //要更新的點
        int position = 0; //記錄點的位置

        for (i = 0; i < white2-1; i++) //走白點虛線(新)
        {
            for (j = 0; j < white2; j++) //走白點實線(舊)
            {
                //找跳躍終點,紀錄,找到後跳出找下一個
                if (white_dash[i+1] == white_real[j])
                {
                    test_update[i].start = position;
                    test_update[i].length = j - position;
                    test_update[i].end = i;
                    break;
                }
            }
            position = j; // 下一個跳躍的起點
        }

        //找跳躍距離最遠,index最小,且不衝突的
        qsort (test_update, white2-1, sizeof(table), compare);
        int num_to_update = 1; //要更新的個數
        int is_to_update  = 1; //可不可被更新
        to_update[0] = test_update[0];
        for (i = 1; i < white2-1; i++)
        {
            //跳過往回走的
            if (test_update[i].length < 0)
                continue;
            is_to_update = 1;
            //衝突則不更新
            for(j = 0; j < num_to_update; j++)
                if ((test_update[i].start > to_update[j].start &&
                     test_update[i].start < (to_update[j].start + to_update[j].length)) ||
                    ((test_update[i].start + test_update[i].length) > to_update[j].start &&
                      (test_update[i].start + test_update[i].length) < (to_update[j].start + to_update[j].length)))
                {
                    is_to_update = 0;
                    break;
                }
            if (is_to_update == 1) //會被更新
            {
                to_update[num_to_update] = test_update[i];
                num_to_update++;
            }
        }

        //merge
        int a;
        for (i = 0; i < num_to_update; i++)
        {
            a = white_real[to_update[i].start];
            old_route[a] = final_route[a]; //更新
            white_real[to_update[i].start] = -2; //標記已被更新
            white_dash[to_update[i].end] = -2;
        }
        for (i = 0; i < node; i++)
            output[round][i] = old_route[i];
        round++;

        int b;
        int is_update = 0;
        for (i = 0; i < num_to_update; i++)
        {
            //更新跳躍中間的點
            for (j = to_update[i].start+1; j < (to_update[i].start+to_update[i].length); j++)
            {
                is_update = 1;
                b = white_real[j];
                old_route[b] = final_route[b]; //更新
                for (k = 0; k < white2-1; k++)
                    if (white_real[j] == white_dash[k])
                    {
                        white_dash[k] = -2;
                        break;
                    }
                white_real[j] = -2;
            }
        }

        if (is_update == 1)
        {
            for (i = 0; i < node; i++)
                output[round][i] = old_route[i];
            round++;
        }

        white = white2;
        white2 = 0;
        //計算新的白點路徑的node數
        for (i = 0; i < white; i++)
            if (white_real[i] != -2)
                white2++;
        if (white2 == 1)
            break;

        int *tmp_old = white_real, *tmp_new = white_dash;
        white_real = (int*)malloc(sizeof(int)*white2);
        white_dash = (int*)malloc(sizeof(int)*white2);

        //填入還沒更新的點
        int o = 0, n = 0;
        for (i = 0; i < white; i++)
        {
            if (tmp_old[i] != -2)
            {
                white_real[o]=tmp_old[i];
                o++;
            }
            if(tmp_new[i] != -2)
            {
                white_dash[n] = tmp_new[i];
                n++;
            }
            free (tmp_old);
            free (tmp_new);
        }
    }

//step 3:更新剩下的點
    int is_update2 = 0;
    for (i = 0; i < node; i++)
        if (final_route[i] == -1)
        {
            is_update2 = 1;
            old_route[i] = final_route[i]; //更新
        }

    if (is_update2 == 1)
    {
        for (i = 0; i < node; i++)
            output[round][i]=old_route[i];
        round++;
    }
//輸出
    printf ("%d\n", round);
    for(i = 0; i < round; i++)
    {
        for(j = 0; j < node-1; j++)
            printf("%d ",output[i][j]);
        printf("%d\n",output[i][j]);
    }

    return 0;
}
