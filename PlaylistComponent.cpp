/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Nov 2025 9:54:25am
    Author:  ME

  ==============================================================================
*/

#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(PlayerAudio& player)
    : audioPlayer(player)
{
    addAndMakeVisible(table);
    table.getHeader().addColumn("Track", 1, 400);
    table.setModel(this);
    table.setColour(juce::TableListBox::backgroundColourId, juce::Colours::darkgrey);
    table.setColour(juce::TableListBox::textColourId, juce::Colours::white);
}

int PlaylistComponent::getNumRows()
{
    return trackFiles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::darkblue);
    else
        g.fillAll(juce::Colours::black);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (rowNumber < trackFiles.size())
    {
        g.setColour(juce::Colours::white);
        g.drawText(trackFiles[rowNumber].getFileNameWithoutExtension(),
            2, 0, width - 4, height, juce::Justification::centredLeft);
    }
}

void PlaylistComponent::loadFiles()
{
    chooser.launchAsync(juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles
        | juce::FileBrowserComponent::canSelectMultipleItems,
        [this](const juce::FileChooser& fc)
        {
            auto results = fc.getResults();
            for (auto& f : results)
                trackFiles.add(f);

            table.updateContent();
        });
}


void PlaylistComponent::playSelectedTrack()
{
    int selectedRow = table.getSelectedRow();
    if (selectedRow >= 0 && selectedRow < trackFiles.size())
    {
        audioPlayer.loadURL(juce::URL{ trackFiles[selectedRow] });
        audioPlayer.start();
    }
}

void PlaylistComponent::resized()
{
    table.setBounds(getLocalBounds());
}
