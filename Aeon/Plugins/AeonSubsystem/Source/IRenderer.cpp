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
#include <Aeon/Subsystem/IRenderer.h>

namespace Aeon
{

	SubsystemType Aeon::IRenderer::GetSubsystemType() const 
	{
		return SUBSYS_RENDERER;
	}

}

