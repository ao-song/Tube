#ifndef PAYLOADBUFFER_H
#define PAYLOADBUFFER_H

#include <cassert>
#include <cstring>
#include <memory>

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

        // data in a chunk
		const void*
		get_data() const;

        // length of the data in a chunk
		size_t
		get_length() const;

        // the total length of data in the buffer
		size_t
		get_total_length() const;

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

		std::shared_ptr<Chunk> firstChunkM;
		size_t                 lengthM;
		size_t                 offsetM;
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
}

#endif