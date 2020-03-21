#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start; //���D���I
    int length; //���D����
    int end; //���D���I
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
    //Ū�J���|
    for (i = 0; i < node; i++)
    {
        scanf ("%d", &old_route[i]);
        output[round][i] = old_route[i];
    }
    round++;
    for (i = 0; i < node; i++)
        scanf ("%d", &final_route[i]);

//step 1:��s���I

    //�������I��u
    white_real[0] = 0; //�����I��u�����|,s = 0
    int white = 1; //���I��u�Ӽ�,s�������I
    //���¸��|
    for (i = 0; i < node-1;)
    {
        i = old_route[i];
        //�P�_���I�b�s���|���S�����L,���h����
        if (final_route[i] != -1 || i == node-1)
        {
            white_real[white] = i;
            white++;
        }
    }

    //�O�����I��u
    int *white_dash = (int*)malloc(sizeof(int)*node);
    white_dash[0] = 0; //�����I��u�����|,s = 0
    white = 1; //���I��u�Ӽ�,s�������I
    int is_red_update = 0; //�P�_���S�����I�Q��s
    //���s���|
    for (i = 0; i < node-1;)
    {
        i = final_route[i];
//        printf ("")
        //�P�_���I�b�¸��|���S�����L,�L:��s�¸��|,��:����
        if (old_route[i] == -1 && i != node-1)
        {
            old_route[i] = final_route[i];
            is_red_update = 1; //�����I�Q��s
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

//step 2:���̻����I�X��

    int white2 = white;
    int j = 0, k = 0;
    while (1)
    {
        table test_update[white2-1]; //�b���|�W��i���s���I
        table to_update[white2-1]; //�n��s���I
        int position = 0; //�O���I����m

        for (i = 0; i < white2-1; i++) //�����I��u(�s)
        {
            for (j = 0; j < white2; j++) //�����I��u(��)
            {
                //����D���I,����,������X��U�@��
                if (white_dash[i+1] == white_real[j])
                {
                    test_update[i].start = position;
                    test_update[i].length = j - position;
                    test_update[i].end = i;
                    break;
                }
            }
            position = j; // �U�@�Ӹ��D���_�I
        }

        //����D�Z���̻�,index�̤p,�B���Ĭ�
        qsort (test_update, white2-1, sizeof(table), compare);
        int num_to_update = 1; //�n��s���Ӽ�
        int is_to_update  = 1; //�i���i�Q��s
        to_update[0] = test_update[0];
        for (i = 1; i < white2-1; i++)
        {
            //���L���^����
            if (test_update[i].length < 0)
                continue;
            is_to_update = 1;
            //�Ĭ�h����s
            for(j = 0; j < num_to_update; j++)
                if ((test_update[i].start > to_update[j].start &&
                     test_update[i].start < (to_update[j].start + to_update[j].length)) ||
                    ((test_update[i].start + test_update[i].length) > to_update[j].start &&
                      (test_update[i].start + test_update[i].length) < (to_update[j].start + to_update[j].length)))
                {
                    is_to_update = 0;
                    break;
                }
            if (is_to_update == 1) //�|�Q��s
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
            old_route[a] = final_route[a]; //��s
            white_real[to_update[i].start] = -2; //�аO�w�Q��s
            white_dash[to_update[i].end] = -2;
        }
        for (i = 0; i < node; i++)
            output[round][i] = old_route[i];
        round++;

        int b;
        int is_update = 0;
        for (i = 0; i < num_to_update; i++)
        {
            //��s���D�������I
            for (j = to_update[i].start+1; j < (to_update[i].start+to_update[i].length); j++)
            {
                is_update = 1;
                b = white_real[j];
                old_route[b] = final_route[b]; //��s
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
        //�p��s�����I���|��node��
        for (i = 0; i < white; i++)
            if (white_real[i] != -2)
                white2++;
        if (white2 == 1)
            break;

        int *tmp_old = white_real, *tmp_new = white_dash;
        white_real = (int*)malloc(sizeof(int)*white2);
        white_dash = (int*)malloc(sizeof(int)*white2);

        //��J�٨S��s���I
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

//step 3:��s�ѤU���I
    int is_update2 = 0;
    for (i = 0; i < node; i++)
        if (final_route[i] == -1)
        {
            is_update2 = 1;
            old_route[i] = final_route[i]; //��s
        }

    if (is_update2 == 1)
    {
        for (i = 0; i < node; i++)
            output[round][i]=old_route[i];
        round++;
    }
//��X
    printf ("%d\n", round);
    for(i = 0; i < round; i++)
    {
        for(j = 0; j < node-1; j++)
            printf("%d ",output[i][j]);
        printf("%d\n",output[i][j]);
    }

    return 0;
}
