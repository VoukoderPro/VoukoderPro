#ifndef DESIGNER_TYPES
#define DESIGNER_TYPES
#include "../VoukoderPro/voukoderpro_api.h"
#include "qmetatype.h"

Q_DECLARE_METATYPE(VoukoderPro::NodeInfo)
Q_DECLARE_METATYPE(VoukoderPro::SceneInfo)
Q_DECLARE_METATYPE(VoukoderPro::AssetInfo)
Q_DECLARE_METATYPE(std::shared_ptr<VoukoderPro::SceneInfo>)

#endif // DESIGNER_TYPES