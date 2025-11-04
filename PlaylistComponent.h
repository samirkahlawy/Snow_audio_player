/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Nov 2025 9:54:10am
    Author:  ME

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel
{
public:
    PlaylistComponent(PlayerAudio& player);
    ~PlaylistComponent() override = default;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    void loadFiles();

    void playSelectedTrack();

    void resized() override;

    void addFileToPlaylist(const juce::File& audioFile);

private:
    PlayerAudio& audioPlayer;
    juce::TableListBox table;
    juce::Array<juce::File> trackFiles;
    juce::FileChooser chooser{ "Select multiple audio files..." };

};