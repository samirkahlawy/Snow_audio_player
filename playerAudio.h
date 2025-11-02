#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource, public juce::ChangeListener, public juce::Timer
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
	void setCustomLoopPoints(double startTime, double endTime);


    bool isLouded();

    void start();
    void stop();

    void setMute(bool shouldMute);
    void goStart();
    void goEnd();
	void forward10Sec();
	void back10Sec();
    void repeat(bool shouldRepeat);
	void setCustomLoopEnabled(bool shouldLoop,double startTime,double endTime);

    double getCurrentPosition() const;

    double getPosition() const;
    double getLengthInSeconds() const;
    bool isPlaying() const;

    // AudioSource functions
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    double getLengthInSeconds();

	// Custom loop functions
	bool isCustomLoopEnabled() const { return customLoopEnabled; }
	double getLoopStartTime() const { return loopStartTime; }
	double getLoopEndTime() const { return loopEndTime; }

    void timerCallback() override;
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampleSource;

    bool looping = false;

    bool isLoaded = false;

    float previousGain = 1.0f;

	bool customLoopEnabled = false;
	double loopStartTime = 0.0;
	double loopEndTime = 0.0;       
    };
