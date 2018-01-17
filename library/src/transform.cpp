/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/



#include <vector>
#include <assert.h>
#include <iostream>

#include "rocfft.h"
#include "plan.h"
#include "repo.h"
#include "transform.h"


rocfft_status rocfft_execution_info_create( rocfft_execution_info *info )
{
	rocfft_execution_info einfo = new rocfft_execution_info_t;
	*info = einfo;

	return rocfft_status_success;
}

rocfft_status rocfft_execution_info_destroy( rocfft_execution_info info )
{
	if(info != nullptr)
		delete info;

	return rocfft_status_success;
}

rocfft_status rocfft_execution_info_set_work_buffer( rocfft_execution_info info, void *work_buffer, size_t work_buffer_size )
{
	info->workBufferSize = work_buffer_size;
	info->workBuffer = work_buffer;

	return rocfft_status_success;
}

rocfft_status rocfft_execution_info_set_stream( rocfft_execution_info info, void *stream )
{
    info->rocfft_stream = (hipStream_t)stream;
	return rocfft_status_success;
}

rocfft_status rocfft_execute(   const rocfft_plan plan,
                                void *in_buffer[],
                                void *out_buffer[],
                                rocfft_execution_info info )
{

	Repo &repo = Repo::GetRepo();
	ExecPlan execPlan;
	repo.GetPlan(plan, execPlan);

#ifdef DEBUG
	PrintNode(execPlan);
#endif
	if(execPlan.workBufSize > 0)
	{
		assert(info != nullptr);
		assert(info->workBufferSize >= (execPlan.workBufSize * 2 * plan->base_type_size ));
	}

	if(plan->placement == rocfft_placement_inplace)
		TransformPowX(execPlan, in_buffer, in_buffer, info);
	else
		TransformPowX(execPlan, in_buffer, out_buffer, info);

	return rocfft_status_success;
}



