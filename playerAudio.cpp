#include "PlayerAudio.h"
#include <JuceHeader.h>

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();


    resampleSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false);

    transportSource.addChangeListener(this);

    isLoaded = false;
    previousGain = 1.0f;

    startTimer(10);
}

PlayerAudio::~PlayerAudio()
{
    stopTimer();
    transportSource.removeChangeListener(this);
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    resampleSource.reset();
}


void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (resampleSource)
        resampleSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (resampleSource)
        resampleSource->getNextAudioBlock(bufferToFill);
    else
        bufferToFill.clearActiveBufferRegion();
}

void PlayerAudio::releaseResources()
{
    if (resampleSource)
        resampleSource->releaseResources();
}

double PlayerAudio::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::timerCallback()
{
    if (customLoopEnabled && transportSource.isPlaying())
    {
        double pos = transportSource.getCurrentPosition();
        double len = transportSource.getLengthInSeconds();
        if (pos >= loopEndTime || len > 0.0 && std::abs(pos - len) < 0.02)
        {
            transportSource.setPosition(loopStartTime);
        }
    }
}

juce::String PlayerAudio::getArtist() const
{
    if (readerSource != nullptr && readerSource->getAudioFormatReader() != nullptr)
    {
        auto& metadata = readerSource->getAudioFormatReader()->metadataValues;
        return metadata.getValue("artist", "Unknown Artist");
    }
    return "Unknown Artist";
}



void PlayerAudio::loadURL(const juce::URL& audioURL)
{

    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
    isLoaded = false;


    std::unique_ptr<juce::InputStream> inStream(audioURL.createInputStream(false));
    if (!inStream)
        return;


    juce::AudioFormatReader* reader = formatManager.createReaderFor(std::move(inStream));
    if (reader == nullptr)
        return;
    if (reader != nullptr) {
        if (reader->metadataValues.size() > 0)
        {
            juce::Logger::outputDebugString("Metadata found:");
            for (auto& key : reader->metadataValues.getAllKeys())
            {
                juce::Logger::outputDebugString(key + ": " + reader->metadataValues[key]);
            }
        }
        else
        {
            juce::Logger::outputDebugString("No metadata found. Displaying filename instead.");
            juce::Logger::outputDebugString(audioURL.getFileName());
        }

    }
    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));

    readerSource = std::move(newSource);
    transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

    isLoaded = true;
}

bool PlayerAudio::isLouded()
{
    return isLoaded;
}

void PlayerAudio::setGain(float gain)
{
    previousGain = gain;
    transportSource.setGain(gain);
}

void PlayerAudio::setSpeed(double ratio)
{
    if (resampleSource)
        resampleSource->setResamplingRatio(ratio);
}

void PlayerAudio::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource)
        readerSource->setLooping(shouldLoop);

    looping = shouldLoop;
}

void PlayerAudio::setCustomLoopPoints(double startTime, double endTime)
{
    loopStartTime = startTime;
    loopEndTime = endTime;
}
void PlayerAudio::setCustomLoopEnabled(bool shouldLoop, double startTime, double endTime)
{
    customLoopEnabled = shouldLoop;
    if (customLoopEnabled && startTime >= 0 && endTime > startTime)
    {
        loopStartTime = startTime;
        loopEndTime = endTime;
        setLooping(false);
        double pos = transportSource.getCurrentPosition();
        if (pos < loopStartTime || pos>loopEndTime)
        {
            transportSource.setPosition(loopStartTime);
        }
    }
    else if (!shouldLoop)
    {
        setLooping(looping);
    }

}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setMute(bool shouldMute)
{
    if (shouldMute)
    {
        previousGain = transportSource.getGain();
        transportSource.setGain(0.0f);
    }
    else
    {
        float restore = (previousGain <= 0.0f) ? 1.0f : previousGain;
        transportSource.setGain(restore);
    }
}

void PlayerAudio::goStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goEnd()
{
    double len = transportSource.getLengthInSeconds();
    if (len > 0.0)
        transportSource.setPosition(len - 1);
}

void PlayerAudio::forward10Sec()
{
    double pos = transportSource.getCurrentPosition();
    double len = transportSource.getLengthInSeconds();
    if (std::abs(len - pos) > 10)
    {
        transportSource.setPosition(pos + 10.0);
    }
    else
    {
        transportSource.setPosition(len);
    }
}


void PlayerAudio::back10Sec()
{
    double pos = transportSource.getCurrentPosition();
    if (pos > 10)
    {
        transportSource.setPosition(pos - 10.0);
    }
    else
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (!transportSource.isPlaying())
        {
            double pos = transportSource.getCurrentPosition();
            double len = transportSource.getLengthInSeconds();
            if (looping && len > 0.0 && std::abs(pos - len) < 0.05)
            {
                transportSource.setPosition(0.0);
                transportSource.start();
            }
        }
    }
}

void PlayerAudio::repeat(bool shouldRepeat)
{

    setLooping(shouldRepeat);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}

double PlayerAudio::getCurrentPosition() const
{
    return transportSource.getCurrentPosition();
}
void PlayerAudio::loadFile()
{
    juce::FileChooser chooser("Select an audio file to play...", {}, "*.mp3;*.wav");

    chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            juce::File audioFile = fc.getResult();

            if (audioFile.existsAsFile())
            {
                auto* reader = formatManager.createReaderFor(audioFile);

                if (reader != nullptr)
                {
                    if (reader->metadataValues.containsKey("artist"))
                        artist = reader->metadataValues["artist"];
                    else
                        artist = "Unknown Artist";
                }
            }
            

        });
}

