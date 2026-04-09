/** 
    @file compressTest.c
    @author 
    Unit tests for the compress.c component.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compress.h"

/** Total number or tests we tried. */
static int totalTests = 0;

/** Number of test cases passed. */
static int passedTests = 0;

/** Macro to terminate the program when an error occurs in the test
    code (rather than the component being tested). */
#define FAIL( msg ) {\
  fprintf( stderr, "%s\n", msg );\
  exit( EXIT_FAILURE );          \
}

/** Macro to check the condition on a test case, keep counts of
    passed/failed tests and report a message if the test fails. */
#define TestCase( conditional ) {\
  totalTests += 1; \
  if ( conditional ) { \
    passedTests += 1; \
  } else { \
    printf( "**** Failed unit test on line %d of %s\n", __LINE__, __FILE__ );    \
  } \
}

/** 
    For comparing an array of bytes.
    @param seqA One sequence of bytes to compare.
    @param seqB Another sequence of bytes to compre against seqA.
    @param len Number of bytes to compare.
    @return True if the sequences are identical.
*/
bool compareBytes( void const *seqA, void const *seqB, int len )
{
  for ( int i = 0; i < len; i++ )
    if ( *( (unsigned char const *)seqA + i ) != *( (unsigned char const *)seqB + i ) )
      return false;
  return true;
}

