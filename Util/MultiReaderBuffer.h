#ifndef MULTI_READER_BUFFER_H
#define MULTI_READER_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "CException.h"

/*!
 * @enum MultiReaderBufferExceptions
 * 
 * @brief	Defines the exceptions that can happen when using the multi reader buffer.
 * 
 * CException is used for an exception framework.
 */
enum
{
	BUFFER_UNDERRUN_EXCEPTION = 0x01,	//!< More items than existent have been tried to be read by a reader.
	BUFFER_OVERRUN_EXCEPTION = 0x02,	//!< More items have been written than could have been read by a reader, i.e. unread items have been overwritten.
	BUFFER_NO_FREE_READER_SLOTS_EXCEPTION = 0x03,	//!< The number of reader slots is exhausted.
	BUFFER_INVALID_READER_EXCEPTION = 0x04	//!< It has been tried to execute an operation with an invalid reader.
} MultiReaderBufferException;

/*!
 * @enum BufferReaderState
 * 
 * @brief  Indicates the different states a reader can have.
 * 
 * A reader is invalid as long as getNewBufferReaderDescriptor returns a descriptor
 * for this particular reader to use for read operations.
 */
typedef enum
{
	BUFFER_READER_VALID = 0x00,	//!< A reader is valid and can be used for read operations.
	BUFFER_READER_OVERRUN = 0x01,	//!< A valid read pointer has been overwritten by the write pointer, i.e. elements are lost.
	BUFFER_READER_INVALID = 0x02	//!< A reader is invalid.
} BufferReaderState;

typedef struct Buffer Buffer;	// Forward declaration
typedef struct MultiReaderBuffer MultiReaderBuffer;

/*!
 * @brief	Writes new data into the buffer.
 * 
 * As with the nature of a circular buffer, the buffer space virtually never "ends"
 * since when space is used up the buffer is written from the start again, overwriting the oldest entries.
 * 
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * @param data
 * 	The data that should be written into the buffer
 */
void 
pushToBuffer(Buffer *self, const void *data);

/*!
 * @brief	 Returns a descriptor for a new buffer reader.
 * 
 * The descriptor shall be used with functions deleteBufferReaderDescriptor,
 * readableItemExistsForReader, popFromBufferWithReader and peekAtBufferWithReader.
 * 
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * 
 * @returns
 * 	A descriptor for the newly allocated reader, represented by an integer
 * 
 * @throws BUFFER_NO_FREE_READER_SLOTS_EXCEPTION
 * 	An exception is thrown when more readers have been tried to be allocated than are allowed;
 * 	use deleteBufferReaderDescriptor to free unused readers.
 */
uint8_t 
getNewBufferReaderDescriptor(Buffer *self);

/*!
 * @brief	Flags the provided reader descriptor as invalid.
 * 
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * @param reader_descriptor
 * 	The descriptor of the reader with which the data should be retrieved
 * 
 * @throws BUFFER_INVALID_READER_EXCEPTION
 * 	An exception is thrown if the descriptor does not correspond to an actual reader slot
 */
void 
deleteBufferReaderDescriptor(Buffer *self, uint8_t reader_descriptor);

/*!
 * @brief	Returns whether there is at least one unread data item left that could be retrieved via
 * 			popFromBufferWithReader or peekAtBufferWithReader.
 * 
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * @param reader_descriptor
 * 	The descriptor of the reader with which the data should be retrieved
 * 
 * @returns
 * 	true if the reader descriptor is valid and there are still elements left to be read;
 * 	false if the reader descriptor is invalid or there are no elements left to be read
 */
bool
readableItemExistsForReader(const Buffer *self, uint8_t reader_descriptor);

/*!
 * @brief	Reads from the buffer including "removal" of the element that has been read.
 * 
 * The element is returned by reference, which needs to be casted into the actual data type
 * (that the user must be aware of) and then dereferenced to retrieve the data item's value.
 * The reader pointer will have proceeded by one data word after this function has been called.
 * 
 * If the reader pointer has been overrun by the write pointer, the reader pointer will be
 * repositioned to the oldest entry in the buffer before an exception will be thrown.
 * Therefore, a subsequent read operation will succeed, given no elements have been written in the meantime.
 *
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * @param reader_descriptor
 * 	The descriptor of the reader with which the data should be retrieved
 * 
 * @returns
 * 	An "untyped" pointer to the current element the reader pointer is positioned at
 * 
 * @throws BUFFER_INVALID_READER_EXCEPTION
 * 	An exception is thrown if the reader descriptor is invalid
 * @throws BUFFER_OVERRUN_EXCEPTION
 * 	An exception is thrown if the reader has been overrun by the write pointer,
 * 	i.e. elements have been lost
 * @throws BUFFER_UNDERRUN_EXCEPTION
 * 	An exception is thrown if there are no elements left to be read for this reader,
 * 	i.e. the buffer is "empty"
 */
