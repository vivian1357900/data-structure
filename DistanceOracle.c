#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int choose(int distance[], short int found[]);
void  shortest_path (int v, int distance[]);

int num_node, num_link, prime;
int cost[1010][1010]; // = adj_matrix
int size; // table�j�p

struct HashNode
{
    int node1, node2, dist;
    struct HashNode* next;
};
struct HashNode **table;          // hash table

int hash_function(int node1, int node2)
{
    int idx;
    idx = ((node1+1)*(node2+1))%prime%size;
    return idx;
}

// �إ߷s���쵲�I
struct HashNode* create_node(int node1, int node2, int dist)
{
    struct HashNode* newNode = malloc(sizeof(struct HashNode));
    newNode->node1 = node1;
    newNode->node2 = node2;
    newNode->dist = dist;
    newNode->next = NULL;
    return newNode;
};

// �إߤ@�ӪŪ�hash table
void hash_new()
{
    table = malloc(sizeof(struct HashNode*) * size);
    for(int i = 0; i < size; i++)
        table[i] = NULL;
}

// �N���I�[��table��
void hash_insert(int node1, int node2, int dist)
{
    int index = hash_function(node1, node2);   // �p���x�s��m
    struct HashNode *newNode = create_node(node1, node2, dist); // �}�s�`�I�Ӧs���
    newNode->next = table[index]; // �������index
    table[index] = newNode; // �[�Jlist
}

//
int hash_search(int node1, int node2)
{
    int index = hash_function(node1, node2);
    struct HashNode *current = table[index]; // ���������ޭȪ�list
    while (current) // �blist���M��
    {
        if ((current->node1 == node1 && current->node2 == node2)||
            (current->node1 == node2 && current->node2 == node1))
            return current->dist;
        current = current->next;
    }
    return -1;
}