/**
   Top-level function for compressTest.c the unit test.
   @return exit status of the program, success if all tests pass.
*/
int main()
{
  // As you finish parts of your implementation, move this directive
  // down past the blocks of code below.  That will enable tests of
  // various functions you're expected to implement.
  

  
  ////////////////////////////////////////////////////////////
  // Test(s) for the serializeBlock() function.
  
  {
    // A block containing two rules.
    Block block = { .data = { 0x41, 0x41, 0x41, 0x42, 0x42, 0x42 },
      .len = 6,
      .rlist = { { .code = 0x41, .first = 0x61, .second = 0x62 },
                 { .code = 0x42, .first = 0x63, .second = 0x64 } },
      .rcount = 2 };
                 
    byte expected[] = {
      0x06,
      0x00, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42,
      0x02,
      0x41, 0x61, 0x62,
      0x42, 0x63, 0x64
    };

    Buffer *dest = makeBuffer();
    serializeBlock( &block, dest );

    // Check the length of the result.
    TestCase( dest->len == sizeof( expected ) );
    
    // Check the contents of the serialized block.
    TestCase( compareBytes( dest->data, expected, sizeof( expected ) ) );

    freeBuffer( dest );
  }


  ////////////////////////////////////////////////////////////
  // Test(s) for the deserializeBlock() function.
  {
    byte enc[] = {
      0x03, 0x00,
      0x10, 0x11, 0x12,
      0x01,
      0x40, 0x20, 0x30
    };
    Buffer *buf = makeBuffer();
    appendBytes(buf, enc, sizeof(enc));
    Block block;
    TestCase( deserializeBlock(&block, buf) );
    TestCase( block.len == 3 );
    TestCase( block.data[0] == 0x10 && block.data[1] == 0x11 && block.data[2] == 0x12 );
    TestCase( block.rcount == 1 );
    TestCase( block.rlist[0].code == 0x40 );
    TestCase( block.rlist[0].first == 0x20 );
    TestCase( block.rlist[0].second == 0x30 );
    freeBuffer(buf);
  
    buf = makeBuffer();
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x02);
    appendByte(buf, 0x00);
    appendByte(buf, 0xAA);
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x01);
    appendByte(buf, 0x00);
    appendByte(buf, 0xBB);
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x01);
    appendByte(buf, 0x00);
    appendByte(buf, 0xCC);
    appendByte(buf, 0x01);
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x01);
    appendByte(buf, 0x00);
    appendByte(buf, 0xDD);
    appendByte(buf, 0x01);
    appendByte(buf, 0xEE);
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x01);
    appendByte(buf, 0x00);
    appendByte(buf, 0xFF);
    appendByte(buf, 0x01);
    appendByte(buf, 0x11);
    appendByte(buf, 0x22);
    TestCase(deserializeBlock(&block, buf) == false);
    freeBuffer(buf);
    buf = makeBuffer();
    appendByte(buf, 0x01); 
    appendByte(buf, 0x00);
    appendByte(buf, 0x99); 
    appendByte(buf, 0x00);
    TestCase(deserializeBlock(&block, buf) == true);
    TestCase(block.len == 1 && block.data[0] == 0x99 && block.rcount == 0);
    freeBuffer(buf);
   
  }
  ////////////////////////////////////////////////////////////
  // Test(s) for the compressBlock() function.
  {
    // This is the same example block from the explanation of byte-pair encoding.
    Block block = { .data = {
        'C', 'D', 'A', 'B', 'A', 'B', 'A', 'B', 'C', 'D', 'A', 'B',
        'C', 'D', 'A', 'B', 'C', 'D' },
      .len = 18,
      .rcount = 0 };
    byte expected[] = {
      0x01, 0x00, 0x00, 0x02, 0x02, 0x02
    };
    compressBlock( &block );
    // Make sure the block compresses to the right sequence of bytes.
    TestCase( block.len == 6 );
    TestCase( compareBytes( block.data, expected, sizeof( expected ) ) );
    // Check the rules used to compress the block.
    TestCase( block.rcount == 3 );
    TestCase( block.rlist[ 0 ].code == 0x00 && block.rlist[ 0 ].first == 'A' &&
              block.rlist[ 0 ].second == 'B' );
    TestCase( block.rlist[ 1 ].code == 0x01 && block.rlist[ 1 ].first == 'C' &&
              block.rlist[ 1 ].second == 'D' );
    TestCase( block.rlist[ 2 ].code == 0x02 && block.rlist[ 2 ].first == 0x00 &&
              block.rlist[ 2 ].second == 0x01 );
    Buffer *result = compressData(NULL);
    TestCase(result == NULL);
    Block block2;
    block2.len = 4;
    block2.rcount = 0;
    for(int i = 0; i < 4; i++) {
      block2.data[i] = (byte)i;
    }
    for(int i = 0; i < 256; i++) {
      block2.rlist[i].code = (byte)i;
    }
    compressBlock(&block2); 
    TestCase(block2.len == 4);
  }
  ////////////////////////////////////////////////////////////
  // Test(s) for the uncompressBlock() function.
  {
    Block block = {
      .data = { 0x01, 0x00, 0x00, 0x02, 0x02, 0x02 },
      .len = 6,
      .rcount = 3,
      .rlist = {
        { .code = 0x00, .first = 'A', .second = 'B' },
        { .code = 0x01, .first = 'C', .second = 'D' },
        { .code = 0x02, .first = 0x00, .second = 0x01 }
      }
    };

    byte expected[] = {
      'C','D','A','B','A','B','A','B','C','D','A','B',
      'C','D','A','B','C','D'
    };
    uncompressBlock(&block);
    TestCase( compareBytes( block.data, expected, sizeof( expected ) ) );
    TestCase( block.rcount == 3 );
    TestCase( block.rlist[ 0 ].code == 0x00 && block.rlist[ 0 ].first == 'A' &&
              block.rlist[ 0 ].second == 'B' );
    TestCase( block.rlist[ 1 ].code == 0x01 && block.rlist[ 1 ].first == 'C' &&
              block.rlist[ 1 ].second == 'D' );
    TestCase( block.rlist[ 2 ].code == 0x02 && block.rlist[ 2 ].first == 0x00 &&
              block.rlist[ 2 ].second == 0x01 );
    TestCase( block.len == sizeof(expected) );
    TestCase( compareBytes(block.data, expected, sizeof(expected)) );
    Buffer *result = uncompressData(NULL);
    TestCase(result == NULL);
    Block block1 = { .len = BLOCK_SIZE_LIMIT * 2, .rcount = 1 };
    block1.data[0] = 0x01;
    block1.rlist[0].code = 0x01;
    block1.rlist[0].first = 'A';
    block1.rlist[0].second = 'B';
    TestCase(uncompressBlock(&block1) == false);
    Block block3 = { .len = BLOCK_SIZE_LIMIT, .rcount = 1 };
    block3.data[0] = 0x01;
    block3.rlist[0].code = 0x01;
    block3.rlist[0].first = 'A';
    block3.rlist[0].second = 'B';
    TestCase(uncompressBlock(&block3) == false);
    Block block4 = { .data = {0x01}, .len = 1, .rcount = 1 };
    block4.rlist[0].code = 0x01;
    block4.rlist[0].first = 'A';
    block4.rlist[0].second = 'B';
    TestCase(uncompressBlock(&block4) == true);
    TestCase(block4.len == 2);
    TestCase(block4.data[0] == 'A' && block4.data[1] == 'B');

  }
  
 
  ////////////////////////////////////////////////////////////
  // Test(s) for the compressData() function.
  
  {
    // Test based on the example from the Philip Gage paper (with
    // no null terminator at the end).
    
    byte srcBytes[ 9 ] = "ABABCABCD";
    Buffer *src = makeBuffer();
    appendBytes( src, srcBytes, sizeof( srcBytes ) );
    
    byte expected[] = {
      0x06, 0x00,                           // Block size
      0x00, 0x00, 0x43, 0x00, 0x43, 0x44,   // Block contents
      0x01,                                 // Number of rules
      0x00, 0x41, 0x42                      // Replacement rule
    };

    Buffer *dest = compressData( src );
    TestCase( dest != NULL );

    // Check the size then contents of the results.
    TestCase( dest->len == sizeof( expected ) );
    TestCase( compareBytes( dest->data, expected, sizeof( expected ) ) );

    freeBuffer( src );
    freeBuffer( dest );

    
  }

  {
    // This 16-byte input should make three replaceent rules.
    byte srcBytes[ 16 ] = "AAAAAAAAAAAAAAAA";
    Buffer *src = makeBuffer();
    appendBytes( src, srcBytes, sizeof( srcBytes ) );
    
    byte expected[] = {
      0x02, 0x00,
      0x02, 0x02,
      0x03,
      0x00, 0x41, 0x41,
      0x01, 0x00, 0x00,
      0x02, 0x01, 0x01
    };

    Buffer *dest = compressData( src );
    TestCase( dest != NULL );

    // Check size and then contents of the result.
    TestCase( dest->len == sizeof( expected ) );
    TestCase( compareBytes( dest->data, expected, sizeof( expected ) ) );

    freeBuffer( src );
    freeBuffer( dest );
  }

  {
    // Maximum possible block size, filled with zeros.
    byte srcBytes[ 16384 ];
    for ( int i = 0; i < sizeof( srcBytes ); i++ )
      srcBytes[ i ] = 0;
    Buffer *src = makeBuffer();
    appendBytes( src, srcBytes, sizeof( srcBytes ) );
    
    byte expected[] = {
      0x02, 0x00,
      0x0D, 0x0D,
      0x0D,
      0x01, 0x00, 0x00,
      0x02, 0x01, 0x01,
      0x03, 0x02, 0x02,
      0x04, 0x03, 0x03,
      0x05, 0x04, 0x04,
      0x06, 0x05, 0x05,
      0x07, 0x06, 0x06,
      0x08, 0x07, 0x07,
      0x09, 0x08, 0x08,
      0x0A, 0x09, 0x09,
      0x0B, 0x0A, 0x0A,
      0x0C, 0x0B, 0x0B,
      0x0D, 0x0C, 0x0C
    };

    Buffer *dest = compressData( src );
    TestCase( dest != NULL );

    // Check size and then contents of the result.
    TestCase( dest->len == sizeof( expected ) );
    TestCase( compareBytes( dest->data, expected, sizeof( expected ) ) );

    freeBuffer( src );
    freeBuffer( dest );
  }
  
  
  
  ////////////////////////////////////////////////////////////
  // Test(s) for the uncompressData() function.
  {
    
    byte enc[] = {
      0x06, 0x00,                        
      0x00, 0x00, 0x43, 0x00, 0x43, 0x44,
      0x01,                               
      0x00, 0x41, 0x42                   
    };
    Buffer *src = makeBuffer();
    appendBytes(src, enc, sizeof(enc));
    Buffer *block = uncompressData(src);
    byte expected[] = "ABABCABCD";
    TestCase( block != NULL );
    TestCase( compareBytes(block->data, expected, sizeof(expected)) );
    freeBuffer(src);
    freeBuffer(block);
    Buffer *block1 = uncompressData(NULL);
    TestCase(block1 == NULL);
    Buffer *src1 = makeBuffer();
    byte badData[] = { 0xFF };
    appendBytes(src1, badData, sizeof(badData));
    Buffer *block2 = uncompressData(src1);
    TestCase(block2 == NULL);
    freeBuffer(src1);
    Buffer *src2 = makeBuffer();
    byte enc2[] = {
        0x01, 0x00,
        0x01,
        0x01,   
        0x01, 'A', 'B'
    };
    appendBytes(src2, enc2, sizeof(enc2));
    Buffer *block3 = uncompressData(src2);
    TestCase(block3 != NULL);
    TestCase(block3->len == 2);
    TestCase(block3->data[0] == 'A' && block3->data[1] == 'B');
    freeBuffer(src2);
    freeBuffer(block3);
  }
  
  // Once you move the #ifdef DISABLE_TESTS to here, you've enabled
  // all the tests.
#ifdef DISABLE_TESTS
#endif

  // Report how many tests are passing.
  printf( "*** Passed %d of %d unit tests for compress.c.\n", passedTests,
          totalTests );

  // Exit successfully if all tests tests were passed.
  if ( passedTests == totalTests )
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}

