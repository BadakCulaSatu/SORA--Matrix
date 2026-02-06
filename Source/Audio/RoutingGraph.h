#pragma once
#include <JuceHeader.h>

class RoutingGraph : public juce::AudioProcessorGraph
{
public:
    RoutingGraph();
    ~RoutingGraph() override;
    
    bool addInputNode(const juce::String& inputDeviceName);
    bool addOutputNode(const juce::String& outputDeviceName);
    bool addRoute(int sourceNodeId, int sourceChannel, 
                  int destNodeId, int destChannel);
    bool addProcessingNode(juce::AudioProcessor* processor);
    
    void clearGraph();
    
    int getInputNodeId() const { return inputNodeId; }
    int getOutputNodeId() const { return outputNodeId; }
    
private:
    int inputNodeId = -1;
    int outputNodeId = -1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RoutingGraph)
};
