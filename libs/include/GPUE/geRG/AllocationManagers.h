#ifndef GE_RG_ALLOCATION_MANAGERS_H
#define GE_RG_ALLOCATION_MANAGERS_H

#include <vector>
#include <geRG/Export.h>

namespace ge
{
   namespace rg
   {

      /** ArrayAllocation represents a single allocation of memory for
       *  array of items. ArrayAllocationManager uses it for memory management.
       *  It keeps vector of ArrayAllocations accessed by integer-based ID value.
       *  Each ID value represents single memory allocation. The ID value is
       *  returned to the user during the allocation process.
       *
       *  The smallest allocation is one array element, or theoretically zero
       *  for allocating just ID. All the items in the array are expected to be
       *  of the same size. The smallest item size is 1 byte.
       */
      template<typename OwnerType>
      struct ArrayAllocation {
         unsigned startIndex;       ///< Index of the start of the allocated array. The real offset is startIndex multiplied by the size of the array item.
         unsigned numItems;         ///< Number of items in the array. The real size is numItems multiplied by the size of the item.
         unsigned nextRec;          ///< \brief Index of ArrayAllocation whose allocated memory follows the current block's memory.
         OwnerType *owner;          ///< Object that owns the allocated array. Null indicates free memory block.
         inline ArrayAllocation()  {}  ///< Default constructor. It does nothing.
         inline ArrayAllocation(unsigned startIndex,unsigned numItems,unsigned nextRec,OwnerType *owner);  ///< Constructs object by the given values.
      };


      /** ArrayAllocationManager provides memory management functionality.
       *  It allows to allocate item arrays. All the items must be of the same size,
       *  but the item can be anything from a single byte to a large structure.
       *  Each array is identified by the intenger-based ID.
       *  The smallest allocation unit is one item. The largest allocation possible
       *  is equal to the largest available continuous block.
       *
       *  ArrayAllocationManager does not maintain any memory buffer. It just provides
       *  memory management functionality. See BufferStorage template for combining
       *  ge::gl::BufferObject and ArrayAllocationManager functionality.
       */
      template<typename ArrayOwnerType>
      class ArrayAllocationManager {
      protected:

         std::vector<ArrayAllocation<ArrayOwnerType>> _allocations;  ///< Array containing allocation info for all allocations. It is indexed by id. Zero id is reserved for invalid id.
         unsigned _capacity;                    ///< Total number of items (allocated and unallocated).
         unsigned _available;                   ///< Number of items available for allocation.
         unsigned _numItemsAvailableAtTheEnd;   ///< Number of available items at the end of the managed memory, e.g. number of items in the block at the end.
         unsigned _firstItemAvailableAtTheEnd;  ///< Index of the first available item at the end of the managed memory, e.g. the first available item that is followed by available items only.
         unsigned _idOfArrayAtTheEnd;           ///< Id (index to std::vector\<BlockAllocation\>) of the last allocated block at the end of the managed memory.

      public:

         typedef ArrayOwnerType AllocationOwner;
         typedef typename std::vector<ArrayAllocation<ArrayOwnerType>>::iterator iterator;
         typedef typename std::vector<ArrayAllocation<ArrayOwnerType>>::const_iterator const_iterator;

         inline ArrayAllocationManager(unsigned capacity=0);
         inline ArrayAllocationManager(unsigned capacity,unsigned numItemsOfNullObject);
         void setCapacity(unsigned value);
         inline unsigned capacity() const;                   ///< Returns total number of items (allocated and unallocated).
         inline unsigned available() const;                  ///< Returns the number of items that are available for allocation.
         inline unsigned largestAvailable() const;           ///< Returns the number of available items in the largest continuous array.
         inline unsigned numItemsAvailableAtTheEnd() const;  ///< Returns the number of items that are available at the end of the managed memory, e.g. number of items in the array at the end.
         inline unsigned firstItemAvailableAtTheEnd() const; ///< Returns the index of the first available item at the end of the managed memory, e.g. the first available item that is followed by available items only.
         inline unsigned idOfArrayAtTheEnd() const;          ///< Returns id (index to std::vector\<ArrayAllocation\>) of the last allocated array at the end of the managed memory.
         inline unsigned numNullItems() const;               ///< Returns the number of null items. Null items are stored in the array with Id 0 and always placed on the beginning of the allocated memory or buffer.

