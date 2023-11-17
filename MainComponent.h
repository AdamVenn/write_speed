#pragma once


#include <juce_gui_extra/juce_gui_extra.h>

using namespace juce;

class ValueButton : public TextButton
{
public:
    double value;
};

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
    void time_writes(double noOfBytes);
    void time_slider();

    String human_readable_bytes(double val);


    FileBrowserComponent m_fileBrowser{
        FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectDirectories,
        File{},  // default starting location
        nullptr,  // all files
        nullptr  // no file previews
    };
    Slider m_slideSize{
        Slider::SliderStyle::LinearHorizontal,
        Slider::TextEntryBoxPosition::TextBoxBelow
    };
    // TextEditor m_inputSize{};
    Label m_labelKib{"KiB"};
    TextButton m_go{"Time write operation"};
    OwnedArray<ValueButton> m_preset{
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton,
        new ValueButton
    };

    TextEditor m_output{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
