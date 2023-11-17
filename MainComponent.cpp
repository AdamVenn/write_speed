#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(600, 400);
    addAndMakeVisible(m_fileBrowser);
    addAndMakeVisible(m_slideSize);
    addAndMakeVisible(m_labelKib);
    addAndMakeVisible(m_go);
    addAndMakeVisible(m_output);

    m_slideSize.valueFromTextFunction = [](const String& text) -> double {
        auto trimmedText{text.dropLastCharacters(4)}; // " KiB"
        auto unit = text.getLastCharacters(3);
        auto val {trimmedText.getDoubleValue()};
        if (unit == "KiB") {
            return val;
        }
        else if (unit == "MiB") {
            return val * 1024;
        }
        else if (unit == "GiB") {
            return val * 1024 * 1024;
        }
        return val;
    };
    m_slideSize.textFromValueFunction = [this](double val) -> String {
        return human_readable_bytes(val);
    };
    m_slideSize.setRange(1.0, 1024.0 * 1024.0, 1024.0);

    m_go.onClick = [this](){time_slider();};

    double val{1024};
    for (auto* but : m_preset)
    {
        but->setButtonText(human_readable_bytes(val));
        but->value = val;
        but->onClick = [this, but](){time_writes(but->value);};
        addAndMakeVisible(but);

        val *= 2;
    }
    
    m_output.setMultiLine(true);
    m_output.setReadOnly(true);
    m_output.setCaretVisible(false);
}

//==============================================================================
void MainComponent::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    FlexBox mainBox;
    FlexBox butBox;

    mainBox.flexWrap = FlexBox::Wrap::noWrap;
    mainBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
    mainBox.alignContent = FlexBox::AlignContent::stretch;
    mainBox.flexDirection = FlexBox::Direction::column;

    butBox.flexWrap = FlexBox::Wrap::noWrap;
    butBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
    butBox.alignContent = FlexBox::AlignContent::stretch;
    butBox.flexDirection = FlexBox::Direction::row;

    for (auto* but : m_preset)
    {
        butBox.items.add(FlexItem(*but).withMinHeight (20.0f).withMinWidth (50.0f).withMaxHeight(50.f).withFlex (1));
    }

    mainBox.items.add(FlexItem(m_fileBrowser).withMinHeight (200.0f).withMinWidth (50.0f).withFlex (1));
    mainBox.items.add(FlexItem(butBox).withMinHeight (50.0f).withMinWidth (50.0f).withFlex (0));
    mainBox.items.add(FlexItem(m_slideSize).withMinHeight (50.0f).withMinWidth (50.0f).withFlex (0));
    mainBox.items.add(FlexItem(m_go).withMinHeight (50.0f).withMinWidth (50.0f).withFlex (0));
    mainBox.items.add(FlexItem(m_output).withMinHeight (200.0f).withMinWidth (50.0f).withFlex (1));

    mainBox.performLayout (getLocalBounds());
}

void MainComponent::time_writes(double noOfBytes)
{
    const auto location{m_fileBrowser.getSelectedFile(0)};
    double duration{};

    TemporaryFile temp(location);

    if (auto out = std::unique_ptr<FileOutputStream>(temp.getFile().createOutputStream()))
    {
        Array<uint8> bytes{};
        bytes.resize(noOfBytes);
        bytes.fill(1);
        {
            ScopedTimeMeasurement timeIt{duration};
            out->write(bytes.getRawDataPointer(), noOfBytes);
        }
        out.reset();
    }

    auto msg{
        String{noOfBytes, 0} + " kib: " + String{duration * 1000} + " ms = " +
        String{noOfBytes / 1000 / duration, 2} + "MiB/S" + "\n"
    };
    m_output.setCaretPosition(m_output.getText().length());
    m_output.insertTextAtCaret(msg);
}

void MainComponent::time_slider()
{
    const auto noOfBytes{m_slideSize.getValue() * 1024};
    time_writes(noOfBytes);
}

String MainComponent::human_readable_bytes(double val)
{
    if (val < 1024) {
        // KiB
        return String{std::round(val)} + String{" KiB"};
    }
    else if (val >= 1024.0 and val < 1024 * 1024) {
        // MiB
        return String{std::round(val / 1024.0)} + String{" MiB"};
    }
    else if (val >= 1024.0 * 1024.0) {
        // GiB
        return String{std::round(val / (1024.0 * 1024.0)) } + String{" GiB"};
    }
    else {
        jassertfalse;
        return String{val} + String{" KiB"};
    }
}