int main()
{
    int id, node1, node2, cost_value, i, j;
    int count = 0;
    j = scanf("%d %d %d", &num_node, &num_link, &prime);
    // Ū�Jcost�æs�Jadj_matrix��
    for (i = 0; i < num_link; i++)
    {
        j = scanf("%d %d %d %d", &id, &node1, &node2, &cost_value);
        cost[node1][node2] = cost_value;
        cost[node2][node1] = cost_value;
    }
    int num_pair;
    j = scanf("%d", &num_pair);
    int pair[num_pair][2];
    for (i = 0; i < num_pair; i++)
        j = scanf ("%d %d %d", &id, &pair[i][0], &pair[i][1]);

    // �p��hash table���j�p
    float a;
    a = pow(num_node, 3);
    a = sqrt(a);
    size = ceil(a);
    hash_new();
    // �p��ݭn��sample node�Ӽ�
    int num_sample, t = 0;
    float tmp;
    tmp = sqrt(num_node);
    num_sample = ceil(tmp);
    int sample_pos[num_sample];
    sample_pos[t] = 0;
    int distance_min[num_node], distance[num_sample][num_node]; // shortest path
    int is_fill[1010][1010] = {{0}}; // �P�_�O�_�w��Jhash table

    // �Ĥ@��sample node: A
    int max = -1;
    shortest_path(sample_pos[t], distance[t]);
    //�ĤG��sample node
    for (i = 1; i < num_node; i++)
    {
        is_fill[sample_pos[t]][i] = 1; // ��Jhash table
        is_fill[i][sample_pos[t]] = 1;
        hash_insert (sample_pos[t], i, distance[t][i]);
        distance_min[i] = distance[t][i];
        max = MAX(max, distance[t][i]); // ��̻��Z��
        count++;

//        int q = hash_search(sample_pos[t], i);
//        printf ("(1)%d %d %d\n", sample_pos[t], i, q);
    }
    distance_min[0] = distance[t][0];

    // ��U�@��sample�I
    for (i = 0; i < num_node; i++)
        if (distance[t][i] == max)
        {
            t++;
            sample_pos[t] = i;
            break;
        }

    while (t < num_sample)
    {
        int max = -1;
        shortest_path(sample_pos[t], distance[t]);
        for (i = 0; i < num_node; i++)
        {
            // �p�G�٨S��Jhash table�N��J
            if (sample_pos[t] != i &&
                is_fill[sample_pos[t]][i] == 0 &&
                is_fill[i][sample_pos[t]] == 0)
            {
                is_fill[sample_pos[t]][i] = 1;
                is_fill[i][sample_pos[t]] = 1;
                hash_insert (sample_pos[t], i, distance[t][i]);
                count++;
//                int q = hash_search(sample_pos[t], i);
//                printf ("(2)%d %d %d\n", sample_pos[t], i, q);
            }
            distance_min[i] = MIN(distance_min[i], distance[t][i]); // �U�I���̵u�Z��

        }

        for (i = 0; i < num_node; i++)
            max = MAX(max, distance_min[i]); // ��̵u�Z�����̻���
        for (i = 0; i < num_node; i++)
            if (distance_min[i] == max)
            {
                t++;
                sample_pos[t] = i; // �s��sample�I
                break;
            }
    }

    // ����u�Z������Jtable
    for (i = 0; i < num_node; i++)
        for (j = 0; j < num_node; j++)
            if (is_fill[i][j] == 0 &&
                is_fill[j][i] == 0 &&
                cost[i][j] < distance_min[i] && cost[i][j] != 0 &&
                cost[i][j] < distance_min[j] && cost[i][j] != 0)
                {
                    hash_insert(i, j, cost[i][j]);
                    is_fill[i][j] = 1;
                    is_fill[j][i] = 1;
                    count++;
//                    int q = hash_search(i, j);
//                    printf ("(3)%d %d %d\n", i, j, q);
                }


    //�d��ÿ�X
    int output;
    printf ("%d\n", count);
    for (i = 0; i < num_pair; i++)
    {
        output = hash_search(pair[i][0], pair[i][1]); // �btable���j�M
        if (output == -1) // �p�G���btable��
        {
            int min = 2*1e6, a, b;
            // ��g�Lsample node���̵u�Z��
            for (j = 0; j < num_sample; j++)
            {
                a = hash_search(pair[i][0], sample_pos[j]);
                b = hash_search(pair[i][1], sample_pos[j]);
                min = MIN(min, a+b);

//                printf("a = %d %d %d\n", pair[i][0], sample_pos[j], a);
//                printf("b = %d %d %d\n", pair[i][1], sample_pos[j], b);
            }
            printf("%d %d %d %d\n", i, pair[i][0], pair[i][1], min);
        }
        else // �btable�����ܪ�����X
        {
            printf("%d %d %d %d\n", i, pair[i][0], pair[i][1], output);
        }
    }

    return 0;
}

// Dijkstra�t��k
void  shortest_path (int v, int distance[])
{
    int i, u, w;
    short int found[num_node]; // shortest path�O�_�Q���
    for (i=0; i<num_node; i++)
    {
        found[i] = 0;
        distance[i] = 2*1e6;
        if (cost[v][i])
            distance[i] = cost[v][i];
    }

    found[v] = 1;
    distance[v] = 0;
    for (i = 0; i < num_node-2; i++)
    {
        u = choose(distance, found);
        found[u] = 1;
        for (w = 0; w < num_node; w++)
        {
            if (!found[w] && cost[u][w])
                if (distance[u]+cost[u][w] < distance[w])
                    distance[w] = distance[u]+cost[u][w];
        }

    }
}

// ��X�٨S�T�{�̵u�Z�����I
int choose(int distance[], short int found[])
{
    int i, min, minpos;
    min = 2*1e6;
    minpos = -1;
    for (i = 0; i < num_node; i ++)
        if (distance[i] < min && !found[i])
        {
            min = distance[i];
            minpos = i;
        }
  return minpos;
}




/*
7 10 3
0 0 1 3
1 0 3 12
2 0 6 4
3 1 2 7
4 1 4 6
5 2 3 5
6 2 6 10
7 3 4 4
8 4 5 7
9 5 6 1
3
0 1 4
1 6 5
2 1 5
*/






