/*
 * Copyright (c) 2015, 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY LOG OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <memkind.h>
#include <memkind/internal/memkind_pmem.h>
#include <jemalloc/jemalloc.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static const size_t PMEM_PART_SIZE = MEMKIND_PMEM_MIN_SIZE + 4096;
static const char*  PMEM_DIR = "/mnt/pmem/";
//static struct memkind *pmem_kind;

int
main(int argc, char *argv[])
{
    memkind_t pmem_kind;

    for ( int i=0;i<250000;i++)
    {
//        // create PMEM partition
        int err = memkind_create_pmem(PMEM_DIR, PMEM_PART_SIZE, &pmem_kind);
        if (err) {
        perror("memkind_create_pmem()");
        fprintf(stderr, "Unable to create pmem partition\n");
        }
        char* default_str = (char *)memkind_malloc(pmem_kind, 1025);
        memkind_free(pmem_kind,default_str);
        (void)default_str;
        err = memkind_destroy_kind(pmem_kind); 
        if (err) {
            perror("memkind_destroy_kind()");
            fprintf(stderr, "Unable to destroy pmem partition\n");
        }
        if(i %100==0)
        {
            fprintf(stderr, "\nvalue of i %d",i);
        }/*
    void *test = jemk_malloc (8000);
    
    void *test_2 = jemk_malloc (0x40000);
    
    jemk_free (test);
    
    jemk_free (test_2);*/
    }


//    const size_t size = 1024;
//    char *default_str = NULL;
//    const size_t num = 1;

//    SetUp(pmem_kind);
    
//    TearDown(pmem_kind);
    
//    SetUp(pmem_kind);
    
//    default_str = (char *)memkind_malloc(pmem_kind, size);
//    if (default_str==NULL)
//    {
//        perror("malloc_failed()");
//    }
    
//    sprintf(default_str, "memkind_malloc MEMKIND_PMEM\n");
//    printf("%s", default_str);
    
//        memkind_free(pmem_kind, default_str);

//   // Out of memory
//    default_str = (char *)memkind_malloc(pmem_kind, 2 * PMEM_PART_SIZE);
    
//    if (default_str)
//    {
//        perror("malloc should failed()");
//    }
    
//    TearDown(pmem_kind);
    
//    SetUp(pmem_kind);
    
//    default_str = (char *)memkind_calloc(pmem_kind, num, size);
//    if (!default_str)
//    {
//        perror("calloc failed()");
//    }

//    sprintf(default_str, "memkind_calloc MEMKIND_PMEM\n");
//    printf("%s", default_str);

//    memkind_free(pmem_kind, default_str);

//    // allocate the buffer of the same size (likely at the same address)
//    default_str = (char *)memkind_calloc(pmem_kind, num, size);
//    if (!default_str)
//    {
//        perror("second calloc failed()");
//    }

//    sprintf(default_str, "memkind_calloc MEMKIND_PMEM\n");
//    printf("%s", default_str);

//    memkind_free(pmem_kind, default_str);

//    TearDown(pmem_kind);
    
//    SetUp(pmem_kind);
    
//    const size_t big_size = MEMKIND_PMEM_CHUNK_SIZE;

//    default_str = (char *)memkind_calloc(pmem_kind, num, big_size);
//    if (!default_str)
//    {
//        perror("big calloc failed()");
//    }

//    sprintf(default_str, "memkind_calloc MEMKIND_PMEM\n");
//    printf("%s", default_str);

//    memkind_free(pmem_kind, default_str);

//    // allocate the buffer of the same size (likely at the same address)
//    default_str = (char *)memkind_calloc(pmem_kind, num, big_size);
//    default_str = (char *)memkind_calloc(pmem_kind, num, size);
//    if (!default_str)
//    {
//        perror("second big calloc failed()");
//    }
//    sprintf(default_str, "memkind_calloc MEMKIND_PMEM\n");
//    printf("%s", default_str);

//    memkind_free(pmem_kind, default_str);

//    TearDown(pmem_kind);
    
//    SetUp(pmem_kind);
//    //realloc

//     const size_t size1 = 512;
//    const size_t size2 = 1024;
    
//    default_str = (char *)memkind_realloc(pmem_kind, default_str, size1);
//    if (!default_str)
//    {
//        perror("realloc  failed()");
//    }

//    sprintf(default_str, "memkind_realloc MEMKIND_PMEM with size %zu\n", size1);
//    printf("%s", default_str);

//    default_str = (char *)memkind_realloc(pmem_kind, default_str, size2);

//    if (!default_str)
//    {
//        perror("realloc  failed()");
//    }
    
//    sprintf(default_str, "memkind_realloc MEMKIND_PMEM with size %zu\n", size2);
//    printf("%s", default_str);

//    memkind_free(pmem_kind, default_str);

    return 0;
}
