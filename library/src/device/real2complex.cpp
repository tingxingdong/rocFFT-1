/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/

#include <iostream>
#include "rocfft.h"
#include "rocfft_hip.h"
#include "kernel_launch.h"
#include "./kernels/common.h"

template<typename T>
__global__
void real2complex_kernel(hipLaunchParm lp, size_t input_size, real_type_t<T> *input, size_t input_distance, T *output, size_t output_distance)
{
    size_t tid = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;
    
    size_t input_offset = hipBlockIdx_y * input_distance;

    size_t output_offset = hipBlockIdx_y * output_distance;

    input += input_offset;
    output += output_offset;

    if( tid < input_size)
    {
        output[tid].y = 0.0;
        output[tid].x = input[tid];
    }
}


/*! \brief auxiliary function

    convert a real vector into a complex one by padding the imaginary part with 0.

    @param[in]
    input_size 
           size of input buffer

    @param[in]
    input_buffer
          data type : float or double 

    @param[in]
    input_distance 
          distance between consecutive batch members for input buffer

    @param[in,output]
    output_buffer
          data type : complex type (float2 or double2)

    @param[in]
    output_distance 
           distance between consecutive batch members for output buffer

    @param[in]
    batch 
           number of transforms

    @param[in]
    precision 
          data type of input buffer. rocfft_precision_single or rocfft_precsion_double

    ********************************************************************/

void real2complex(const void *data_p, void *back_p) 
{
    DeviceCallIn *data = (DeviceCallIn *)data_p;

    size_t input_size = 1;

    for(size_t i=0; i<data->node->length.size();i++){
        input_size *= data->node->length[i];//flat the dimension to 1D
    }

    if(input_size == 1) return;
    
    size_t input_distance = data->node->iDist;
    size_t output_distance = data->node->oDist;
    //printf("input_size=%zu, input_distance=%zu, output_distance=%zu\n", input_size, input_distance, output_distance);

    void* input_buffer = data->bufIn[0];
    void* output_buffer = data->bufOut[0];

    size_t batch = data->node->batch;
    rocfft_precision precision = data->node->precision;
    
    size_t blocks = (input_size-1)/512 + 1;

    dim3 grid(blocks, batch, 1);//the second dimension is used for batching 
    dim3 threads(512, 1, 1);//use 512 threads (work items)

    hipStream_t rocfft_stream = data->rocfft_stream; 

    if(precision == rocfft_precision_single) 
        hipLaunchKernel( real2complex_kernel<float2>, grid, threads, 0, rocfft_stream, input_size, (float *)input_buffer, input_distance, (float2 *)output_buffer, output_distance);  
    else 
        hipLaunchKernel( real2complex_kernel<double2>, grid, threads, 0, rocfft_stream, input_size, (double *)input_buffer, input_distance, (double2 *)output_buffer,
output_distance);http://www.mitbbs.com/ym_article/ImmigrationNews/31219615.html

    return;    
}


/*============================================================================================*/


template<typename T>
__global__
void complex2hermitian_kernel(hipLaunchParm lp, size_t input_size, T *input, size_t input_distance, T *output, size_t output_distance)
{
    
    size_t input_offset = hipBlockIdx_z * input_distance;//batch offset

    size_t output_offset = hipBlockIdx_z * output_distance;//batch

    size_t output_size = input_size/2 + 1;

    input_offset += hipBlockIdx_y * input_size;//notice for 1D, hipBlockIdx_y == 0 and thus has no effect for input_offset
    output_offset += hipBlockIdx_y * output_size;//notice for 1D, hipBlockIdx_y == 0 and thus has no effect for output_offset

    input += input_offset;
    output += output_offset;

    size_t tid = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;

    if( tid < output_size)//only read and write the first [input_size/2+1] elements due to conjugate redundancy
    {
        output[tid] = input[tid];
    }
}


/*! \brief auxiliary function

    read from input_buffer and store the first  [1 + input_size/2] elements to the output_buffer

    @param[in]
    input_size 
           size of input buffer

    @param[in]
    input_buffer
          data type dictated by precision parameter but complex type (float2 or double2)

    @param[in]
    input_distance 
           distance between consecutive batch members for input buffer

    @param[in,output]
    output_buffer
          data type dictated by precision parameter but complex type (float2 or double2)
          but only store first [1 + input_size/2] elements according to conjugate symmetry

    @param[in]
    output_distance 
           distance between consecutive batch members for output buffer

    @param[in]
    batch 
           number of transforms

    @param[in]
    precision 
           data type of input and output buffer. rocfft_precision_single or rocfft_precsion_double

    ********************************************************************/

void complex2hermitian(const void *data_p, void *back_p) 
{
    DeviceCallIn *data = (DeviceCallIn *)data_p;

    size_t input_size = data->node->length[0];//input_size is the innermost dimension
    
    size_t input_distance = data->node->iDist;
    size_t output_distance = data->node->oDist;

    void* input_buffer = data->bufIn[0];
    void* output_buffer = data->bufOut[0];

    printf("hermitian input_distance = %zu, output_distance = %zu\n", input_distance, output_distance);//TODO bug for batch > 1, oDist must be N/2+1, instead of N

    size_t batch = data->node->batch;
    size_t high_dimension = 1;
    if(data->node->length.size() > 1)
    {
        for(int i=1; i<data->node->length.size(); i++)
        { 
            high_dimension *= data->node->length[i];
        }
    }
    rocfft_precision precision = data->node->precision;
    
    size_t blocks = (input_size-1)/512 + 1;

    if(high_dimension > 65535 || batch > 65535 ) printf("2D and 3D or batch is too big; not implemented\n");
    //the z dimension is used for batching, 
    //if 2D or 3D, the number of blocks along y will multiple high dimensions 
    //notice the maximum # of thread blocks in y & z is 65535 according to HIP && CUDA
    dim3 grid(blocks, high_dimension, batch);
    dim3 threads(512, 1, 1);//use 512 threads (work items)

    hipStream_t rocfft_stream = data->rocfft_stream; 
/*
    float2* tmp; tmp = (float2*)malloc(sizeof(float2)*input_distance*batch);
    hipMemcpy(tmp, input_buffer, sizeof(float2)*input_distance*batch, hipMemcpyDeviceToHost);

    for(size_t j=0;j<data->node->length[1]; j++)
    {
        for(size_t i=0; i<data->node->length[0]; i++)
        { 
            printf("kernel output[%zu][%zu]=(%f, %f) \n", i, j, tmp[j*data->node->length[0]+i].x, tmp[j*data->node->length[0]+i].x);
        }
    }
*/
    if(precision == rocfft_precision_single) 
        hipLaunchKernel( complex2hermitian_kernel<float2>, grid, threads, 0, rocfft_stream, input_size, (float2 *)input_buffer, input_distance, (float2 *)output_buffer, output_distance);  
    else 
        hipLaunchKernel( complex2hermitian_kernel<double2>, grid, threads, 0, rocfft_stream, input_size, (double2 *)input_buffer, input_distance, (double2 *)output_buffer, output_distance);  

    return;    
}




