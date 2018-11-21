#include "CommonHeader.hpp"
#include <sstream>

static void GenerateTape(const std::string &path, int size) {
    std::fstream newFile;
    newFile.open(path, std::ios::out | std::ios::binary);
    std::default_random_engine gen(
        static_cast<unsigned int>(clock())); // clock()
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                DIST_UPPER_LIMIT);
    std::uniform_int_distribution<int> distInt(RECORD_MIN_SIZE,
                                               RECORD_MAX_SIZE);
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

static int InputFromConsole(std::string path) {
    std::fstream file;
    std::remove(path.c_str());
    int size = 0;
    file.open(path, std::ios::out | std::ios::binary | std::ios::app);
    std::cout << "======== INSERTING DATA FROM CONSOLE ========" << std::endl;
    while (true) {
        std::cin.clear();
        std::vector<double> data;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        double input;
        while (iss >> input) {
            data.push_back(input);
        }
        if (data.empty())
            break;
        data.push_back(SEPARATOR_VALUE);
        std::cout << "RECORD ADDED: ";
        for (auto item : data)
            if (!std::isnan(item))
                std::cout << item << ' ';
        std::cout << std::endl;
        size++;
        double *toWrite = &data[0];
        file.write(reinterpret_cast<char *>(toWrite),
                   data.size() * sizeof(double));
        file.flush();
    }
    return size;
}