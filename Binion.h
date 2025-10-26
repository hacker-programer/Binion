#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <string>

struct Binion {
    Binion() = default;
    struct Format {
        std::vector<char> data;
        std::function<bool(const char*, size_t)> getEnd;

        Format(const char* inputData, size_t inputSize, std::function<bool(const char*, size_t)> func)
            : data(inputData, inputData + inputSize), getEnd(func) {}
    };

    struct Structure {
        std::vector<Format> formats;

        void addFormat(const Format& fmt) {
            formats.push_back(fmt);
        }
    };

    void writeStructure(const std::string& filename, const Structure& s) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo para escritura.\n";
            return;
        }

        for (const auto& fmt : s.formats) {
            size_t written = 0;
            for (size_t i = 1; i <= fmt.data.size(); ++i) {
                if (fmt.getEnd(fmt.data.data(), i)) {
                    file.write(fmt.data.data(), i);
                    written = i;
                    break;
                }
            }
            if (written == 0) {
                file.write(fmt.data.data(), fmt.data.size());
            }
        }

        file.close();
    }

    void readStructure(const std::string& filename, Binion::Structure& s) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo para lectura.\n";
            return;
        }

        for (auto& fmt : s.formats) {
            fmt.data.clear(); // asegurarnos de empezar vacío
            char c;
            while (file.get(c)) {
                fmt.data.push_back(c);
                if (fmt.getEnd(fmt.data.data(), fmt.data.size())) {
                    break;
                }
            }
        }

        file.close();
    }
};
