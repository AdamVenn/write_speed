#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(600, 400);
    addAndMakeVisible(m_fileBrowser);
    addAndMakeVisible(m_inputSize);
    addAndMakeVisible(m_go);
    m_inputSize.setMultiLine(false);
    m_inputSize.setInputRestrictions(16, "0123456789");
    m_go.onClick = [this](){time_writes();};
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    const auto w{getLocalBounds().getWidth()};
    const auto h{getLocalBounds().getHeight()};
    m_fileBrowser.setBounds(0, 0, w, h / 2);
    m_inputSize.setBounds(0, h / 2, w, 50);
    m_go.setBounds(0, h / 2 + 50, w, 50);
}

void MainComponent::time_writes()
{
    const auto text{m_inputSize.getText()};
    const auto noOfBytes{text.getIntValue() * 1024};
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

    auto msg{String{"It took "} + String(duration) + String(" seconds")};
    AlertWindow::showMessageBoxAsync(
        MessageBoxIconType::InfoIcon,
        "Result",
        msg
    );
}
