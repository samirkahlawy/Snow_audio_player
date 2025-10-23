#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    resampleSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false);
    transportSource.addChangeListener(this);
}

PlayerAudio::~PlayerAudio() {}


void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resampleSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource->getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    resampleSource->releaseResources();
}

void PlayerAudio::loadURL(const juce::URL& audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}



void PlayerAudio::setGain(float gain) { transportSource.setGain(gain); }

void PlayerAudio::setSpeed(double ratio) { resampleSource->setResamplingRatio(ratio); }

void PlayerAudio::setPosition(double posInSecs) { transportSource.setPosition(posInSecs); }

void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource != nullptr)
    {
        readerSource->setLooping(shouldLoop);
    }
}

void PlayerAudio::start() { transportSource.start(); }
void PlayerAudio::stop() { transportSource.stop(); }

void PlayerAudio::setMute(bool shouldMute)
{
    transportSource.setGain(shouldMute ? 0.0 : 1.1);
}

void PlayerAudio::goStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goEnd()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {

        if (!transportSource.isPlaying() &&
            std::abs(transportSource.getCurrentPosition() - transportSource.getLengthInSeconds()) < 0.05)
        {
            if (looping)
            {
                transportSource.setPosition(0.0);
                transportSource.start();
            }
        }
    }
}
void PlayerAudio::repeat(bool shouldRepeat)
{
    looping = shouldRepeat;
}


double PlayerAudio::getPosition() const { return transportSource.getCurrentPosition(); }

double PlayerAudio::getLengthInSeconds() const { return transportSource.getLengthInSeconds(); }

bool PlayerAudio::isPlaying() const { return transportSource.isPlaying(); }


