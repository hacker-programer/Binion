#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <string>

struct Binion {
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

    Structure readStructure(const std::string& filename,
                            const std::vector<std::function<bool(const char*, size_t)>>& getEndFuncs) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo para lectura.\n";
            return {};
        }

        Structure s;

        for (const auto& func : getEndFuncs) {
            std::vector<char> buffer;
            char c;
            while (file.get(c)) {
                buffer.push_back(c);
                if (func(buffer.data(), buffer.size())) {
                    break;
                }
            }
            s.addFormat(Format(buffer.data(), buffer.size(), func));
        }

        file.close();
        return s;
    }
};
