//-------------------------------------------------------------------------------------------------
#include <graphics/RenderQueue.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
#include <graphics/RenderCommand.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

void RenderQueue::add(RenderCommand** rc, size_t numCommands)
{
    for (size_t i = 0; i < numCommands; i++) {
        RenderCommand* currentRC = rc[i];
        push_back(currentRC);
    }
}

//-------------------------------------------------------------------------------------------------

void RenderQueue::sort()
{
    std::sort(begin(), end(), [](const RenderCommand* a, const RenderCommand* b) {
        return a->key() < b->key();
    });
}

//-------------------------------------------------------------------------------------------------

std::pair<int32_t, int32_t> RenderQueue::getBucket(uint64_t key, uint64_t refMask)
{
    int32_t start = -1;
    int32_t end = -1;
    int32_t currentIndex = 0;
    for (RenderCommand* r : *this) {
        // only check for bits that are set in refMask
        if ((r->key() & refMask) == key) {
            if (start == -1) {
                start = currentIndex;
                end = currentIndex;
            } else {
                end = currentIndex;
            }
        } else {
            // all renderables between start and end should be in the same bucket
            if (start != -1)
                break;
        }

        currentIndex++;
    }

    return std::make_pair(start, end);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
