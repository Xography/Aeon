/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#ifndef ICompressionAlgorithm_h__
#define ICompressionAlgorithm_h__

#include <Aeon/Compress/Export.h>

namespace Aeon {
	namespace Compress {
		/**
		* Interface for compression algorithms.
		*/
		class AEON_COMPRESS_EXPORT ICompressionAlgorithm
		{
		public:
			virtual ~ICompressionAlgorithm()
			{

			}
		};
	}
}

#endif // ICompressionAlgorithm_h__
