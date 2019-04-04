#include <unity.h>
#include <CException.h>
#include "Util/MultiReaderBuffer.h"

#define BUFFER_WORD_SIZE (2)
#define MAX_ELEMENTS (50)
#define MAX_READERS (5)

static uint8_t raw_memory_circular_buffer[MULTI_READER_BUFFER_SIZE(BUFFER_WORD_SIZE, MAX_ELEMENTS, MAX_READERS)];
static Buffer *buffer = (Buffer*) &raw_memory_circular_buffer;
static MultiReaderBuffer *circular_buffer = (MultiReaderBuffer*) &raw_memory_circular_buffer;

void
setUp(void)
{
  initMultiReaderBuffer(circular_buffer, BUFFER_WORD_SIZE, MAX_ELEMENTS, MAX_READERS);
}

void
test_getReaderDescriptorWhenCalledFirst(void)
{
  uint8_t descriptor = getNewBufferReaderDescriptor(buffer);
  TEST_ASSERT_EQUAL(0, descriptor);
}

void
test_getReaderDescriptorsWhenCalledAsManyTimesAsMaxReaders(void)
{
  for (uint8_t descriptor_number = 0; descriptor_number < MAX_READERS; ++descriptor_number)
  {
    uint8_t descriptor = getNewBufferReaderDescriptor(buffer);
    TEST_ASSERT_EQUAL(descriptor_number, descriptor);
  }
}

void
test_getReaderDescriptorsWhenCalledMoreTimesThanReaderSlotsAreAvailable(void)
{
  for (uint8_t descriptor_number = 0; descriptor_number < MAX_READERS; ++descriptor_number)
  {
    uint8_t descriptor = getNewBufferReaderDescriptor(buffer);
    TEST_ASSERT_EQUAL(descriptor_number, descriptor);
  }

  CEXCEPTION_T e;

  Try
  {
    getNewBufferReaderDescriptor(buffer);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch (e)
  {
    TEST_ASSERT_EQUAL(BUFFER_NO_FREE_READER_SLOTS_EXCEPTION, e);
  }
}

void
test_deleteReaderDescriptorWhenAllSlotsAreFullAndGetNewReaderSlotAsTheOneThatHasBeenDeleted(void)
{
  const uint8_t saved_descriptor = 2; // less than MAX_READERS

  for (uint8_t descriptor_number = 0; descriptor_number < MAX_READERS; ++descriptor_number)
  {
    getNewBufferReaderDescriptor(buffer);
  }
  
  deleteBufferReaderDescriptor(buffer, saved_descriptor);

  CEXCEPTION_T e;

  Try
  {
    uint8_t descriptor = getNewBufferReaderDescriptor(buffer);
    TEST_ASSERT_EQUAL(saved_descriptor, descriptor);
  }
  Catch (e)
  {
    TEST_FAIL_MESSAGE("Exception thrown!");
  }
}

void
test_pushPeek(void)
{
  uint16_t input = 1;
  pushToBuffer(buffer, &input);
  uint8_t reader = getNewBufferReaderDescriptor(buffer);
  uint16_t output = *((uint16_t*) peekAtBufferWithReader(buffer, reader));
  TEST_ASSERT_EQUAL(1, output);
}

void
test_pushPeekAnotherValue(void)
{
  uint16_t input = 5;
  pushToBuffer(buffer, &input);
  uint8_t reader = getNewBufferReaderDescriptor(buffer);
  uint16_t output = *((uint16_t*) peekAtBufferWithReader(buffer, reader));
  TEST_ASSERT_EQUAL(input, output);
}

void
test_doNotPeekWithInvalidReader(void)
{
  uint16_t input = 1;
  pushToBuffer(buffer, &input);

  CEXCEPTION_T e;

  Try
  {
    peekAtBufferWithReader(buffer, 0);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_INVALID_READER_EXCEPTION, e);
  }
}

void
test_peekSameValueWithTwoReaders(void)
{
  uint16_t input = 6;
  pushToBuffer(buffer, &input);

  uint8_t reader1 = getNewBufferReaderDescriptor(buffer);
  uint8_t reader2 = getNewBufferReaderDescriptor(buffer);

  TEST_ASSERT_NOT_EQUAL(reader1, reader2);

  uint16_t output1 = *((uint16_t*) peekAtBufferWithReader(buffer, reader1));
  uint16_t output2 = *((uint16_t*) peekAtBufferWithReader(buffer, reader2));

  TEST_ASSERT_EQUAL(output1, output2);
}

