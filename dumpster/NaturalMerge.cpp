#include "NaturalMerge.hpp"
#include <fstream>
#include <stdexcept>

NaturalMerge::NaturalMerge(const std::string &inputFile, const std::string &outputFile)
    : inputFile(inputFile), outputFile(outputFile)
{
}

std::vector<Number> NaturalMerge::loadInput() const
{
    std::ifstream in(inputFile);
    if (!in)
        throw std::runtime_error("Cannot open input file: " + inputFile);

    std::vector<Number> numbers;
    std::string token;
    while (in >> token)
        numbers.emplace_back(token);
    return numbers;
}

void NaturalMerge::writeOutput(const std::vector<Number> &numbers) const
{
    std::ofstream out(outputFile, std::ios::trunc);
    if (!out)
        throw std::runtime_error("Cannot open output file: " + outputFile);

    for (const auto &num : numbers)
        out << num.to_string() << '\n';
}

bool NaturalMerge::naturalMergePass(std::vector<Number> &data) const
{
    if (data.size() <= 1)
        return false;

    std::vector<Number> merged;
    merged.reserve(data.size());

    const std::size_t n = data.size();
    std::size_t i = 0;
    bool anyMerge = false;

    while (i < n)
    {
        const std::size_t start1 = i;
        while (i + 1 < n && !(data[i + 1] < data[i]))
            ++i;
        const std::size_t end1 = i++;

        if (i >= n)
        {
            merged.insert(merged.end(), data.begin() + start1, data.begin() + end1 + 1);
            break;
        }

        const std::size_t start2 = i;
        while (i + 1 < n && !(data[i + 1] < data[i]))
            ++i;
        const std::size_t end2 = i++;

        anyMerge = true;

        std::size_t idx1 = start1;
        std::size_t idx2 = start2;
        while (idx1 <= end1 && idx2 <= end2)
        {
            if (data[idx2] < data[idx1])
                merged.push_back(data[idx2++]);
            else
                merged.push_back(data[idx1++]);
        }
        while (idx1 <= end1)
            merged.push_back(data[idx1++]);
        while (idx2 <= end2)
            merged.push_back(data[idx2++]);
    }

    if (!anyMerge)
        return false;

    data.swap(merged);
    return true;
}

void NaturalMerge::sort()
{
    auto numbers = loadInput();
    if (numbers.size() <= 1)
    {
        writeOutput(numbers);
        return;
    }

    while (naturalMergePass(numbers))
        continue;

    writeOutput(numbers);
}