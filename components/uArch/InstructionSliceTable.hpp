
#ifndef FLEXUS_uARCH_INSTRUCTIONSLICETABLE_HPP_INCLUDED
#define FLEXUS_uARCH_INStRUCTIONSLICETABLE_HPP_INCLUDED

#include "uArchInterfaces.hpp"

#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <core/performance/profile.hpp>
#include <list>
#include <vector>

namespace nuArch {

class InstructionSliceTable
{
  protected:
    std::vector<VirtualMemoryAddress> theISTList;
    uint32_t capacity;

  public:
    void initialize(uint32_t theSize)
    {
        theISTList.reserve(theSize);
        capacity = theSize;
        reset();
    }

    void reset()
    {
        FLEXUS_PROFILE();
        theISTList.clear();
    }

    void access(VirtualMemoryAddress anInsnAddr)
    {
        FLEXUS_PROFILE();
        
        auto it = std::find(theISTList.begin(), theISTList.end(), anInsnAddr);
        if (it != theISTList.end()) {
            // HIT: Move this element to the back (MRU)
            // std::rotate shifts elements left, moving the element at 'it' to the end.
            std::rotate(it, it + 1, theISTList.end());
            return;
        } else {
            // MISS: Insert new element
            if (theISTList.size() == capacity) {
                // Evict LRU (the element at the front)
                theISTList.erase(theISTList.begin());
            }
            theISTList.push_back(anInsnAddr);
            return;
        }
    }

    bool lookup(VirtualMemoryAddress anInsnAddr)
    {
        FLEXUS_PROFILE();
        return std::find(theISTList.begin(), theISTList.end(), anInsnAddr) != theISTList.end();
    }

    void clear()
    {
        FLEXUS_PROFILE();
        theISTList.clear();
    }

};

} // namespace nuArchARM

#endif //FLEXUS_uARCH_INSTRUCTIONSLICETABLE_HPP_INCLUDED