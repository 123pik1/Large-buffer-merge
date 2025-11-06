#ifndef NATURALMERGE_HPP
#define NATURALMERGE_HPP

#include "Number.hpp"
#include <string>
#include <vector>

class NaturalMerge
{
public:
    NaturalMerge(const std::string &inputFile, const std::string &outputFile);
    void sort();

private:
    std::string inputFile;
    std::string outputFile;

    std::vector<Number> loadInput() const;
    void writeOutput(const std::vector<Number> &numbers) const;
    bool naturalMergePass(std::vector<Number> &data) const;
};

#endif // NATURALMERGE_HPP