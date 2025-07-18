#include "wave_reader.hpp"

WaveReader::WaveReader(const std::string& filename, bool& enable, std::mutex& mtx)
    : filename_(filename) {
    // Initialize and read file
}

WaveHeader WaveReader::GetHeader() {
    return {44100, 16, 2}; // Dummy data for example
}

std::string WaveReader::GetFilename() const {
    return filename_;
}