const void* 
popFromBufferWithReader(Buffer *self, uint8_t reader_descriptor);

/*!
 * @brief	Reads from the buffer without "removing" the element that has been read.
 * 
 * The element is returned by reference, which needs to be casted into the actual data type
 * (that the user must be aware of) and then dereferenced to retrieve the data item's value.
 * The reader pointer will not have proceeded after this function has been called.
 * 
 * If the reader pointer has been overrun by the write pointer, the reader pointer will be
 * repositioned to the oldest entry in the buffer before an exception will be thrown.
 * Therefore, a subsequent read operation will succeed, given no elements have been written in the meantime.
 * 
 * @param self
 * 	A pointer to the circular buffer, addressed by its generalized type
 * @param reader_descriptor
 * 	The descriptor of the reader with which the data should be retrieved
 * 
 * @returns
 * 	An "untyped" pointer to the current element the reader pointer is positioned at
 * 
 * @throws BUFFER_INVALID_READER_EXCEPTION
 * 	An exception is thrown if the reader descriptor is invalid
 * @throws BUFFER_OVERRUN_EXCEPTION
 * 	An exception is thrown if the reader has been overrun by the write pointer,
 * 	i.e. elements have been lost
 * @throws BUFFER_UNDERRUN_EXCEPTION
 * 	An exception is thrown if there are no elements left to be read for this reader,
 * 	i.e. the buffer is "empty"
 */
const void* 
peekAtBufferWithReader(const Buffer *self, uint8_t reader_descriptor);

/*!
 * @brief	Initializes a multi reader buffer.
 * 
 * This function creates all the necessary structures (see MultiReaderBuffer)
 * to use the provided memory as a multi reader buffer and needs to be called
 * first in order to use all the other functions provided by this header.
 * 
 * The memory passed via the first parameter needs to be large enough to hold
 * the entire multi reader buffer given the set of customization parameters, i.e.
 * the memory should be created using the MULTI_READER_BUFFER_SIZE with the same
 * set of parameters.
 * 
 * @param self
 * 	Pointer to the memory that should be used for the circular buffer
 * @param word_size
 * 	The size in byte of the individual data items that should be managed in the buffer
 * @param max_elements
 * 	The maximum number of elements that the buffer should be able to hold at once
 * @param max_readers
 * 	The maximum number of buffer readers that are allowed to exist in parallel
 */
void 
initMultiReaderBuffer(MultiReaderBuffer *self, uint8_t word_size, size_t max_elements, size_t max_readers);

/*!
 * @define MULTI_READER_BUFFER_SIZE
 * 
 * @brief	Expands to the number of bytes required to create a circular buffer with the provided parameters.
 * 
 * This macro should be used to obtain the correct number of bytes to be allocated for creating a circular buffer.
 * One buffer position is "wasted" for distinguishing whether the buffer is full or empty.
 * 
 * @param word_size
 * 	The size in byte of the individual data items that should be managed in the buffer
 * @param max_elements
 * 	The maximum number of elements that the buffer should be able to hold at once
 * @param max_readers
 * 	The maximum number of buffer readers that are allowed to exist in parallel
 */
#define MULTI_READER_BUFFER_SIZE(word_size, max_elements, max_readers) (sizeof(MultiReaderBuffer) + word_size * (max_elements + 1) + max_readers * (sizeof(void*) + sizeof(BufferReaderState)))

/*!
 * @struct MultiReaderBuffer
 * 
 * @brief	Defines the structure of the circular buffer implementation.
 * 
 * This circular buffer implementation allows to use multiple buffer readers,
 * to provide a mechanism for implementing 1-to-n producer-consumer relationships
 * in an efficient manner as often needed in the realm of embedded systems.
 * It is utilized as part of the data processing pipeline implemented by the edge device.
 */
struct MultiReaderBuffer
{
	uint8_t word_size_in_byte;	//!< Stores the size in byte of the individual data items stored
	size_t max_elements;	//!< Stores the maximum number of elements
	size_t max_readers;	//!< Stores the maximum number of readers allowed in parallel
	void *start;	//!< Stores the pointer to the start position of the buffer
	void *write;	//!< Stores the writer pointer, which always points to the next position it would write to
	void **readers;	//!< Holds the array of readers, where each reader is represented by a pointer into the buffer memory
	BufferReaderState *reader_state_indicators;	//!< For each reader slot this array stores the current reader state (see BufferReaderState)
};

#endif