void
test_pushPop(void)
{
  uint16_t first_input = 34;
  pushToBuffer(buffer, &first_input);
  uint16_t second_input = 452;
  pushToBuffer(buffer, &second_input);

  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint16_t first_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));
  uint16_t second_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));

  TEST_ASSERT_EQUAL(first_input, first_output);
  TEST_ASSERT_EQUAL(second_input, second_output);
}

void
test_doNotPopWithInvalidReader(void)
{
  uint16_t input = 1;
  pushToBuffer(buffer, &input);

  CEXCEPTION_T e;

  Try
  {
    popFromBufferWithReader(buffer, 0);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_INVALID_READER_EXCEPTION, e);
  }
}

void
test_pushPopWithNewWordSize(void)
{
  initMultiReaderBuffer((MultiReaderBuffer*) buffer, 2*BUFFER_WORD_SIZE, MAX_ELEMENTS/2, MAX_READERS);

  uint32_t first_input = 10430;
  pushToBuffer(buffer, &first_input);
  uint32_t second_input = 645332;
  pushToBuffer(buffer, &second_input);

  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint32_t first_output = *((uint32_t*) popFromBufferWithReader(buffer, reader));
  uint32_t second_output = *((uint32_t*) popFromBufferWithReader(buffer, reader));
  TEST_ASSERT_EQUAL(first_input, first_output);
  TEST_ASSERT_EQUAL(second_input, second_output);
}

void
test_overwriteAllElementsWhenTwiceAsManyElementsHaveBeenWritten(void)
{
  uint16_t first_input = 1;
  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    pushToBuffer(buffer, &first_input);
  }

  uint16_t second_input = 2;
  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    pushToBuffer(buffer, &second_input);
  }

  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    uint16_t second_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));
    TEST_ASSERT_EQUAL(second_input, second_output);
  }
}

void
test_peekThrowOverrunExceptionWhenReaderHasBeenOverwritten(void)
{
  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint16_t input = 1;
  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    pushToBuffer(buffer, &input);
  }

  CEXCEPTION_T e;

  Try
  {
    peekAtBufferWithReader(buffer, reader);
  }
  Catch (e)
  {
    TEST_FAIL_MESSAGE("Exception should not be thrown!");
  }

  pushToBuffer(buffer, &input);

  Try
  {
    peekAtBufferWithReader(buffer, reader);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch (e)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_OVERRUN_EXCEPTION, e);
  }
}

void
test_overwriteTwoElementsOnWraparoundAndReadTheseElementsLast(void)
{
  uint16_t first_input = 1;
  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    pushToBuffer(buffer, &first_input);
  }

  uint16_t second_input = 2, third_input = 3;
  pushToBuffer(buffer, &second_input);
  pushToBuffer(buffer, &third_input);

  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  for (uint8_t times = 0; times < MAX_ELEMENTS-2; times++)
  {
    uint16_t first_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));
    TEST_ASSERT_EQUAL(first_input, first_output);
  }

  uint16_t second_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));
  TEST_ASSERT_EQUAL(second_input, second_output);

  uint16_t third_output = *((uint16_t*) popFromBufferWithReader(buffer, reader));
  TEST_ASSERT_EQUAL(third_input, third_output);
}

void
test_throwUnderrunExceptionWhenMoreElementsPoppedThanPushed(void)
{
  const uint8_t elements_available = 5;

  uint16_t input = 1;
  for (uint8_t times = 0; times < elements_available; times++)
  {
    pushToBuffer(buffer, &input);
  }

  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  for (uint8_t times = 0; times < elements_available; times++)
  {
    popFromBufferWithReader(buffer, reader);
  }

  CEXCEPTION_T e;

  Try
  {
    popFromBufferWithReader(buffer, reader);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch (e)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_UNDERRUN_EXCEPTION, e);
  }
}

void
test_pushPopWithTwoReaders(void)
{
  uint16_t input = 6;
  pushToBuffer(buffer, &input);

  uint8_t reader1 = getNewBufferReaderDescriptor(buffer);
  uint8_t reader2 = getNewBufferReaderDescriptor(buffer);

  uint16_t output1 = *((uint16_t*) popFromBufferWithReader(buffer, reader1));
  uint16_t output2 = *((uint16_t*) popFromBufferWithReader(buffer, reader2));
  
  TEST_ASSERT_EQUAL(output1, output2);
}

