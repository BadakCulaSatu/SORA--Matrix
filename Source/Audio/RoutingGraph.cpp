#include "RoutingGraph.h"

RoutingGraph::RoutingGraph()
{
    setPlayConfigDetails(0, 2, getSampleRate(), getBlockSize());
}

RoutingGraph::~RoutingGraph()
{
    clearGraph();
}

bool RoutingGraph::addInputNode(const juce::String& inputDeviceName)
{
    auto node = addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    
    if (node != nullptr)
    {
        inputNodeId = node->nodeId;
        return true;
    }
    return false;
}

bool RoutingGraph::addOutputNode(const juce::String& outputDeviceName)
{
    auto node = addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    
    if (node != nullptr)
    {
        outputNodeId = node->nodeId;
        return true;
    }
    return false;
}

bool RoutingGraph::addRoute(int sourceNodeId, int sourceChannel, 
                           int destNodeId, int destChannel)
{
    return addConnection({
        {sourceNodeId, sourceChannel},
        {destNodeId, destChannel}
    });
}

bool RoutingGraph::addProcessingNode(juce::AudioProcessor* processor)
{
    auto node = addNode(std::unique_ptr<juce::AudioProcessor>(processor));
    return node != nullptr;
}

void RoutingGraph::clearGraph()
{
    clear();
    inputNodeId = -1;
    outputNodeId = -1;
}
