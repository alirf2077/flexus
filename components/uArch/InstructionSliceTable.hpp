
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

  public:
    void initialize(uint32_t theSize)
    {
        theISTList.reserve(theSize);
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
        
        auto it = std::find(entries.begin(), entries.end(), anInsnAddr);
        if (it != entries.end()) {
            // HIT: Move this element to the back (MRU)
            // std::rotate shifts elements left, moving the element at 'it' to the end.
            std::rotate(it, it + 1, entries.end());
            return true;
        } else {
            // MISS: Insert new element
            if (entries.size() == capacity) {
                // Evict LRU (the element at the front)
                entries.erase(entries.begin());
            }
            entries.push_back(anInsnAddr);
            return false;
        }
    }

    void lookup(VirtualMemoryAddress anInsnAddr)
    {
        FLEXUS_PROFILE();
        return std::find(entries.begin(), entries.end(), anInsnAddr) != entries.end();
    }

};

} // namespace nuArchARM

#endif //FLEXUS_uARCH_INSTRUCTIONSLICETABLE_HPP_INCLUDED