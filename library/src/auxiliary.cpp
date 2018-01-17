/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/




#if defined(__NVCC__)
#include "helper_math.h"
#endif

#include "rocfft_hip.h"
#include "rocfft.h"


// library setup function, called once in program at the start of library use
rocfft_status rocfft_setup()
{
	return rocfft_status_success;
}

// library cleanup function, called once in program after end of library use
rocfft_status rocfft_cleanup()
{
	return rocfft_status_success;
}
