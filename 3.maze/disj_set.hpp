#pragma once
#include "../tinySTL/functor.hpp"
#include "../tinySTL/map.hpp"
template <typename T, typename Torder = tinySTL::less<T>> class disj_set
{
  private:
    tinySTL::map<T, T, Torder> parent;
    tinySTL::map<T, int>       rank;

    const T find(const T &x)
    {
        if (!parent.count(x)) parent[x] = x;
        return x == parent[x] ? x : (parent[x] = find(parent[x]));
    }

  public:
    disj_set() {}
    ~disj_set() {}

    void join(const T &x1, const T &x2)
    {
        T f1 = find(x1);
        T f2 = find(x2);
        if (rank[f1] > rank[f2])
            parent[f2] = f1;
        else {
            parent[f1] = f2;
            if (rank[f1] == rank[f2]) ++rank[f2];
        }
    }
    bool is_same(const T &x1, const T &x2) { return find(x1) == find(x2); }
};