         inline ArrayAllocation<ArrayOwnerType>& operator[](unsigned id);
         inline const ArrayAllocation<ArrayOwnerType>& operator[](unsigned id) const;
         inline iterator begin();
         inline iterator end();
         inline const_iterator begin() const;
         inline const_iterator end() const;

         inline bool canAllocate(unsigned numItems) const;
         unsigned alloc(unsigned numItems,ArrayOwnerType &owner);  // Allocates number of items. Returns id or zero on failure.
         inline void free(unsigned id);  // Frees allocated items. Id must be valid. Zero id is allowed and safely ignored.
         void clear();
      };


      /** ItemAllocationManager provides memory management functionality.
       *  It allows to allocate items. All the items must be of the same size,
       *  but the item can be anything from a single byte to a large structure.
       *  Items are identified by the intenger-based ID. ID is the number of the item,
       *  begining at 0 for the first item and ending at numItemsTotal()-1 for the last item.
       *
       *  The smallest allocation unit is one item. The largest allocation possible
       *  is equal to the number of available items.
       *
       *  ItemAllocationManager does not maintain any memory buffer. It just provides
       *  memory management functionality. See BufferStorage template for combining
       *  ge::gl::BufferObject and ItemAllocationManager functionality.
       */
      class GERG_EXPORT ItemAllocationManager {
      protected:

         std::vector<unsigned*> _allocations;
         unsigned _capacity;                    ///< Total number of items (allocated plus unallocated).
         unsigned _available;                   ///< Number of items available for allocation.
         unsigned _numItemsAvailableAtTheEnd;   ///< Number of available items at the end of the managed memory, e.g. number of items in the block at the end.
         unsigned _firstItemAvailableAtTheEnd;  ///< Index of the first available item at the end of the managed memory that is followed by available items only.
         unsigned _numNullItems;                ///< Number of null objects (null design pattern) allocated at the beginning of the managed memory.

      public:

         typedef std::vector<unsigned*>::iterator iterator;
         typedef std::vector<unsigned*>::const_iterator const_iterator;

         inline ItemAllocationManager(unsigned capacity=0);
         inline ItemAllocationManager(unsigned capacity,unsigned numNullItems);
         void setCapacity(unsigned value);
         inline unsigned capacity() const;                    ///< Returns total number of items (allocated and unallocated).
         inline unsigned available() const;                   ///< Returns the number of items that are available for allocation.
         inline unsigned largestAvailable() const;            ///< Returns the number of available items in the largest continuous array.
         inline unsigned numItemsAvailableAtTheEnd() const;   ///< Returns the number of items that are available at the end of the managed memory, e.g. number of items in the array at the end.
         inline unsigned firstItemAvailableAtTheEnd() const;  ///< Returns the index of the first available item at the end of the managed memory, e.g. the first available item that is followed by available items only.
         inline unsigned numNullItems() const;                ///< Returns the number of null items (null design pattern) allocated at the beginning of the managed memory. Ids of null items are in the range 0 and numNullItems()-1.

         inline unsigned*& operator[](unsigned id);
         inline unsigned*const& operator[](unsigned id) const;
         inline void setOwner(unsigned id,unsigned*const owner);
         inline unsigned* owner(unsigned id) const;
         inline iterator begin();
         inline iterator end();
         inline const_iterator begin() const;
         inline const_iterator end() const;
         inline unsigned firstId() const;
         inline unsigned lastId() const;

         inline bool canAllocate(unsigned numItems) const;
         bool alloc(unsigned *id);  ///< \brief Allocates one item and stores the item's id in the variable pointed by id parameter.
         bool alloc(unsigned numItems,unsigned* ids);  ///< \brief Allocates number of items. The returned items may not form the continuous block of memory. Array pointed by ids must be at least numItems long.
         void free(unsigned id);  ///< Frees allocated item. Id must be valid.
         void free(unsigned* ids,unsigned numItems);  ///< Frees allocated items. Ids pointed by ids parameter must be valid.
         void clear();
         void assertEmpty();
      };

   }
}