void
test_overwriteFirstElementOnWraparoundAndReadThisElementLastWithMoreThanOneReader(void)
{
  uint16_t first_input = 1;
  for (uint8_t times = 0; times < MAX_ELEMENTS; times++)
  {
    pushToBuffer(buffer, &first_input);
  }

  uint16_t second_input = 2;
  pushToBuffer(buffer, &second_input);

  uint8_t reader1 = getNewBufferReaderDescriptor(buffer);
  uint8_t reader2 = getNewBufferReaderDescriptor(buffer);

  for (uint8_t times = 0; times < MAX_ELEMENTS-1; times++)
  {
    uint16_t first_output1 = *((uint16_t*) popFromBufferWithReader(buffer, reader1));
    uint16_t first_output2 = *((uint16_t*) popFromBufferWithReader(buffer, reader2));
    TEST_ASSERT_EQUAL(first_input, first_output1);
    TEST_ASSERT_EQUAL(first_input, first_output2);
  }

  uint16_t second_output1 = *((uint16_t*) popFromBufferWithReader(buffer, reader1));
  uint16_t second_output2 = *((uint16_t*) popFromBufferWithReader(buffer, reader2));
  TEST_ASSERT_EQUAL(second_input, second_output1);
  TEST_ASSERT_EQUAL(second_input, second_output2);
}

void
test_overrunReaderThrowOverrunExceptionButEnablePeekStartingFromOldestElementAfterwards(void)
{
  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint16_t input = 0;

  for (; input < MAX_ELEMENTS + 1; input++) // Element with value 0 will be overwritten
  {
    pushToBuffer(buffer, &input);
  }

  CEXCEPTION_T e1, e2;

  Try
  {
    peekAtBufferWithReader(buffer, reader);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch (e1)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_OVERRUN_EXCEPTION, e1);
    
    Try
    {
      uint16_t output = *((uint16_t*) peekAtBufferWithReader(buffer, reader));
      TEST_ASSERT_EQUAL(1, output);
    }
    Catch (e2)
    {
      TEST_FAIL_MESSAGE("Exception thrown!");
    }
  }
}

void
test_overrunReaderThrowOverrunExceptionButEnablePopStartingFromOldestElementAfterwards(void)
{
  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint16_t input = 0;

  for (; input < MAX_ELEMENTS + 1; input++) // Element with value 0 will be overwritten
  {
    pushToBuffer(buffer, &input);
  }

  CEXCEPTION_T e1, e2;

  Try
  {
    popFromBufferWithReader(buffer, reader);
    TEST_FAIL_MESSAGE("Exception not thrown!");
  }
  Catch (e1)
  {
    TEST_ASSERT_EQUAL_HEX32(BUFFER_OVERRUN_EXCEPTION, e1);
    
    Try
    {
      uint16_t output1 = *((uint16_t*) popFromBufferWithReader(buffer, reader));
      uint16_t output2 = *((uint16_t*) popFromBufferWithReader(buffer, reader));
      TEST_ASSERT_EQUAL(1, output1);
      TEST_ASSERT_EQUAL(2, output2);
    }
    Catch (e2)
    {
      TEST_FAIL_MESSAGE("Exception thrown!");
    }
  }
}

void
test_noReadableItemsExistForInvalidReader(void)
{
  bool readable_items_exist = readableItemExistsForReader(buffer, 0);
  TEST_ASSERT_FALSE(readable_items_exist);
  readable_items_exist = readableItemExistsForReader(buffer, MAX_READERS);
  TEST_ASSERT_FALSE(readable_items_exist);
}

void
test_readableItemsExistForReaderWhenSomethingHasBeenWritten(void)
{
  uint8_t reader = getNewBufferReaderDescriptor(buffer);

  uint16_t input = 2;
  pushToBuffer(buffer, &input);

  bool readable_items_exist = readableItemExistsForReader(buffer, reader);
  TEST_ASSERT_TRUE(readable_items_exist);

  popFromBufferWithReader(buffer, reader);

  readable_items_exist = readableItemExistsForReader(buffer, reader);
  TEST_ASSERT_FALSE(readable_items_exist);
}

void
test_noReadableItemsExistForReaderWhenNothingHasBeenWritten(void)
{
  uint8_t reader = getNewBufferReaderDescriptor(buffer);
  bool readable_items_exist = readableItemExistsForReader(buffer, reader);
  TEST_ASSERT_FALSE(readable_items_exist);
}

