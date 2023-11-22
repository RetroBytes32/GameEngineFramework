#include "../configuration.h"

#ifdef RUN_UNIT_TESTS
 #include "../../tests/framework.h"
#endif

#include "../../vendor/CodeBaseLibrary/poolallocator.h"
#include "../../vendor/CodeBaseLibrary/strings.h"
#include "../../vendor/CodeBaseLibrary/types.h"
#include "../../vendor/CodeBaseLibrary/random.h"
#include "../../vendor/CodeBaseLibrary/logging.h"
#include "../../vendor/CodeBaseLibrary/fileloader.h"
#include "../../vendor/CodeBaseLibrary/timer.h"

#include "../Input/InputSystem.h"
#include "../Physics/PhysicsSystem.h"
#include "../Renderer/RenderSystem.h"
#include "../Resources/ResourceManager.h"
#include "../Scripting/ScriptSystem.h"
#include "../Serialization/Serialization.h"
#include "../AI/ActorSystem.h"

#include "../application/applicationlayer.h"

#include "../engine/engine.h"
