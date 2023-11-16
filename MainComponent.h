#pragma once


#include <juce_gui_extra/juce_gui_extra.h>

using namespace juce;

class MainComponent final : public juce::Component
{
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    /* What we're all here for */
    void time_writes();

    FileBrowserComponent m_fileBrowser{
        FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectDirectories,
        File{},  // default starting location
        nullptr,  // all files
        nullptr  // no file previews
    };
    TextEditor m_inputSize{};
    TextButton m_go{"Time write operation"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
