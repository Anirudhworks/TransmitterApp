#ifndef WAVE_READER_HPP
#define WAVE_READER_HPP

#include <string>
#include <mutex>

struct WaveHeader {
    uint32_t sampleRate;
    uint16_t bitsPerSample;
    uint16_t channels;
};

class WaveReader {
public:
    WaveReader(const std::string& filename, bool& enable, std::mutex& mtx);
    WaveHeader GetHeader();
    std::string GetFilename() const;

private:
    std::string filename_;
};

#endif // WAVE_READER_HPP
