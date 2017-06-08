#ifndef PAYLOADBUFFER_H
#define PAYLOADBUFFER_H

#include <cassert>
#include <cstring>
#include <list>


namespace Tube
{
    class PayloadBuffer
    {
    public:
        PayloadBuffer();

        explicit
        PayloadBuffer(
            const void* data,
            size_t      length,
            size_t      offset);

        ~PayloadBuffer();

        void
        append(
            const void* data,
            size_t      length);

        void
        append(
            const void* data,
            size_t      length,
            size_t      offset);

        // get data from a chunk as an FIFO queue
        const void*
        get_data() const;

        // get the length of the data in a chunk
        size_t
        get_length() const;

        void
        clear();

        bool
        is_empty() const;

    private:
        class Chunk
        {
        public:
            Chunk(
                const void* data,
                size_t      length,
                size_t      offset);

            explicit
            Chunk(
                size_t length);

            ~Chunk();

            unsigned char* dataM;
            size_t         lengthM;
            Chunk*         nextM;

        private:
            Chunk(
                const Chunk& other);
            Chunk&
            operator=(
                const Chunk& other);
        };

    private:
        PayloadBuffer(
            const PayloadBuffer& other);

        PayloadBuffer&
        operator=(const PayloadBuffer&);

        std::list<Chunk> payloadBufferListM;
    };

// ----------------------------------------------------------------------------

    inline
    PayloadBuffer::Chunk::Chunk(
        const void* data,
        size_t      length,
        size_t      offset)
    :   dataM(0),
        lengthM(length - offset),
        nextM(0)
    {
        if(lengthM > 0)
        {
            dataM = new unsigned char[lengthM];
            memcpy(dataM, ((const char*)data) + offset, lengthM);
        }
    }

    inline
    PayloadBuffer::Chunk::Chunk(
        size_t length)
    :   dataM(0),
        lengthM(length),
        nextM(0)
    {
        if(lengthM > 0)
        {
            dataM = new unsigned char[lengthM];
        }
    } 

    inline
    PayloadBuffer::Chunk::~Chunk()
    {
        delete [] dataM;
    }

    inline
    PayloadBuffer::PayloadBuffer()
    : payloadBufferListM(0)
    {
        // Empty
    }

    inline
    PayloadBuffer::PayloadBuffer(
        const void* data,
        size_t      length,
        size_t      offset)
    : payloadBufferListM(0)
    {
        payloadBufferListM.push_back(
            Chunk(data, length, offset));
    }

    inline
    PayloadBuffer::~PayloadBuffer()
    {
        clear();
    }

    inline
    void
    PayloadBuffer::append(
        const void* data,
        size_t      length)
    {
        payloadBufferListM.push_back(
            Chunk(data, length, 0));
    }

    inline
    void
    PayloadBuffer::append(
        const void* data,
        size_t      length,
        size_t      offset)
    {
        payloadBufferListM.push_back(
            Chunk(data, length, offset));
    }

    // get data from a chunk as an FIFO queue
    inline
    const void*
    PayloadBuffer::get_data() const
    {
        if(!is_empty())
        {
            Chunk firstChunk = payloadBufferListM.front();
            payloadBufferListM.pop_front();

            return firstChunk.dataM;        
        }       
        
        return 0;
    }

    // get the length of the data in a chunk
    inline
    size_t
    PayloadBuffer::get_length() const
    {
        if(!is_empty())
        {
            Chunk firstChunk = payloadBufferListM.front();
            payloadBufferListM.pop_front();

            return firstChunk.lengthM;      
        }       
        
        return 0;
    }

    inline
    void
    PayloadBuffer::clear()
    {
        payloadBufferListM.clear();
    }

    inline
    bool
    PayloadBuffer::is_empty() const
    {
        return payloadBufferListM.empty();
    }
}

#endif