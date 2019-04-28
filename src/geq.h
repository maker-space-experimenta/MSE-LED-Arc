// graphic equalizer
#include "globals.h"
#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT();

#define SAMPLES 1024 // Must be a power of 2
#define SAMPLING_FREQUENCY                                                                                                                 \
    40000             // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT
                      // Fmax=sampleF/2.
#define amplitude 150 // Depending on your audio source level, you may need to increase this value
unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0, 0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int dominant_value;

float averages[8];
int averagesCount[8] = {0};

int lastBand = 0;
void displayBand(int band, int size) {
    size /= amplitude;
    if (size > peak[band]) {
        peak[band] = size;
    }
    averages[band] += size;
    averagesCount[band]++;
    // Serial.printf("%d, %d\n", band, size);
}

void geqInit() { sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); }

void geqLoop() {
    for (int i = 0; i < SAMPLES; i++) {
        newTime = micros();
        vReal[i] = analogRead(MIC_PIN);
        vImag[i] = 0;
        while ((micros() - newTime) < sampling_period_us) { /* do nothing to wait */
        }
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    for (int i = 2; i < (SAMPLES / 2); i++) { // Don't use sample 0 and only the first SAMPLES/2 are usable.
        // Each array element represents a frequency and its value, is the amplitude. Note the frequencies are not discrete.
        if (vReal[i] > amplitude * 10) { // Add a crude noise filter, 10 x amplitude or more
            if (i <= 2)
                displayBand(0, (int)vReal[i]); // 125Hz
            if (i > 2 && i <= 4)
                displayBand(1, (int)vReal[i]); // 250Hz
            if (i > 4 && i <= 7)
                displayBand(2, (int)vReal[i]); // 500Hz
            if (i > 7 && i <= 15)
                displayBand(3, (int)vReal[i]); // 1000Hz
            if (i > 15 && i <= 40)
                displayBand(4, (int)vReal[i]); // 2000Hz
            if (i > 40 && i <= 70)
                displayBand(5, (int)vReal[i]); // 4000Hz
            if (i > 70 && i <= 288)
                displayBand(6, (int)vReal[i]); // 8000Hz
            if (i > 288)
                displayBand(7, (int)vReal[i]); // 16000Hz
                                               // Serial.println(i);
            // Serial.printf("%d,", (int)vReal[i]);
            // if(i == (SAMPLES / 2) - 1) {
            //     Serial.println();
            // }
        }
        // for (byte band = 0; band <= 7; band++)
        //     display.drawHorizontalLine(1 + 16 * band, 64 - peak[band], 14);
    }

    for (byte band = 0; band <= 7; band++) {
        if (band == 0) {
            Serial.println("\n\n\n\n\n");
        }
        int val = 0;
        if (averagesCount[band] > 0) {
            val = averages[band] / averagesCount[band];
        }
        Serial.printf("\n%d %4d ", band, val);
        for (int i = 0; i < val / 8; i++) {
            Serial.print('#');
        }
        averages[band] = 0;
        averagesCount[band] = 0;
    }

    // if (millis() % 4 == 0) { // Decay the peak
    //     for (byte band = 0; band <= 7; band++) {
    //         if (peak[band] > 0)
    //             peak[band] -= 1;
    //     }
    // }
}