//
//  Convolver.cpp
//  Vacancy - Shared Code
//
//  Created by Isaiah Egan on 3/7/18.
//

#include "Convolver.h"

struct Convolver {
    void setup(){
        sampleRate = spec.sampleRate;
        convolution.prepare (spec);
        updateParameters();
    }
        
    void process (ProcessContextReplacing<float> context)
    {
        context.isBypassed = bypass;
        convolution.process (context);
    }
    
    void reset()
    {
        convolution.reset();
    }
    
    void updateParameters()
    {
        if (auto* cabinetTypeParameter = dynamic_cast<ChoiceParameter*> (parameters[0]))
        {
            if (cabinetTypeParameter->getCurrentSelectedID() == 1)
            {
                bypass = true;
            }
            else
            {
                bypass = false;
                
                auto maxSize = static_cast<size_t> (roundToInt (sampleRate * (8192.0 / 44100.0)));
                
                if (cabinetTypeParameter->getCurrentSelectedID() == 2)
                    convolution.loadImpulseResponse (BinaryData::guitar_amp_wav,
                                                     BinaryData::guitar_amp_wavSize,
                                                     false, true, maxSize);
                else
                    convolution.loadImpulseResponse (BinaryData::cassette_recorder_wav,
                                                     BinaryData::cassette_recorder_wavSize,
                                                     false, true, maxSize);
            }
        }
    }
    
    //==============================================================================
    double sampleRate = 0;
    bool bypass = false;
    
    dsp::Convolution convolution;
    
    ChoiceParameter cabinetParam { {"Bypass", "Guitar amplifier 8''", "Cassette recorder"}, 1, "Cabinet Type" };
    
    std::vector<DSPDemoParameterBase*> parameters { &cabinetParam };
};
