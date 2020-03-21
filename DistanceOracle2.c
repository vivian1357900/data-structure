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

// 建立新的鏈結點
struct Node* create_node(int node1, int node2, int dist)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->node1 = node1;
    newNode->node2 = node2;
    newNode->dist = dist;
    newNode->next = NULL;
    return newNode;
};

// 建立一個空的hash table
void hash_new(int n)
{
    hash_table = malloc(n*sizeof(struct Node));
    for(int i=0; i<n; i++)
        hash_table[i] = NULL;
}

// 將結點加到table中
void hash_insert(int index, int node1, int node2, int dist)
{
    struct Node* newNode = create_node(node1, node2, dist);
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
}

// 在同index中透過node1,node2尋找對應的dist
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
    return -1; // 不在hash table中
}

int main()
{
    int num_node, num_line, P;
    scanf("%d %d %d", &num_node, &num_line, &P);
    int index, i, j;
    // 讀入cost並存入adj_matrix中
    for(i = 0; i < num_line; i++)
    {
        int node1, node2, cost_value;
        scanf("%d %d %d %d", &index, &node1, &node2, &cost_value);
        cost[node1][node2] = cost_value;
        cost[node2][node1] = cost_value;
    }
    // 找每個點到每個點的shortest path
    for(i = 0; i < num_node; i++)
    {
        shortest_path(i, num_node);
        for(j = 0; j < num_node; j++)
            distence[i][j] = distence_min[j];
    }
    // 計算sample點個數
    double num_sample;
    num_sample = sqrt(num_node);
    num_sample = ceil(num_sample);
    // 從A(0)開始
    int t = 0, min, chosen[1010] = {0}, select[1010] = {0}; // 如果i被選到, chosen[i]=1
    int small_num[1010] = {0}, is_fill[num_node][num_node];
    for(i = 0; i < num_node; i++) // 初始化
        for(j = 0; j < num_node; j++)
            is_fill[i][j] = 0;
    for(i = 0; i < num_sample; i++)
    {
        select[i] = t; // 選到的sample點
        shortest_path(t, num_node); // 找sample點的shortest path
        for(j = 0; j < num_node; j++)
        {
            is_fill[t][j] = 1;
            is_fill[j][t] = 1;
            // 找最小半徑並儲存
            if(t == 0) // 一開始直接將A到各點的最短路徑存為最小半徑
            {
                min_r[j] = distence_min[j];
                small_num[j] = i;
            }
            else if(distence_min[j] < min_r[j]) // 其他有更小的再做更換
            {
                min_r[j] = distence_min[j];
                small_num[j] = i;
            }
            sample_dist[i][j] = distence_min[j]; // 存入各sample點的最短路徑
        }
        chosen[t] = 1;
        if(i == num_sample -1) break;
        // 找最遠且還沒被找過的點並儲存
        int max = -1;
        for(j = 0; j < num_node; j++)
            if(min_r[j] > max && chosen[j] != 1)
            {
                t = j;
                max = min_r[j];
            }
    }

    // 放入hash table
    double x;
    int size, count_hash = 0, done[1010] = {0};
    x = pow(num_node, 1.5); // 計算hash table大小
    size = ceil(x);
    hash_new(size);
    for(i = 0; i < num_sample; i++)
    {
        for(j = 0; j < num_node; j++)
        {
            // 如果不用放或已放過則繼續
            if(j == select[i] || done[j] == 1) continue;
            // 計算hash function
            int index = ((select[i]+1) * (j+1)) % P % size;
            hash_insert(index, select[i], j, sample_dist[i][j]);
            done[select[i]] = 1; // 標記找過
            count_hash++;
        }
    }

    // 有更短距離的也放入hash table
    for(i = 1; i < num_node; i++)
    {
        for(j = 0; j < num_node; j++)
        {
            // 如果不用放或已放過則繼續
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
    //查表並輸出
    for(int i=0; i<num_query; i++)
    {
        int node1, node2;
        scanf("%d %d %d", &index, &node1, &node2);
        int hash_f = ((node1+1) * (node2+1)) % P % size;
        int dist = hah_search(hash_f, node1, node2);
        if(dist != -1 ) // 如果在table中,直接輸出
            printf("%d %d %d %d\n", i, node1, node2, dist);
        else // 如果不在table中,找經過sample node的最短距離
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

// 找出還沒確認最短距離的點
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

// Dijkstra演算法
void shortest_path(int s, int n)
{
    int found[n], i, v; // found: shortest path是否被找到
    // 初始化
    for(i = 0; i < n; i++)
    {
        distence_min[i] = 2*1e6;
        found[i] = -1;
    }
    distence_min[s] = 0; // 到自己的距離為0
    // 找shortest path
    for(i = 0; i < n-1; i++)
    {
        int u = choose(found, n); // 找出還沒確認最短距離的點
        found[u] = 1;
        // 更新距離
        for(v = 0; v < n; v++)
        {
            // 如果還沒被找到,且s到u的距離比原本的更小的話,則更新
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
