#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

#include "wave_reader.hpp"

class Transmitter {
public:
    Transmitter();
    ~Transmitter();

    void Transmit(WaveReader& reader, float frequency, float bandwidth, uint16_t dmaChannel, bool continuePlaying);
    void Stop();
};

#endif // TRANSMITTER_HPP
