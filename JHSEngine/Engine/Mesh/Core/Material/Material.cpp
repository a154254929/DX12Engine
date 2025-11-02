#include "Material.h"

CMaterial::CMaterial()
	: baseColor(.5f, .5f, .5f, 1.f)
	, roughness(.2f)
	, materialType(EMaterialType::Lambertain)
{
}
