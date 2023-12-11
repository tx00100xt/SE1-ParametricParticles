#include <Engine/Engine.h>

/* rcg10042001 protect against Visual C-isms. */
#ifdef _MSC_VER
#define DECL_DLL __declspec(dllimport)
#include "EntitiesMP/Global.h"
#else
#define DECL_DLL
#endif
#include "EntitiesMP/Global.h"

#undef DECL_DLL
#ifdef _MSC_VER
#define DECL_DLL __declspec(dllexport)
#else
#define DECL_DLL
#endif
#include "ParametricParticlesMP/WeakPointer.h"

#ifdef PLATFORM_UNIX
#define DECL_DLL 
#endif

bool InWED();

#define ID_ParametricParticles 4242
#define ID_SpawnShapeBox       4243
#define ID_SpawnShapeSphere    4244
#define ID_SpawnShapeCylinder  4245
#define ID_ParticleRotation    4246
#define ID_ParticleVelocity    4247
#define ID_AutoHeightMap       4248
#define ID_SpawnShapeBase      4249

#define ENTITY_ID(entity) entity->GetClass()->ec_pdecDLLClass->dec_iID
#define ANCESTOR_ID(entity) entity->GetClass()->ec_pdecDLLClass->dec_pdecBase->dec_iID
