/**
* This is the compress class. It serializes, deserializes, compresses, and decompresses blocks and data
* @file compress.c
* @author Jacob McLain 
*/

#include "compress.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Max amount of replaces */
#define REPLACEMENT_THRESHOLD 3

/**
* This function writes out the contents of the given
* block by adding it to the end of the given buffer.
* Compressing a file could require multiple blocks,
* so this function should add the compressed representation
* of block to the end of the buffer, after anything that was previously in the buffer.
* @param block the block
* @param buf the buffer
*/
void serializeBlock(Block *block, Buffer *buf) {
    appendByte(buf, (byte)(block->len & 0xFF));
    appendByte(buf, (byte)((block->len >> 8) & 0xFF));
    appendBytes(buf, block->data, block->len);
    appendByte(buf, (byte)block->rcount);

    for(int i = 0; i < block->rcount; i++) {
        appendByte(buf, block->rlist[i].code);
        appendByte(buf, block->rlist[i].first);
        appendByte(buf, block->rlist[i].second);
    }
}
/**
* This function reads the contents of a block from
* the given buffer. If successful, it should return
* true. If the given buffer doesn’t contain a valid 
* block (e.g., if the end of the buffer is reached 
* before an entire block can be read), it should return
* false. This function should use the extractByte() and
* extractBytes() functions to access the contents of the
* buffer consecutively. A file might be represented as
* multiple compressed blocks, so this function should
* continue processing the contents of the buffer, picking
* up from where a previous call to deserializeBlock() may have left off.
* @param block the block
* @param buf the buffer
* @return T or F based on if the block is deserialized
*/
bool deserializeBlock(Block *block, Buffer *buf) {
    byte low;
    byte high;
    if(!extractByte(buf, &low)) {
        return false;
    }

    if(!extractByte(buf, &high)) {
        return false;
    }

    block->len = (low | (high << 8));
    if(block->len > BLOCK_SIZE_LIMIT) {
        return false;
    }

    for(int i = 0; i < block->len; i++) {
        if(!extractByte(buf, &block->data[i])) {
            return false;
        }
    }

    if(!extractByte(buf, &block->rcount)) {
        return false;
    }

    if(block->rcount > BYTE_CODES - 1) {
        return false;
    } 

    for(int i = 0; i < block->rcount; i++) {
        if(!extractByte(buf, &block->rlist[i].code)) {
            return false;
        }
        if(!extractByte(buf, &block->rlist[i].first)) {
            return false;
        }
        if(!extractByte(buf, &block->rlist[i].second)) {
            return false;
        }
    }

    return true;
    
}
/**
* This function attempts to compress the contents
* of the given block using the technique described 
* under “Block Compression” above. It should repeatedly
* select the most common byte pair in the block and 
* replace it with the lowest-numbered available byte,
* until there are no more available bytes or the most 
* common byte pair has fewer than three occurrences.
* @param block the block
*/
void compressBlock(Block *block) {
    if(!block) {
        return;
    }

    block->rcount = 0;

    bool used[BYTE_CODES] = { false };

    for(int i = 0; i < block->len; i++) {
        used[block->data[i]] = true;
    }

    byte available[BYTE_CODES];
    int availCount = 0;

    for(int i = 0; i < BYTE_CODES; i++) {
        if (!used[i]) {
            available[availCount++] = (byte)i;
        }
    }

    while(availCount > 0) {
        int *pairCounts = calloc(BYTE_CODES * BYTE_CODES, sizeof(int));
        if(!pairCounts) {
            break;
        }
        
        for(int i = 0; i < block->len - 1; i++) {
            unsigned char a = block->data[i];
            unsigned char b = block->data[i + 1];
            pairCounts[a * BYTE_CODES + b]++;
        }

        int maxCount = REPLACEMENT_THRESHOLD - 1;
        byte commonA = 0;
        byte commonB = 0;

        for(int i = 0; i < BYTE_CODES; i++) {
            for(int j = 0; j < BYTE_CODES; j++) {
                if(pairCounts[i * BYTE_CODES + j] > maxCount) {
                    maxCount = pairCounts[i * BYTE_CODES + j];
                    commonA = (byte)i;
                    commonB = (byte)j;
                }
            }
        }

        free(pairCounts);

        if(maxCount < REPLACEMENT_THRESHOLD) {
            break;
        }

        byte code = available[0];

        for(int k = 1; k < availCount; k++) {
            available[k - 1] = available[k];
        }

        availCount--;

        int newLen = 0;

        for(int i = 0; i < block->len; i++) {
            if(i < block->len - 1 && block->data[i] == commonA && block->data[i + 1] == commonB) {
                block->data[newLen++] = code;
                i++;
            } 
            else {
                block->data[newLen++] = block->data[i];
            }
        }

        block->len = newLen;

        block->rlist[block->rcount].code = code;
        block->rlist[block->rcount].first = commonA;
        block->rlist[block->rcount].second = commonB;
        block->rcount++;

    }
}



