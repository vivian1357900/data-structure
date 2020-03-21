#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int choose(int discovered[], int n);
void shortest_path(int src, int n);

int cost[1010][1010];
int distence[1010][1010];
int distence_min[1010];
int min_r[1010];
int sample_dist[1010][1010];

struct Node
{
    int node1;
    int node2;
    int dist;
    struct Node* next;
};
struct Node** hash_table;

// �إ߷s���쵲�I
struct Node* create_node(int node1, int node2, int dist)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->node1 = node1;
    newNode->node2 = node2;
    newNode->dist = dist;
    newNode->next = NULL;
    return newNode;
};

// �إߤ@�ӪŪ�hash table
void hash_new(int n)
{
    hash_table = malloc(n*sizeof(struct Node));
    for(int i=0; i<n; i++)
        hash_table[i] = NULL;
}

// �N���I�[��table��
void hash_insert(int index, int node1, int node2, int dist)
{
    struct Node* newNode = create_node(node1, node2, dist);
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
}

// �b�Pindex���z�Lnode1,node2�M�������dist
int hah_search(int index, int node1, int node2)
{
    struct Node* current = hash_table[index];
    while(current)
    {
        if((current->node1 == node1 && current->node2 == node2) ||
            (current->node1 == node2 && current->node2 == node1))
            return current->dist;
        current = current->next;
    }
    return -1; // ���bhash table��
}

int main()
{
    int num_node, num_line, P;
    scanf("%d %d %d", &num_node, &num_line, &P);
    int index, i, j;
    // Ū�Jcost�æs�Jadj_matrix��
    for(i = 0; i < num_line; i++)
    {
        int node1, node2, cost_value;
        scanf("%d %d %d %d", &index, &node1, &node2, &cost_value);
        cost[node1][node2] = cost_value;
        cost[node2][node1] = cost_value;
    }
    // ��C���I��C���I��shortest path
    for(i = 0; i < num_node; i++)
    {
        shortest_path(i, num_node);
        for(j = 0; j < num_node; j++)
            distence[i][j] = distence_min[j];
    }
    // �p��sample�I�Ӽ�
    double num_sample;
    num_sample = sqrt(num_node);
    num_sample = ceil(num_sample);
    // �qA(0)�}�l
    int t = 0, min, chosen[1010] = {0}, select[1010] = {0}; // �p�Gi�Q���, chosen[i]=1
    int small_num[1010] = {0}, is_fill[num_node][num_node];
    for(i = 0; i < num_node; i++) // ��l��
        for(j = 0; j < num_node; j++)
            is_fill[i][j] = 0;
    for(i = 0; i < num_sample; i++)
    {
        select[i] = t; // ��쪺sample�I
        shortest_path(t, num_node); // ��sample�I��shortest path
        for(j = 0; j < num_node; j++)
        {
            is_fill[t][j] = 1;
            is_fill[j][t] = 1;
            // ��̤p�b�|���x�s
            if(t == 0) // �@�}�l�����NA��U�I���̵u���|�s���̤p�b�|
            {
                min_r[j] = distence_min[j];
                small_num[j] = i;
            }
            else if(distence_min[j] < min_r[j]) // ��L����p���A����
            {
                min_r[j] = distence_min[j];
                small_num[j] = i;
            }
            sample_dist[i][j] = distence_min[j]; // �s�J�Usample�I���̵u���|
        }
        chosen[t] = 1;
        if(i == num_sample -1) break;
        // ��̻��B�٨S�Q��L���I���x�s
        int max = -1;
        for(j = 0; j < num_node; j++)
            if(min_r[j] > max && chosen[j] != 1)
            {
                t = j;
                max = min_r[j];
            }
    }

    // ��Jhash table
    double x;
    int size, count_hash = 0, done[1010] = {0};
    x = pow(num_node, 1.5); // �p��hash table�j�p
    size = ceil(x);
    hash_new(size);
    for(i = 0; i < num_sample; i++)
    {
        for(j = 0; j < num_node; j++)
        {
            // �p�G���Ω�Τw��L�h�~��
            if(j == select[i] || done[j] == 1) continue;
            // �p��hash function
            int index = ((select[i]+1) * (j+1)) % P % size;
            hash_insert(index, select[i], j, sample_dist[i][j]);
            done[select[i]] = 1; // �аO��L
            count_hash++;
        }
    }

    // ����u�Z�����]��Jhash table
    for(i = 1; i < num_node; i++)
    {
        for(j = 0; j < num_node; j++)
        {
            // �p�G���Ω�Τw��L�h�~��
            if(is_fill[j][i] == 1 || i == j) continue;
            if(distence[i][j] <= min_r[i] || distence[i][j] <= min_r[j])
            {
                int index = ((i+1) * (j+1)) % P % size;
                hash_insert(index, i, j, distence[i][j]);
                is_fill[i][j] = 1;
                count_hash++;
            }
        }
    }
    printf("%d\n", count_hash);
    int num_query;
    scanf("%d", &num_query);
    //�d��ÿ�X
    for(int i=0; i<num_query; i++)
    {
        int node1, node2;
        scanf("%d %d %d", &index, &node1, &node2);
        int hash_f = ((node1+1) * (node2+1)) % P % size;
        int dist = hah_search(hash_f, node1, node2);
        if(dist != -1 ) // �p�G�btable��,������X
            printf("%d %d %d %d\n", i, node1, node2, dist);
        else // �p�G���btable��,��g�Lsample node���̵u�Z��
        {

            int dist1 = min_r[node1];
            dist1 = dist1 + sample_dist[small_num[node1]][node2];
            int dist2 = min_r[node2];
            dist2 = dist2 + sample_dist[small_num[node2]][node1];
            if(dist1 < dist2)
                printf("%d %d %d %d\n", i, node1, node2, dist1);
            else
                printf("%d %d %d %d\n", i, node1, node2, dist2);
        }
    }

    return 0;
}

// ��X�٨S�T�{�̵u�Z�����I
int choose(int found[], int n)
{
    int min = 2*1e6, min_index, i;
    for(i = 0; i < n; i++)
    {
        if(found[i] == -1 && distence_min[i] <= min)
        {
            min = distence_min[i];
            min_index = i;
        }
    }
    return min_index;
}

// Dijkstra�t��k
void shortest_path(int s, int n)
{
    int found[n], i, v; // found: shortest path�O�_�Q���
    // ��l��
    for(i = 0; i < n; i++)
    {
        distence_min[i] = 2*1e6;
        found[i] = -1;
    }
    distence_min[s] = 0; // ��ۤv���Z����0
    // ��shortest path
    for(i = 0; i < n-1; i++)
    {
        int u = choose(found, n); // ��X�٨S�T�{�̵u�Z�����I
        found[u] = 1;
        // ��s�Z��
        for(v = 0; v < n; v++)
        {
            // �p�G�٨S�Q���,�Bs��u���Z����쥻����p����,�h��s
            if(found[v] != 1 && cost[u][v] && distence_min[u] != 2*1e6 &&
               (distence_min[u] + cost[u][v] < distence_min[v]))
               distence_min[v] = distence_min[u] + cost[u][v];
        }
    }
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
