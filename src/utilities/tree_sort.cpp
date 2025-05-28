#include <iostream>
#include <vector>
#include <algorithm>
#include <stdarg.h>

#include "utilities/json_handler.h"

using namespace std;

// tree sort
template <typename T>
struct Nodesort
{
    GenerateBusJson::BusInfo val;
    T tosort;
};
template <typename T>
class Node
{
public:
    Nodesort<T> val;
    Node *left;
    Node *right;
    Node(GenerateBusJson::BusInfo x, T tosort) : val(), left(NULL), right(NULL)
    {
        val.val = x;
        val.tosort = tosort;
    }
    Node(Nodesort<T> x) : val(), left(NULL), right(NULL)
    {
        val = x;
    }
    ~Node()
    {
        delete left;
        delete right;
    }
};

template <typename T>
class TreeSort
{
public:
    void insert(Node<T> *root, Nodesort<T> val)
    {
        if (root->val.tosort > val.tosort)
        {
            if (root->left == NULL)
            {
                root->left = new Node(val);
            }
            else
            {
                insert(root->left, val);
            }
        }
        else
        {
            if (root->right == NULL)
            {
                root->right = new Node(val);
            }
            else
            {
                insert(root->right, val);
            }
        }
    }
    // sort
    void sort(Node<T> *root, vector<GenerateBusJson::BusInfo> &result)
    {
        if (root == nullptr)
            return;

        // Ліве піддерево
        sort(root->left, result);

        // Поточне значення
        result.push_back(root->val.val);

        // Праве піддерево
        sort(root->right, result);
    }
    void reverse_sort(Node<T> *root, vector<GenerateBusJson::BusInfo> &result)
    {
        if (root == nullptr)
            return;

        // Праве піддерево
        reverse_sort(root->right, result);

        // Поточне значення
        result.push_back(root->val.val);

        // Ліве піддерево
        reverse_sort(root->left, result);
    }
};

template <typename T>
Nodesort<T> byname(const GenerateBusJson::BusInfo &bus)
{
    Nodesort<T> nodesort;
    nodesort.val = bus;
    nodesort.tosort = bus.driver_name;
    return nodesort;
};

Nodesort<int> bytime(const GenerateBusJson::BusInfo &bus)
{
    Nodesort<int> nodesort;
    nodesort.val = bus;
    int start = 0, end = 0;
    int flag = 0;
    for (int i = 0; i < 11; i++)
    {
        if (bus.time_range[i] == '-')
        {
            flag = 1;
            continue;
        }
        if (flag == 0)
            start = start * 10 + bus.time_range[i] - '0';
        else
            end = end * 10 + bus.time_range[i] - '0';
    }

    nodesort.tosort = end - start;

    return nodesort;
}
vector<GenerateBusJson::BusInfo> sort_by_name(const vector<GenerateBusJson::BusInfo> &buses, int order)
{

    TreeSort<string> ts;
    Node<string> *root = new Node<string>(buses[0], byname<string>(buses[0]).tosort);
    size_t size = buses.size();
    for (size_t i = 1; i < size; i++)
    {
        ts.insert(root, byname<string>(buses[i]));
    }
    vector<GenerateBusJson::BusInfo> result;
    if (order == 1)
        ts.sort(root, result);
    else
        ts.reverse_sort(root, result);
    delete root;
    return result;
}
vector<GenerateBusJson::BusInfo> sort_by_time(const vector<GenerateBusJson::BusInfo> &buses, int order)
{

    TreeSort<int> ts;
    Node<int> *root = new Node<int>(buses[0], bytime(buses[0]).tosort);
    size_t size = buses.size();
    for (size_t i = 1; i < size; i++)
    {
        ts.insert(root, bytime(buses[i]));
    }
    vector<GenerateBusJson::BusInfo> result;
    if (order == 1)
        ts.sort(root, result);
    else
        ts.reverse_sort(root, result);
    delete root;
    return result;
}
vector<GenerateBusJson::BusInfo> tree_sort(const vector<GenerateBusJson::BusInfo> &buses, ...)
{
    // args are string first check it
    va_list args;
    va_start(args, buses);
    string first = va_arg(args, const char *);
    int order = va_arg(args, int);
    va_end(args);
    vector<GenerateBusJson::BusInfo> result;
    if (first == "name")
    {
        result = sort_by_name(buses, order);
    }
    else if (first == "time")
    {
        result = sort_by_time(buses, order);
    }
    // cout << "first" << first << endl;
    // TreeSort ts;
    // Node *root = new Node(buses[0], byname(buses[0]).tosort);
    // size_t size = buses.size();
    // for (size_t i = 1; i < size; i++)
    // {
    //     ts.insert(root, byname(buses[i]));
    // }
    // vector<GenerateBusJson::BusInfo> result;
    // ts.sort(root, result);
    // for (size_t i = 0; i < result.size(); i++)
    // {
    //     cout << result[i].driver_name << endl;
    // }

    // vector<GenerateBusJson::BusInfo> invresult;
    // ts.reverse_sort(root, invresult);
    // for (size_t i = 0; i < invresult.size(); i++)
    // {
    //     cout << invresult[i].driver_name << endl;
    // }
    // delete root;
    return result;
}