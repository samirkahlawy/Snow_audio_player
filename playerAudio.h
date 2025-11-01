#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource, public juce::ChangeListener
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL(const juce::URL& audioURL);
    void setGain(float gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setLooping(bool shouldLoop);

    bool isLouded();

    void start();
    void stop();

    void setMute(bool shouldMute);
    void goStart();
    void goEnd();
    void repeat(bool shouldRepeat);

    double getCurrentPosition() const;

    double getPosition() const;
    double getLengthInSeconds() const;
    bool isPlaying() const;

    // AudioSource functions
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    double getLengthInSeconds();
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampleSource;
    bool looping = false;

    bool isLoaded = false;

    float previousGain = 1.0f;
};