/**
* This function reverses the compressBlock operation,
* returning true if successful or false if there’s an
* error in how the block is compressed (e.g., if the 
* block would be too large after decompression). This
* function should undo each byte pair replacement in 
* the block’s rule list, working from the end of the 
* list to the start.
* @param block the block
* @return T or F based on if the block was uncompressed
*/
bool uncompressBlock(Block *block) {

    for(int i = block->rcount - 1; i >= 0; i--) {

        Replacement rule = block->rlist[i];
        int newLen = 0;
        byte temp[BLOCK_SIZE_LIMIT];

        for(int j = 0; j < block->len; j++) {
            if (block->data[j] == rule.code) {
                if(newLen + 2 > BLOCK_SIZE_LIMIT) {
                    return false;
                }

                temp[newLen++] = rule.first;
                temp[newLen++] = rule.second;
                
            } 
            else {
                if(newLen + 1 > BLOCK_SIZE_LIMIT) {
                    return false;
                } 

                temp[newLen++] = block->data[j];

            }
        }

        memcpy(block->data, temp, newLen);
        block->len = newLen;

    }
    return true;
}
/**
* This function compresses a file stored in the given
* src buffer, returning a new buffer containing a 
* compressed representation of the file. The file
* may need to be compressed as more than one buffer.
* If so, then the returned buffer will contain 
* encodings of each block, stored consecutively.
* @param src the buffer
* @return The buffer the data was compressed into 
*/
Buffer *compressData(Buffer *src) {
    if(!src) {
        return NULL;
    }

    Buffer *dest = makeBuffer();

    if(!dest) {
        return NULL;
    }

    src->pos = 0;
    byte carry = 0;
    bool hasCarry = false;

    while(src->pos < src->len || hasCarry) {

        Block block;
        block.len = 0;
        block.rcount = 0;
        bool seen[BYTE_CODES] = { false };
        int uniqueCount = 0;

        if(hasCarry) {
            block.data[block.len++] = carry;
            seen[carry] = true;
            uniqueCount = 1;
            hasCarry = false;
        }

        while(block.len < BLOCK_SIZE_LIMIT && src->pos < src->len) {

            byte b = src->data[src->pos];

            if(!seen[b] && uniqueCount == 224) {
                carry = b;
                hasCarry = true;
                src->pos++;
                break;
            }

            if(!seen[b]) {
                seen[b] = true;
                uniqueCount++;
            }

            block.data[block.len++] = b;
            src->pos++;

        }

        compressBlock(&block);
        serializeBlock(&block, dest);

    }

    return dest;
}
/**
* This function uncompresses the contents of the given
* source buffer and returns it in a new destination 
* buffer. If there is an error decompressing the
* given buffer, it returns NULL.
* @param src the buffer
* @return the buffer that the data was uncompressed from
*/
Buffer *uncompressData(Buffer *src) {

    if(!src) {
        return NULL;
    }

    Buffer *dest = makeBuffer();

    if(!dest) {
        return NULL; 
    } 

    src->pos = 0;

    while(src->pos < src->len) {
        Block block;
        if(!deserializeBlock(&block, src)) { 
            freeBuffer(dest); 
            return NULL;
        }
        if(!uncompressBlock(&block)) { 
            freeBuffer(dest);
            return NULL;
        }
        appendBytes(dest, block.data, block.len);
    }

    return dest;
}
