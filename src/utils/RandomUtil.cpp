#include "RandomUtil.h"
#include <random>

RandomUtil::RandomUtil()
    : m_engine(std::random_device{}())
{
}

int RandomUtil::nextInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(m_engine);
}

double RandomUtil::nextDouble(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(m_engine);
}
