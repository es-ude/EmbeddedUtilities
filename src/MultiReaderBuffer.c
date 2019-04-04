#include "Util/MultiReaderBuffer.h"
#include "CException.h"

/**
 *  Helper function
 */
static void
copyMemory(void *destination, const void *source, uint8_t number_of_bytes);

/**
 *  Helper function
 */
static void*
nextWordPosition(MultiReaderBuffer *buffer, void *pointer);

/**
 *  Helper function
 */
static void
checkIfReadingIsPossible(MultiReaderBuffer *buffer, uint8_t reader_descriptor);

void
initMultiReaderBuffer(MultiReaderBuffer *self, uint8_t word_size, size_t max_elements, size_t max_readers)
{
  self->word_size_in_byte = word_size;
  self->max_elements = max_elements;
  self->max_readers = max_readers;

  self->start = (void*) (((uint8_t*) self) + sizeof(MultiReaderBuffer));

  for (uint8_t byte_in_buffer = 0; byte_in_buffer < (self->word_size_in_byte * (self->max_elements + 1)); ++byte_in_buffer)
  {
    *(((uint8_t*) self->start) + byte_in_buffer) = 0; // Set buffer content to 0
  }

  self->write = self->start;

  self->readers = (void**) (((uint8_t*) self->start) + (self->word_size_in_byte * (self->max_elements + 1)));
	self->reader_state_indicators = (BufferReaderState*) (self->readers + self->max_readers);

  for (uint8_t reader_index = 0; reader_index < self->max_readers; ++reader_index)
  {
    *(self->readers + reader_index) = self->start;  // All readers initially at start position
    *(self->reader_state_indicators + reader_index) = BUFFER_READER_INVALID; // All readers initially invalid
  }
}

void
pushToBuffer(Buffer *self, const void *data)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  for (uint8_t reader_slot = 0; reader_slot < impl->max_readers; ++reader_slot)
  {
    /* Check for all read pointers whether they need to be updated */

    if ((impl->readers)[reader_slot] == nextWordPosition(impl, impl->write))
    {
      /* Write pointer has overrun a read pointer */

      if ((impl->reader_state_indicators)[reader_slot] != BUFFER_READER_INVALID)
      {
        (impl->reader_state_indicators)[reader_slot] = BUFFER_READER_OVERRUN;
      }

      (impl->readers)[reader_slot] = nextWordPosition(impl, (impl->readers)[reader_slot]);
    }
  }

  copyMemory(impl->write, data, impl->word_size_in_byte);
  impl->write = nextWordPosition(impl, impl->write);
}

const void*
peekAtBufferWithReader(const Buffer *self, uint8_t reader_descriptor)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  checkIfReadingIsPossible(impl, reader_descriptor);  // Can throw exceptions

  return (impl->readers)[reader_descriptor];
}

const void*
popFromBufferWithReader(Buffer *self, uint8_t reader_descriptor)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  checkIfReadingIsPossible(impl, reader_descriptor);  // Can throw exceptions

  void *return_value = (impl->readers)[reader_descriptor];
  (impl->readers)[reader_descriptor] = nextWordPosition(impl, (impl->readers)[reader_descriptor]);

  return return_value;
}

uint8_t 
getNewBufferReaderDescriptor(Buffer *self)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  for (uint8_t reader_slot = 0; reader_slot < impl->max_readers; ++reader_slot)
  {
    if ((impl->reader_state_indicators)[reader_slot] == BUFFER_READER_INVALID)
    {
      (impl->reader_state_indicators)[reader_slot] = BUFFER_READER_VALID;
      return reader_slot;
    }
  }

  Throw(BUFFER_NO_FREE_READER_SLOTS_EXCEPTION);
  return 0; // Should not be reached
}

bool
readableItemExistsForReader(const Buffer *self, uint8_t reader_descriptor)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  if (reader_descriptor < impl->max_readers && (impl->reader_state_indicators)[reader_descriptor] != BUFFER_READER_INVALID)
  {
    if ((impl->readers)[reader_descriptor] != impl->write)  // Equality would mean buffer is empty
    {
      return true;
    }
  }

  return false;
}

void
deleteBufferReaderDescriptor(Buffer *self, uint8_t reader_descriptor)
{
  MultiReaderBuffer *impl = (MultiReaderBuffer*) self;

  if (reader_descriptor < impl->max_readers)
  {
    (impl->reader_state_indicators)[reader_descriptor] = BUFFER_READER_INVALID;
  }
  else
  {
    Throw(BUFFER_INVALID_READER_EXCEPTION);
  }
}

/* Helper functions */

void
checkIfReadingIsPossible(MultiReaderBuffer *buffer, uint8_t reader_descriptor)
{
  if (reader_descriptor >= buffer->max_readers || (buffer->reader_state_indicators)[reader_descriptor] == BUFFER_READER_INVALID)
  {
    Throw(BUFFER_INVALID_READER_EXCEPTION);
  }

  if ((buffer->reader_state_indicators)[reader_descriptor] == BUFFER_READER_OVERRUN)
  {
    /* 
     * Enable subsequent reading operations by flagging reader as valid again
     * and pointing it to the currently oldest buffer element
     */

    (buffer->reader_state_indicators)[reader_descriptor] = BUFFER_READER_VALID;
    (buffer->readers)[reader_descriptor] = nextWordPosition(buffer, buffer->write);

    Throw(BUFFER_OVERRUN_EXCEPTION);
  }

  if ((buffer->readers)[reader_descriptor] == buffer->write)
  {
    Throw(BUFFER_UNDERRUN_EXCEPTION);
  }
}

void
copyMemory(void *destination, const void *source, uint8_t number_of_bytes)
{
  while (number_of_bytes > 0)
  {
    *((uint8_t*) destination++) = *((uint8_t*) source++);
    --number_of_bytes;
  }
}

void*
nextWordPosition(MultiReaderBuffer *buffer, void *pointer)
{
  pointer += buffer->word_size_in_byte;

  if (pointer >= buffer->start + (buffer->word_size_in_byte * (buffer->max_elements + 1)))
  {
    pointer = buffer->start;
  }

  return pointer;
}
