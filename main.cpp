#include "transmitter.hpp"
#include "wave_reader.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <mutex>

std::mutex mtx;
bool enable = true;
Transmitter* transmitter = nullptr;

void sigIntHandler(int sigNum) {
    if (transmitter) {
        std::cout << "Stopping..." << std::endl;
        transmitter->Stop();
        enable = false;
    }
}

int main(int argc, char** argv) {
    float frequency = 100.f, bandwidth = 200.f;
    uint16_t dmaChannel = 0;
    bool showUsage = true, loop = false;
    int opt, filesOffset;

    while ((opt = getopt(argc, argv, "rf:d:b:v")) != -1) {
        switch (opt) {
            case 'r': loop = true; break;
            case 'f': frequency = std::stof(optarg); break;
            case 'd': dmaChannel = std::stoi(optarg); break;
            case 'b': bandwidth = std::stof(optarg); break;
            case 'v':
                std::cout << "TransmitterApp version: 1.0" << std::endl;
                return 0;
        }
    }

    if (optind < argc) {
        filesOffset = optind;
        showUsage = false;
    }

    if (showUsage) {
        std::cout << "Usage: TransmitterApp [-f <frequency>] [-b <bandwidth>] [-d <dma_channel>] [-r] <file>" << std::endl;
        return 0;
    }

    std::signal(SIGINT, sigIntHandler);
    std::signal(SIGTERM, sigIntHandler);

    int result = EXIT_SUCCESS;

    try {
        transmitter = new Transmitter();
        std::cout << "Broadcasting at " << frequency << " MHz with "
                  << bandwidth << " kHz bandwidth" << std::endl;

        do {
            std::string filename = argv[optind++];
            if ((optind == argc) && loop) optind = filesOffset;

            WaveReader reader(filename != "-" ? filename : std::string(), enable, mtx);
            WaveHeader header = reader.GetHeader();

            std::cout << "Playing: " << reader.GetFilename() << ", "
                      << header.sampleRate << " Hz, "
                      << header.bitsPerSample << " bits, "
                      << ((header.channels > 1) ? "stereo" : "mono") << std::endl;

            transmitter->Transmit(reader, frequency, bandwidth, dmaChannel, optind < argc);

        } while (enable && (optind < argc));
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        result = EXIT_FAILURE;
    }

    delete transmitter;
    transmitter = nullptr;

    return result;
}
