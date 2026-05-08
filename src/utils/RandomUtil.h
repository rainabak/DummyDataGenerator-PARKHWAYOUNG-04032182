#pragma once
#include <random>
#include <vector>

class RandomUtil
{
public:
    RandomUtil();

    int    nextInt(int min, int max);
    double nextDouble(double min, double max);

    template <typename T>
    const T& pickOne(const std::vector<T>& pool)
    {
        int idx = nextInt(0, static_cast<int>(pool.size()) - 1);
        return pool[idx];
    }

private:
    std::mt19937 m_engine;
};
