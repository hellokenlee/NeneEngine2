/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "core_render/static_mesh_render_data.h"


namespace nene::g
{
	/** the asset data of static mesh 
	 * 
	 * @see
	 *		GameThread: `staic_mesh_component`: owned by entity, refferences an static mesh asset by handle
	 *		Asset: `static_mesh_asset`: owned by asset manager, the vertices data in game-thread 
	 *		RenderThread: `static_mesh_render_data: owend by asset, the render thread proxy of an static mesh 
	 */
	class static_mesh_asset : public asset
	{
	public:
		
		
	protected:
		std::unique_ptr<r::static_mesh_lod_render_data> m_render_data;
	};
}
