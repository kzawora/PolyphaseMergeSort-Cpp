#include "CommonHeader.hpp"

static void GenerateTape(std::string path, int size) {
    std::fstream newFile;
    newFile.open(path, std::ios::out | std::ios::binary);
    std::default_random_engine gen(clock()); // clock()
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                DIST_UPPER_LIMIT);
    std::uniform_int_distribution<int> distInt(1, 1);
    auto generator = [&]() { return dist(gen); };
    auto generatorInt = [&]() { return distInt(gen); };
    for (int i = 0; i < size; i++) {
        int setSize = generatorInt();
        while (setSize--) {
            double num = generator();
            newFile.write(reinterpret_cast<char *>(&num), sizeof(double));
        }
        double separator = SEPARATOR_VALUE;
        newFile.write(reinterpret_cast<char *>(&separator), sizeof(double));
    }
    newFile.close();
}

static void IsSorted(std::string filename) {
    std::cout << std::endl << "======== SORT CHECK ========" << std::endl;
    std::cout << "==== CHECKING: " << filename << " ====" << std::endl;
    auto tape =
        std::make_shared<Tape>(filename, std::ios::in | std::ios::binary);
#if PRINT_TAPES == 1
    std::cout << *tape << std::endl;
#endif
    long long counter = 1;
    bool isSorted = true;
    auto prev = tape->GetNext();
    while (tape->HasNext()) {
        counter++;
        auto next = tape->GetNext();
        if (prev > next) {
            isSorted = false;
            break;
        }
        prev = next;
    }
    if (!isSorted) {
        std::cout << "NOT SORTED!" << std::endl;
    } else
        std::cout << "SORTED! " << counter << " RECORDS TOTAL" << std::endl;
}