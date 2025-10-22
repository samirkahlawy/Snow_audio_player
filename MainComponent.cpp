#include "MainComponent.h"

MainComponent::MainComponent() : playerGUI(player)
{
    addAndMakeVisible(playerGUI);
    setSize(1000, 700);
    setAudioChannels(0, 2); // no inputs, 2 outputs
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkcyan);
}

void MainComponent::resized()
{
    playerGUI.setBounds(getLocalBounds());
}

