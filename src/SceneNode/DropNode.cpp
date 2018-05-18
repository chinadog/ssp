#include "DropNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"
#include "IFileSystem.h"
#include "Common/Common.h"

#include "GamePark.h"

//! constructor
DropNode::DropNode(GamePark* gamePark, scene::IMesh* mesh, ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
            const core::vector3df& position, const core::vector3df& rotation,
            const core::vector3df& scale)
    : MeshSceneNode(mesh, parent, mgr, id, position, rotation, scale),
      m_gamePark(gamePark)
{
    #ifdef _DEBUG
    setDebugName("CMeshSceneNode");
    #endif

    setMesh(mesh);

    active.connect_member(m_gamePark->player(),&Player::grab);
}

//! destructor
DropNode::~DropNode()
{
    std::cout << "DROP NODE DELETE" << std::endl;
}

void DropNode::OnAnimate(u32 timeMs)
{
    f32 distance = getPosition().getDistanceFrom(m_gamePark->player()->camera()->getPosition());
    if(distance < 10)
    {
        active.Emit(this);
    }
    MeshSceneNode::OnAnimate(timeMs);
}
