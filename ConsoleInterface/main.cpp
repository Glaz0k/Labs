#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cerr << "Required filename argument\n";
        return 1;
    }
    std::fstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "File cannot be opened\n";
        return 2;
    }

    inputFile.close();
    return 0;
}