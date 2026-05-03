#pragma once
#include <string>
#include <iostream>


template<typename ContainerType>
void Print(const ContainerType& a, const std::string& prefix = "")
{
    if (!prefix.empty())
    {
        std::cout << "\t" << prefix << std::endl;
    }

    std::cout << "\t";
    for (auto e : a)
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
