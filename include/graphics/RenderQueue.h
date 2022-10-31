//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderQueue_h
#define DKE_Graphics_RenderQueue_h
//-------------------------------------------------------------------------------------------------
#include <algorithm>
#include <cstdint>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class RenderCommand;

// A queue of renderables that will be rendered in a given frame.
// The queue will be sorted so that renderables are always render
// in an expected order no matter when they are entering the queue.
//
// Things to consider: should be queueing the draw commands
// instead of queueing renderables. Renderables can be found
// in the commands.
class RenderQueue : public std::vector<RenderCommand*> {
public:
    RenderQueue() = default;
    ~RenderQueue() = default;
    RenderQueue(const RenderQueue& rq) = delete;

    // Adds a renderable to the render queue.
    void add(RenderCommand** rc, size_t numCommands);

    // Sort the render queue based on key.
    void sort();

    // get position of first consecutive renderables which the renderable key matches
    // input key for bits set on refMask.
    std::pair<int, int> getBucket(uint64_t key, uint64_t refMask);

private:
    int m_currentIndex; //current
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderQueue_h