// inline and template methods

#include <cstring>

namespace ge
{
   namespace rg
   {

      template<typename OwnerType> inline ArrayAllocation<OwnerType>::ArrayAllocation(unsigned startIndex,unsigned numItems,unsigned nextRec,OwnerType *owner)
      { this->startIndex=startIndex; this->numItems=numItems; this->nextRec=nextRec; this->owner=owner; }

      template<typename OwnerType>
      inline ArrayAllocationManager<OwnerType>::ArrayAllocationManager(unsigned capacity)
         : _capacity(capacity), _available(capacity), _numItemsAvailableAtTheEnd(capacity),
           _firstItemAvailableAtTheEnd(0), _idOfArrayAtTheEnd(0)
      {
         // zero element is reserved for invalid object
         // and it serves as Null object (Null object design pattern)
         _allocations.emplace_back(0,0,0,nullptr);
      }
      template<typename OwnerType>
      inline ArrayAllocationManager<OwnerType>::ArrayAllocationManager(unsigned capacity,unsigned numItemsOfNullObject)
         : _capacity(capacity), _available(capacity-numItemsOfNullObject),
           _numItemsAvailableAtTheEnd(capacity-numItemsOfNullObject),
           _firstItemAvailableAtTheEnd(numItemsOfNullObject), _idOfArrayAtTheEnd(0)
      {
         // zero element is reserved for invalid object
         // and it serves as Null object (Null object design pattern)
         _allocations.emplace_back(0,numItemsOfNullObject,0,nullptr);
      }
      template<typename OwnerType> void ArrayAllocationManager<OwnerType>::setCapacity(unsigned value)
      {
         int delta=value-_capacity;
         _capacity=value;
         _available+=delta;
         _numItemsAvailableAtTheEnd+=delta;
      }
      template<typename OwnerType>
      void ArrayAllocationManager<OwnerType>::clear()
      {
         auto numItemsOfNullArray=numNullItems();
         _allocations.clear();
         _allocations.emplace_back(0,numItemsOfNullArray,0,nullptr);
         _available=_capacity-numItemsOfNullArray;
         _numItemsAvailableAtTheEnd=_available;
         _firstItemAvailableAtTheEnd=numItemsOfNullArray;
         _idOfArrayAtTheEnd=0;
      }
      inline ItemAllocationManager::ItemAllocationManager(unsigned capacity)
         : _allocations(capacity), // sets the capacity and resizes the vector, default-inserting elements
           _capacity(capacity), _available(capacity-1), _numItemsAvailableAtTheEnd(capacity-1),
           _firstItemAvailableAtTheEnd(1), _numNullItems(1)  { operator[](0)=nullptr; }
      inline ItemAllocationManager::ItemAllocationManager(unsigned capacity,unsigned numNullItems)
         : _allocations(capacity), // sets the capacity and resizes the vector, default-inserting elements
           _capacity(capacity), _available(capacity-numNullItems),
           _numItemsAvailableAtTheEnd(capacity-numNullItems),
           _firstItemAvailableAtTheEnd(numNullItems), _numNullItems(numNullItems)
      {
         std::memset(_allocations.data(),0,numNullItems*sizeof(unsigned*));
      }

      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::capacity() const  { return _capacity; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::available() const  { return _available; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::largestAvailable() const  { return _numItemsAvailableAtTheEnd; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::numItemsAvailableAtTheEnd() const  { return _numItemsAvailableAtTheEnd; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::firstItemAvailableAtTheEnd() const  { return _firstItemAvailableAtTheEnd; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::idOfArrayAtTheEnd() const  { return _idOfArrayAtTheEnd; }
      template<typename OwnerType> inline unsigned ArrayAllocationManager<OwnerType>::numNullItems() const  { return _allocations[0].numItems; }
      inline unsigned ItemAllocationManager::capacity() const  { return _capacity; }
      inline unsigned ItemAllocationManager::available() const  { return _available; }
      inline unsigned ItemAllocationManager::largestAvailable() const  { return _numItemsAvailableAtTheEnd; }
      inline unsigned ItemAllocationManager::numItemsAvailableAtTheEnd() const  { return _numItemsAvailableAtTheEnd; }
      inline unsigned ItemAllocationManager::firstItemAvailableAtTheEnd() const  { return _firstItemAvailableAtTheEnd; }
      inline unsigned ItemAllocationManager::numNullItems() const  { return _numNullItems; }

      template<typename OwnerType> inline ArrayAllocation<OwnerType>& ArrayAllocationManager<OwnerType>::operator[](unsigned id)  { return _allocations[id]; }
      template<typename OwnerType> inline const ArrayAllocation<OwnerType>& ArrayAllocationManager<OwnerType>::operator[](unsigned id) const  { return _allocations[id]; }
      template<typename OwnerType> inline typename ArrayAllocationManager<OwnerType>::iterator ArrayAllocationManager<OwnerType>::begin()
      { return _allocations.begin()+1; } // there is always null object in the list, so we increment by one to skip it
      template<typename OwnerType> inline typename ArrayAllocationManager<OwnerType>::iterator ArrayAllocationManager<OwnerType>::end()
      { return _allocations.end(); }
      template<typename OwnerType> inline typename ArrayAllocationManager<OwnerType>::const_iterator ArrayAllocationManager<OwnerType>::begin() const
      { return _allocations.begin()+1; } // there is always null object in the list, so we increment by one to skip it
      template<typename OwnerType> inline typename ArrayAllocationManager<OwnerType>::const_iterator ArrayAllocationManager<OwnerType>::end() const
      { return _allocations.end(); }
      inline unsigned*& ItemAllocationManager::operator[](unsigned id)  { return _allocations[id]; }
      inline unsigned*const& ItemAllocationManager::operator[](unsigned id) const  { return _allocations[id]; }
      inline void ItemAllocationManager::setOwner(unsigned id,unsigned*const owner)  { _allocations[id]=owner; }
      inline unsigned* ItemAllocationManager::owner(unsigned id) const  { return _allocations[id]; }
      inline ItemAllocationManager::iterator ItemAllocationManager::begin()  { return _allocations.begin()+_numNullItems; }
      inline ItemAllocationManager::iterator ItemAllocationManager::end()  { return _allocations.end()-_numItemsAvailableAtTheEnd; }
      inline ItemAllocationManager::const_iterator ItemAllocationManager::begin() const  { return _allocations.begin()+_numNullItems; }
      inline ItemAllocationManager::const_iterator ItemAllocationManager::end() const  { return _allocations.end()-_numItemsAvailableAtTheEnd; }
      inline unsigned ItemAllocationManager::firstId() const  { return _numNullItems; }
      inline unsigned ItemAllocationManager::lastId() const  { return _firstItemAvailableAtTheEnd-1; }

      template<typename OwnerType> inline bool ArrayAllocationManager<OwnerType>::canAllocate(unsigned numItems) const
      { return _numItemsAvailableAtTheEnd>=numItems; }
      inline bool ItemAllocationManager::canAllocate(unsigned numItems) const
      { return _numItemsAvailableAtTheEnd>=numItems; }

      template<typename OwnerType>
      unsigned ArrayAllocationManager<OwnerType>::alloc(unsigned numItems,OwnerType &owner)
      {
         if(_numItemsAvailableAtTheEnd<numItems)
            return 0;

         unsigned id=unsigned(_allocations.size());
         _allocations.emplace_back(_firstItemAvailableAtTheEnd,numItems,0,&owner);
         _allocations.operator[](_idOfArrayAtTheEnd).nextRec=id;
         _available-=numItems;
         _numItemsAvailableAtTheEnd-=numItems;
         _firstItemAvailableAtTheEnd+=numItems;
         _idOfArrayAtTheEnd=id;
         return id;
      }
      template<typename OwnerType> inline void ArrayAllocationManager<OwnerType>::free(unsigned id)  { _allocations.operator[](id).owner=nullptr; }

   }
}

#endif /* GE_RG_ALLOCATION_MANAGERS_H */
