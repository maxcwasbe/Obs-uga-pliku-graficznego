#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

struct Color {
    int r, g, b;

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

namespace std {
    template <>
    struct hash<Color> {
        size_t operator()(const Color& c) const {
            return hash<int>()(c.r) ^ hash<int>()(c.g) ^ hash<int>()(c.b);
        }
    };
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::tuple<int, int, std::vector<Color>> read_image(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyc pliku " << file_name << std::endl;
        return {0, 0, {}};
    }

    std::string format;
    std::getline(file, format);
    if (format != "P1" && format != "P2" && format != "P3") {
        std::cerr << "Nieobslugiwany format pliku." << std::endl;
        return {0, 0, {}};
    }

    std::string line;
    std::getline(file, line);

    int width, height;
    file >> width >> height;

    int max_val;
    file >> max_val;

    std::vector<Color> colors;
    if (format == "P1") {
        int pixel;
        while (file >> pixel) {
            colors.push_back({pixel, pixel, pixel});
        }
    } else {
        int r, g, b;
        while (file >> r >> g >> b) {
            colors.push_back({r, g, b});
        }
    }

    return {width, height, colors};
}

int main() {
    std::string file_name;
    char choice = '\0';

    do {
        std::cout << "Podaj nazwe pliku: ";
        std::cin >> file_name;

        int width, height;
        std::vector<Color> colors;
        std::tie(width, height, colors) = read_image(file_name);

        if (width == 0 || height == 0) {
            continue;
        }

        std::cout << "\nSzerokosc obrazu: " << width << std::endl;
        std::cout << "Wysokosc obrazu: " << height << std::endl;

        std::unordered_map<Color, int> color_counts;
        for (const auto& color : colors) {
            color_counts[color]++;
        }

        auto most_common_color = std::max_element(color_counts.begin(), color_counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });

        std::cout << "Najczęsciej występujący kolor to " << most_common_color->first.r << "-"
                  << most_common_color->first.g << "-" << most_common_color->first.b
                  << " i wystąpił " << most_common_color->second << " razy" << std::endl;

        std::cout << "Liczba unikalnych kolorow: " << color_counts.size() << std::endl;

        std::cout << "\nCzyy chcesz wczytac kolejny plik (tak/nie): ";
        std::cin >> choice;

    } while (tolower(choice) == 't');

    return 0;
}

